#pragma once

#include <iostream>

#include "cycle_separation.hpp"
#include "fvs/discrete/discrete.hpp"
#include "fvs/solvers/detail/util.hpp"
#include <objscip/objscip.h>
#include <scip/type_result.h>

struct SCIP_ConsData
{};

namespace fvs::detail {

class CycleSeparation
{
  public:
    CycleSeparation() = delete;
    CycleSeparation(const CycleSeparation& other) = delete;
    CycleSeparation(CycleSeparation&& other) = delete;

    CycleSeparation& operator=(CycleSeparation other) = delete;
    CycleSeparation& operator=(CycleSeparation&& other) = delete;

    explicit CycleSeparation(const Graph& graph)
      : _graph(graph)
      , _cuts()
      , _pe(graph.N())
      , _pv(graph.N())
      , _pl(graph.N())
      , _dist(graph.N())
      , _weight(graph.M())
      , _heap()
      , _heapid(graph.M())
    {
    }

    ~CycleSeparation() = default;

    void separate()
    {
        _cuts.clear();
        find_cycles<true>();
    }

    bool check() { return !find_cycles<false>(); }

    [[nodiscard]] const std::vector<std::vector<index_t>>& cuts() const
    {
        return _cuts;
    }
    [[nodiscard]] std::vector<double>& weights() { return _weight; }

  private:
    const Graph& _graph;

    std::vector<std::vector<index_t>> _cuts;
    std::vector<index_t> _pe; // outgoing edge
    std::vector<index_t> _pv; // outgoing vertex connected with prev edge
    std::vector<index_t> _pl; // length of cycle?
    std::vector<double> _dist;
    std::vector<double> _weight;

    std::vector<index_t> _heap;
    std::vector<int> _heapid;

    template<bool cut>
    bool find_cycles()
    {
        for (index_t vertex_s = 0; vertex_s < _graph.N(); vertex_s++) {

            for (index_t i = 0; i < _graph.N(); i++) {
                _dist[i] = INF;
                _pv[i] = std::numeric_limits<index_t>::max();
                _pl[i] = std::numeric_limits<index_t>::max();
                _heapid[i] = 0;
            }

            _heap.resize(0);
            int heaprun = 0;

            _heap.push_back(vertex_s);
            _dist[vertex_s] = 0.0;
            _pv[vertex_s] = 0;
            _pl[vertex_s] = 0;
            _heapid[vertex_s] = heaprun++;

            for (index_t k = 0; k < _graph.indeg()[vertex_s]; k++) {
                const auto& vertex_t = _graph.inadj()[vertex_s][k];

                // we have an edge t -> s

                if constexpr (cut)
                    if (vertex_t < vertex_s)
                        continue;

                const auto& weight_ts = _weight[_graph.in2arc()[vertex_s][k]];

                bool ts_removed = round_to_bool(weight_ts);
                if (ts_removed)
                    continue;

                if (_dist[vertex_t] < INF && _heapid[vertex_t] < 0 &&
                    !round_to_bool(_dist[vertex_t] + weight_ts)) {

                    if constexpr (cut)
                        cut_cycle(vertex_s, vertex_t, k);
                    else
                        return true;
                }

                while (!_heap.empty()) {
                    std::pop_heap(_heap.begin(), _heap.end(), std::greater<>{});
                    index_t vertex = _heap.back();
                    _heap.pop_back();

                    _heapid[vertex] = -1;

                    if (round_to_bool(_dist[vertex]))
                        continue;

                    for (index_t l = 0; l < _graph.outdeg()[vertex]; ++l) {
                        const index_t j = _graph.outadj()[vertex][l];

                        if constexpr (cut)
                            if (j < vertex_s)
                                continue;

                        const double new_dist_j =
                          _weight[_graph.out2arc()[vertex][l]] + _dist[vertex];

                        if constexpr (cut)
                            if (new_dist_j >= 1.0)
                                continue;

                        bool on_heap = _heapid[j] >= 0;
                        if (!on_heap)
                            continue;

                        bool better_distance = new_dist_j < _dist[j];
                        bool better_length = (new_dist_j == _dist[j]) &&
                                             (_pl[vertex] + 1 < _pl[j]);
                        if (better_distance || better_length) {
                            _dist[j] = new_dist_j;
                            _pv[j] = vertex;
                            _pe[j] = _graph.out2arc()[vertex][l];
                            _pl[j] = _pl[vertex] + 1;

                            if (_heapid[j])
                                continue;

                            _heap.push_back(j);

                            std::push_heap(
                              _heap.begin(), _heap.end(), std::greater<>{});

                            _heapid[j] = heaprun++;
                        }
                    }

                    if (vertex != vertex_t)
                        continue;

                    if (!round_to_bool(_dist[vertex] + weight_ts)) {
                        if constexpr (cut)
                            cut_cycle(vertex_s, vertex_t, k);
                        else
                            return true;
                    }

                    break;
                }
            }
        }
        return false;
    }

    void cut_cycle(
      index_t s,
      index_t t,
      index_t k) // TODO: should i not be able to compute one of this on my own?
    {
        // we have found a cycle t -> s with edge between t->s is given by k-th
        // in edge of s.

        _cuts.emplace_back(_pl[t] + 1);

        for (index_t v = t; v != s; v = _pv[v])
            _cuts.back()[_pl[v]] = _pe[v];

        _cuts.back()[0] = _graph.in2arc()[s][k];
    }
};

static SCIP_RETCODE
sepaCycle(SCIP* scip,
          SCIP_CONSHDLR* conshdlr,
          SCIP_CONS** /* conss */,
          int /* nconss */,
          int /* nusefulconss */,
          SCIP_SOL* sol,
          SCIP_RESULT* result,
          const Graph& data,
          CycleSeparation* csep,
          std::span<SCIP_VAR*> vars,
          bool enfo)
{
    assert(scip != nullptr);
    assert(vars.data() != nullptr);

    *result = SCIP_DIDNOTRUN;

    for (index_t e = 0; e < data.M(); ++e)
        csep->weights()[e] = SCIPgetSolVal(scip, sol, vars[data.tails()[e]]);

    csep->separate();

    int effi(0);
    const auto& cuts = csep->cuts();
    for (const auto& cut : cuts) {
        SCIP_ROW* row = nullptr;
        SCIP_CALL(SCIPcreateEmptyRowConshdlr(scip,
                                             &row,
                                             conshdlr,
                                             "Cycle",
                                             1.0,
                                             SCIPinfinity(scip),
                                             FALSE,
                                             FALSE,
                                             TRUE));

        SCIP_CALL(SCIPcacheRowExtensions(scip, row));

        for (const auto& e : cut)
            SCIP_CALL(SCIPaddVarToRow(scip, row, vars[data.tails()[e]], 1.0));

        SCIP_CALL(SCIPflushRowExtensions(scip, row));

        if (SCIPisCutEfficacious(scip, sol, row)) {
            SCIP_Bool infeasible = false;
            SCIP_CALL(SCIPaddRow(scip, row, FALSE, &infeasible));
            // SCIP_CALL(SCIPaddPoolCut(scip, row));
            if (infeasible) {
                *result = SCIP_CUTOFF;
                break;
            } else {
                *result = enfo ? SCIP_CONSADDED : SCIP_SEPARATED;
            }

            ++effi;
        }

        SCIP_CALL(SCIPreleaseRow(scip, &row));

        if (effi > 20000) // NOLINT
            break;
    }

    if (0 == cuts.size())
        *result = enfo ? SCIP_FEASIBLE : SCIP_DIDNOTFIND;

    if (cuts.size() && effi == 0)
        *result = enfo ? SCIP_INFEASIBLE : SCIP_DIDNOTFIND;

    return SCIP_OKAY;
}

/** C++ constraint handler for cycle inequalities (on edge variables) */
class ConshdlrCycles : public scip::ObjConshdlr
{

  public:
    const Graph& _data;
    std::span<SCIP_VAR*> _vars;
    CycleSeparation* _csep;

    ConshdlrCycles() = delete;
    ConshdlrCycles(const ConshdlrCycles& other) = delete;
    ConshdlrCycles(ConshdlrCycles&& other) = delete;

    ConshdlrCycles& operator=(ConshdlrCycles other) = delete;
    ConshdlrCycles& operator=(ConshdlrCycles&& other) = delete;

    explicit ConshdlrCycles(const Graph& data,
                            SCIP* scip,
                            std::span<SCIP_VAR*> vars)
      : ObjConshdlr(scip,
                    "Cycles",
                    "Edge-based Cycle Separator",
                    2147483646, // NOLINT
                    2147483646, // NOLINT
                    2147483646, // NOLINT
                    1,
                    1,
                    1,
                    100, // NOLINT
                    FALSE,
                    FALSE,
                    TRUE,
                    SCIP_PROPTIMING_BEFORELP,
                    SCIP_PRESOLTIMING_FAST)
      , _data(data)
      , _vars(vars)
      , _csep(new CycleSeparation(data))
    {
    }

    ~ConshdlrCycles() override { delete _csep; }

    SCIP_RETCODE scip_check(SCIP* scip,
                            SCIP_CONSHDLR* /* conshdlr */,
                            SCIP_CONS** /* conss */,
                            int /* nconss */,
                            SCIP_SOL* sol,
                            SCIP_Bool /* checkintegrality */,
                            SCIP_Bool /* checklprows */,
                            SCIP_Bool /* printreason */,
                            SCIP_Bool /* completely */,
                            SCIP_RESULT* result) override
    {
        assert(result != nullptr);

        for (index_t e = 0; e < _data.M(); ++e)
            _csep->weights()[e] =
              SCIPgetSolVal(scip, sol, _vars[_data.tails()[e]]);

        bool feasible = _csep->check();

        if (feasible)
            *result = SCIP_FEASIBLE;
        else
            *result = SCIP_INFEASIBLE;

        return SCIP_OKAY;
    }

    SCIP_RETCODE scip_enfolp(SCIP* scip,
                             SCIP_CONSHDLR* conshdlr,
                             SCIP_CONS** conss,
                             int nconss,
                             int nusefulconss,
                             SCIP_Bool /* solinfeasible */,
                             SCIP_RESULT* result) override
    {
        assert(result != nullptr);

        SCIP_CALL(sepaCycle(scip,
                            conshdlr,
                            conss,
                            nconss,
                            nusefulconss,
                            nullptr,
                            result,
                            _data,
                            _csep,
                            _vars,
                            true));

        return SCIP_OKAY;
    }

    SCIP_RETCODE scip_enfops(SCIP* scip,
                             SCIP_CONSHDLR* conshdlr,
                             SCIP_CONS** conss,
                             int nconss,
                             int nusefulconss,
                             SCIP_Bool /* solinfeasible */,
                             SCIP_Bool /* objinfeasible */,
                             SCIP_RESULT* result) override
    {
        assert(result != nullptr);

        SCIP_CALL(sepaCycle(scip,
                            conshdlr,
                            conss,
                            nconss,
                            nusefulconss,
                            nullptr,
                            result,
                            _data,
                            _csep,
                            _vars,
                            true));

        return SCIP_OKAY;
    }

    SCIP_RETCODE scip_lock(SCIP* scip,
                           SCIP_CONSHDLR* /* conshdlr */,
                           SCIP_CONS* /* cons */,
                           SCIP_LOCKTYPE /* locktype */,
                           int nlockspos,
                           int nlocksneg) override
    {
        for (index_t v = 0; v < _data.N(); ++v)
            SCIPaddVarLocksType(
              scip, _vars[v], SCIP_LOCKTYPE_MODEL, nlockspos, nlocksneg);

        return SCIP_OKAY;
    }

    SCIP_DECL_CONSTRANS(scip_trans) override
    {
        SCIP_CONSDATA* targetdata = nullptr;
        SCIP_CALL(SCIPallocBlockMemory(scip, &targetdata));

        SCIP_CALL(SCIPcreateCons(scip,
                                 targetcons,
                                 SCIPconsGetName(sourcecons),
                                 conshdlr,
                                 targetdata,
                                 SCIPconsIsInitial(sourcecons),
                                 SCIPconsIsSeparated(sourcecons),
                                 SCIPconsIsEnforced(sourcecons),
                                 SCIPconsIsChecked(sourcecons),
                                 SCIPconsIsPropagated(sourcecons),
                                 SCIPconsIsLocal(sourcecons),
                                 SCIPconsIsModifiable(sourcecons),
                                 SCIPconsIsDynamic(sourcecons),
                                 SCIPconsIsRemovable(sourcecons),
                                 SCIPconsIsStickingAtNode(sourcecons)));

        return SCIP_OKAY;
    }

    SCIP_DECL_CONSHDLRCLONE(scip::ObjProbCloneable* clone) override
    {
        assert(valid != nullptr);
        *valid = true;
        return new ConshdlrCycles(_data, scip, _vars);
    }

    SCIP_DECL_CONSSEPASOL(scip_sepasol) override
    {
        SCIP_CALL(sepaCycle(scip,
                            conshdlr,
                            conss,
                            nconss,
                            nusefulconss,
                            sol,
                            result,
                            _data,
                            _csep,
                            _vars,
                            false));

        return SCIP_OKAY;
    }

    SCIP_DECL_CONSSEPALP(scip_sepalp) override
    {
        SCIP_CALL(sepaCycle(scip,
                            conshdlr,
                            conss,
                            nconss,
                            nusefulconss,
                            nullptr,
                            result,
                            _data,
                            _csep,
                            _vars,
                            false));

        return SCIP_OKAY;
    }

    SCIP_RETCODE scip_delete(SCIP* scip,
                             SCIP_CONSHDLR* /* conshdlr */,
                             SCIP_CONS* /* cons */,
                             SCIP_CONSDATA** consdata) override
    {
        assert(consdata != nullptr);

        SCIPfreeBlockMemory(scip, consdata);

        return SCIP_OKAY;
    }
};

SCIP_RETCODE
SCIPcreateConsCycle(SCIP* scip,
                    SCIP_CONS** cons,
                    const char* name,
                    SCIP_Bool initial,
                    SCIP_Bool separate,
                    SCIP_Bool enforce,
                    SCIP_Bool check,
                    SCIP_Bool propagate,
                    SCIP_Bool local,
                    SCIP_Bool modifiable,
                    SCIP_Bool dynamic,
                    SCIP_Bool removable)
{
    SCIP_CONSHDLR* conshdlr = SCIPfindConshdlr(scip, "Cycles");

    if (conshdlr == nullptr) {
        SCIPerrorMessage("constraint handler not found\n");
        return SCIP_PLUGINNOTFOUND;
    }

    SCIP_CONSDATA* consdata = nullptr;
    SCIP_CALL(SCIPallocBlockMemory(scip, &consdata));

    SCIP_CALL(SCIPcreateCons(scip,
                             cons,
                             name,
                             conshdlr,
                             consdata,
                             initial,
                             separate,
                             enforce,
                             check,
                             propagate,
                             local,
                             modifiable,
                             dynamic,
                             removable,
                             FALSE));

    return SCIP_OKAY;
}
}
