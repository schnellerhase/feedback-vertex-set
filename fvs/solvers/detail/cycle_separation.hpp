#pragma once

#include <iostream>

#include "cycle_separation.hpp"
#include "fvs/discrete/discrete.hpp"
#include <objscip/objscip.h>

#ifdef __unix__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
struct SCIP_ConsData
{};

namespace fvs::detail {

static const double INF = std::numeric_limits<double>::max();
static const double EPS = 1e-6;

class CycleSeparation // NOLINT
{
  public:
    CycleSeparation(const Graph& graph)
      : _graph(graph)
      , _cuts()
      , _pe(graph.N())
      , _pv(graph.N())
      , _pl(graph.N())
      , _dist(graph.N())
      , _weight(graph.M())
      , _heap()
      , _heapid(graph.M())
      , _heaprun(0)
    {
        _heap.reserve(graph.M());
        _cuts.reserve(graph.M());
    }

    ~CycleSeparation() = default;

    void separate(double* x)
    {
        update_weights(x);

        _cuts.resize(0);

        find_cycles<true>();
    }

    bool check(double* x)
    {
        update_weights(x);
        return !find_cycles<false>();
    }

    [[nodiscard]] const std::vector<std::vector<index_t>>& cuts() const
    {
        return _cuts;
    }

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
    int _heaprun; // NOLINT

    void update_weights(double* x)
    {
        for (index_t e = 0; e < _graph.M(); ++e)
            _weight[e] = (1.0 - x[e]); // NOLINT
    }

    template<bool cut>
    bool find_cycle_from(index_t s)
    {
        for (index_t i = 0; i < _graph.N(); ++i) {
            _dist[i] = INF;
            _pv[i] = std::numeric_limits<index_t>::max();
            _pl[i] = std::numeric_limits<index_t>::max();
            _heapid[i] = 0;
        }

        _heaprun = 0;
        _heap.resize(0);

        const index_t start = s;
        _dist[start] = 0.0;
        _pv[start] = 0;
        _pl[start] = 0;

        _heap.push_back(start);
        _heapid[start] = _heaprun++;

        for (index_t k = 0; k < _graph.indeg()[s]; ++k) {
            const index_t t = _graph.inadj()[s][k];

            if constexpr (cut)
                if (t < s)
                    continue;

            double w_cl = _weight[_graph.in2arc()[s][k]];

            if (w_cl + EPS > 1.0)
                continue;

            if (_dist[t] < INF && _heapid[t] < 0) {
                if (_dist[t] + w_cl + EPS < 1.0) {

                    if constexpr (cut)
                        cut_cycle(s, t, k);
                    else
                        return true;
                }
            }

            while (!_heap.empty()) {
                index_t node = _heap[0];
                std::pop_heap(_heap.begin(), _heap.end(), std::greater<>{});
                _heap.resize(_heap.size() - 1);

                _heapid[node] = -1;

                if (_dist[node] + EPS > 1.0) {
                    continue;
                }

                for (index_t l = 0; l < _graph.outdeg()[node]; ++l) {
                    const index_t j = _graph.outadj()[node][l];

                    if constexpr (cut)
                        if (j < s)
                            continue;

                    const double new_dist_j =
                      _weight[_graph.out2arc()[node][l]] + _dist[node];

                    if constexpr (cut)
                        if (new_dist_j >= 1.0)
                            continue;

                    if (_heapid[j] >= 0 &&
                        ((new_dist_j < _dist[j]) ||
                         (new_dist_j <= _dist[j] && _pl[node] + 1 < _pl[j]))) {
                        _dist[j] = new_dist_j;
                        _pv[j] = node;
                        _pe[j] = _graph.out2arc()[node][l];
                        _pl[j] = _pl[node] + 1;

                        if (!_heapid[j]) {

                            _heap.push_back(j);

                            std::push_heap(
                              _heap.begin(), _heap.end(), std::greater<>{});

                            _heapid[j] = _heaprun++;
                        } else {
                            if (!std::is_heap(
                                  _heap.begin(), _heap.end(), std::greater<>{}))
                                std::make_heap(
                                  _heap.begin(), _heap.end(), std::greater<>{});
                        }
                    }
                }

                if (node == t) {

                    if (_dist[node] + w_cl + EPS < 1.0) {
                        if constexpr (cut)
                            cut_cycle(s, t, k);
                        else
                            return true;
                    }

                    break;
                }
            }
        }
        return false;
    }

    template<bool cut>
    bool find_cycles()
    {
        for (index_t s = 0; s < _graph.N(); ++s) {
            bool result = find_cycle_from<cut>(s);
            if constexpr (!cut)
                if (result)
                    return true;
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
          SCIP_VAR** vars,
          bool enfo)
{
    assert(scip != nullptr);
    assert(vars != nullptr);

    *result = SCIP_DIDNOTRUN;

    std::vector<double> x(data.M());
    for (index_t e = 0; e < data.M(); ++e)
        x[e] = 1.0 - SCIPgetSolVal(scip, sol, vars[data.tails()[e]]); // NOLINT

    csep->separate(x.data());

    bool one_efficacious(false);

    int effi(0);
    const auto& cuts = csep->cuts();
    for (index_t i = 0; i < cuts.size() && (*result != SCIP_CUTOFF); ++i) {
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

        for (const auto& e : cuts[i])
            SCIP_CALL(
              SCIPaddVarToRow(scip, row, vars[data.tails()[e]], 1.0)); // NOLINT

        SCIP_CALL(SCIPflushRowExtensions(scip, row));

        if (SCIPisCutEfficacious(scip, sol, row)) {
            one_efficacious = true;
            SCIP_Bool infeasible = false;
            SCIP_CALL(SCIPaddRow(scip, row, FALSE, &infeasible));
            // SCIP_CALL(SCIPaddPoolCut(scip, row));
            if (infeasible) {
                *result = SCIP_CUTOFF;
            } else {
                if (enfo) {
                    *result = SCIP_CONSADDED;
                } else {
                    *result = SCIP_SEPARATED;
                }
            }
            ++effi;
        }
        SCIP_CALL(SCIPreleaseRow(scip, &row));

        if (effi > 20000) // NOLINT
            break;
    }

    if (0 == cuts.size()) {
        if (enfo) {
            *result = SCIP_FEASIBLE;
        } else {
            *result = SCIP_DIDNOTFIND;
        }
    }

    if (cuts.size() && one_efficacious == false) {
        if (enfo) {
            *result = SCIP_INFEASIBLE;
        } else {
            *result = SCIP_DIDNOTFIND;
        }
    }

    return SCIP_OKAY;
}

/** C++ constraint handler for cycle inequalities (on edge variables) */
class ConshdlrCycles : public scip::ObjConshdlr // NOLINT
{

  public:
    const Graph& _data;
    SCIP_VAR** _vars;
    CycleSeparation* _csep;

    ConshdlrCycles(const Graph& data, SCIP* scip, SCIP_VAR** vars)
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

    virtual SCIP_DECL_CONSCHECK(scip_check)
    {
        assert(result != nullptr);

        std::vector<double> x(_data.M());
        for (index_t e = 0; e < _data.M(); ++e) {
            index_t v = _data.tails()[e];
            x[e] = 1.0 - SCIPgetSolVal(scip, sol, _vars[v]); // NOLINT
        }

        bool feasible = _csep->check(x.data());

        if (feasible)
            *result = SCIP_FEASIBLE;
        else
            *result = SCIP_INFEASIBLE;

        return SCIP_OKAY;
    }

    virtual SCIP_DECL_CONSENFOLP(scip_enfolp)
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

    virtual SCIP_DECL_CONSENFOPS(scip_enfops)
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

    virtual SCIP_DECL_CONSLOCK(scip_lock)
    {
        for (index_t v = 0; v < _data.N(); ++v) {
            SCIPaddVarLocksType(scip,
                                _vars[v], // NOLINT
                                SCIP_LOCKTYPE_MODEL,
                                nlockspos,
                                nlocksneg);
        }

        return SCIP_OKAY;
    }

    virtual SCIP_DECL_CONSTRANS(scip_trans)
    {

        //    SCIP_CONSDATA *sourcedata;
        SCIP_CONSDATA* targetdata = nullptr;

        //    sourcedata = SCIPconsGetData(sourcecons);
        // assert(sourcedata != nullptr);

        SCIP_CALL(SCIPallocBlockMemory(scip, &targetdata));
        // targetdata->graph = sourcedata->graph;

        // create target constraint
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

    virtual SCIP_DECL_CONSHDLRCLONE(scip::ObjProbCloneable* clone)
    {
        assert(valid != nullptr);
        *valid = true;
        return new ConshdlrCycles(_data, scip, _vars);
    }

    virtual SCIP_DECL_CONSSEPASOL(scip_sepasol)
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

    virtual SCIP_DECL_CONSSEPALP(scip_sepalp)
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

    virtual SCIP_DECL_CONSDELETE(scip_delete)
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
    ;
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

#ifdef __unix__
#pragma GCC diagnostic pop
#endif