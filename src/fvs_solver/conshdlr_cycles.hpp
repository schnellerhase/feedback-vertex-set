#pragma once

#include <iostream>

#include "cycle_separation.hpp"
#include "discrete/discrete.hpp"
#include <objscip/objscip.h>

#ifndef SINGLETON_SCIP_CONSDATA
#define SINGLETON_SCIP_CONSDATA
struct SCIP_ConsData
{};
#endif

using namespace std;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
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
{ /*lint --e{715}*/
    assert(scip != nullptr);
    // assert(data != nullptr);
    assert(vars != nullptr);

    *result = SCIP_DIDNOTRUN;

    std::vector<double> x(data.M());
    for (index_t e = 0; e < data.M(); ++e)
        x[e] = 1.0 - SCIPgetSolVal(scip, sol, vars[data.tails()[e]]);

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
            SCIP_CALL(SCIPaddVarToRow(scip, row, vars[data.tails()[e]], 1.0));

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

        if (effi > 20000)
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
#pragma GCC diagnostic pop

/** C++ constraint handler for cycle inequalities (on edge variables) */
class ConshdlrCycles : public scip::ObjConshdlr
{

  public:
    const Graph& _data;
    SCIP_VAR** _vars;
    CycleSeparation* _csep;
    int num_calls_enfolp = 0, num_calls_enfops = 0, num_calls_check = 0,
        num_calls_lock = 0, num_calls_trans = 0, num_calls_sepalp = 0,
        num_calls_sepasol = 0;

    /** constructor */
    ConshdlrCycles(const Graph& data, SCIP* scip, SCIP_VAR** vars)
      : ObjConshdlr(scip,
                    "Cycles",
                    "Edge-based Cycle Separator",
                    2147483646,
                    2147483646,
                    2147483646,
                    1,
                    1,
                    1,
                    100,
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

    virtual ~ConshdlrCycles() { delete _csep; }

    virtual SCIP_DECL_CONSCHECK(scip_check)
    { /*lint --e{715}*/
        num_calls_check++;

        assert(result != nullptr);

        auto* x = new double[_data.M()];
        for (index_t e = 0; e < _data.M(); ++e) {
            int v = _data.tails()[e];
            x[e] = 1.0 - SCIPgetSolVal(scip, sol, _vars[v]);
        }

        bool feasible = _csep->check(x);

        delete[] x;

        if (feasible)
            *result = SCIP_FEASIBLE;
        else
            *result = SCIP_INFEASIBLE;

        return SCIP_OKAY;
    }

    virtual SCIP_DECL_CONSENFOLP(scip_enfolp)
    { /*lint --e{715}*/
        num_calls_enfolp++;
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
    { /*lint --e{715}*/
        num_calls_enfops++;
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
    { /*lint --e{715}*/
        num_calls_lock++;

        for (index_t v = 0; v < _data.N(); ++v) {
            SCIPaddVarLocksType(
              scip, _vars[v], SCIP_LOCKTYPE_MODEL, nlockspos, nlocksneg);
        }

        return SCIP_OKAY;
    }

    virtual SCIP_DECL_CONSTRANS(scip_trans)
    {
        num_calls_trans++;

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

    virtual SCIP_DECL_CONSHDLRCLONE(
      scip::ObjProbCloneable* clone) /*lint !e665*/
    {
        assert(valid != nullptr);
        *valid = true;
        return new ConshdlrCycles(_data, scip, _vars);
    }

    virtual SCIP_DECL_CONSSEPASOL(scip_sepasol)
    {
        num_calls_sepasol++;

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
        num_calls_sepalp++;

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
    { /*lint --e{715}*/
        assert(consdata != NULL);

        SCIPfreeBlockMemory(scip, consdata);

        return SCIP_OKAY;
    }

    // Additional functions
    void printStatistics(std::ostream& out) const
    {
        out << "Callback calls tri enfolp: " << num_calls_enfolp << std::endl;
        out << "Callback calls tri enfops: " << num_calls_enfops << std::endl;
        out << "Callback calls tri check: " << num_calls_check << std::endl;
        out << "Callback calls tri lock: " << num_calls_lock << std::endl;
        out << "Callback calls tri trans: " << num_calls_trans << std::endl;
        out << "Callback calls tri sepa lp: " << num_calls_sepalp << std::endl;
        out << "Callback calls tri sepa sol: " << num_calls_sepasol
            << std::endl;
    }

}; /*lint !e1712*/

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
    SCIP_CALL(SCIPallocBlockMemory(scip, &consdata)); /*lint !e530*/

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
