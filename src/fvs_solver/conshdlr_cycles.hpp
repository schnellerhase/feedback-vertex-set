#pragma once

#include <iostream>

#include "cycle_separation.hpp"
#include "discrete/discrete.hpp"
#include <objscip/objscip.h>

struct SCIP_ConsData
{};

static SCIP_RETCODE
sepaCycle(SCIP* scip,              
          SCIP_CONSHDLR* conshdlr, 
          SCIP_CONS** conss,       
          int nconss,              
          int nusefulconss, 
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
            SCIP_CALL(SCIPaddVarToRow(scip, row, vars[data.tails()[e]], 1.0)); // NOLINT

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
            SCIPaddVarLocksType(
              scip, _vars[v], SCIP_LOCKTYPE_MODEL, nlockspos, nlocksneg); // NOLINT
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
SCIPcreateConsCycle(
  SCIP* scip,        
  SCIP_CONS** cons,  
  const char* name,  
  SCIP_Bool initial, 
  SCIP_Bool
    separate, 
  SCIP_Bool
    enforce, 
  SCIP_Bool check,     
  SCIP_Bool propagate, 
  SCIP_Bool local,     
  SCIP_Bool
    modifiable, 
  SCIP_Bool dynamic,  
  SCIP_Bool removable 
)
{
    SCIP_CONSHDLR* conshdlr = SCIPfindConshdlr(scip, "Cycles");;
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
