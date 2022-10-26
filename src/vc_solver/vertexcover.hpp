#pragma once

#include <iostream>
#include <scip/scip.h>
#include <scip/scipdefplugins.h>
#include <scip/scip_exception.hpp>

#include <vector>

#include "discrete/discrete.hpp"

class VCoverSolver
{
  private:
    SCIP* _scip{};
    SCIP_VAR** _vars;
    const UndirectedGraph& _data;

  public:
    VCoverSolver(const UndirectedGraph& data)
      : _data(data)
      , _vars(new SCIP_Var*[data.N()])
    {
        SCIP_CALL_EXC(SCIPcreate(&_scip));
        SCIP_CALL_EXC(SCIPcreateProbBasic(_scip, "vertex cover problem"));
        SCIP_CALL_EXC(SCIPsetObjsense(_scip, SCIP_OBJSENSE_MINIMIZE));
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "constraints/agelimit", 10));
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "constraints/obsoleteage", 10));
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "separating/cutagelimit", 20));

        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "display/verblevel", 0));
        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "display/freq", 0));

        // ToDo: Check if necessary
        SCIPenableDebugSol(_scip);

        // Turn off presolving
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "presolving/maxrounds", 0))
        // SCIP_CALL_EXC(SCIPsetRealParam(_scip, "limits/time", 600))
        // Add Clique constraint handler

        SCIP_CALL_EXC(SCIPincludeDefaultPlugins(_scip));
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "branching/pscost/priority",
        // 536870911))

        for (index_t v = 0; v < _data.N(); ++v) {
            SCIP_VAR* var;
            SCIP_CALL_EXC(SCIPcreateVarBasic(
              _scip, &var, NULL, 0.0, 1.0, 1.0, SCIP_VARTYPE_BINARY));
            _vars[v] = var;
            SCIP_CALL_EXC(SCIPaddVar(_scip, var));
            SCIP_CALL_EXC(SCIPreleaseVar(_scip, &var));
        }

        SCIP_VAR** cvars = new SCIP_VAR*[2];
        SCIP_Real* cvals = new SCIP_Real[2]{ 1.0, 1.0 };
        for (index_t e = 0; e < _data.M(); ++e) {
            cvars[0] = _vars[_data.tails()[e]];
            cvars[1] = _vars[_data.heads()[e]];

            SCIP_CONS* cons;
            SCIPcreateConsBasicLinear(
              _scip, &cons, "", 2, cvars, cvals, 1.0, SCIPinfinity(_scip));
            SCIPaddCons(_scip, cons);
            SCIPreleaseCons(_scip, &cons);
        }

        delete[] cvars;
        delete[] cvals;
    }

    ~VCoverSolver()
    {
        try {
            SCIPfree(&_scip);

            BMScheckEmptyMemory();
        } catch (SCIPException& e) {
        }

        delete[] _vars;
    }

    void set_time_limit(double seconds)
    {
        SCIP_CALL_EXC(SCIPsetRealParam(_scip, "limits/time", seconds))
    }

    void set_verbosity(int level)
    {
        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "display/verblevel", 4));
        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "display/freq", level));
    }

    void set_initial_solution(const VC& vc)
    {
        assert(vc.size() == _data.N());

        SCIP_SOL* sol;
        SCIPcreateSol(_scip, &sol, NULL);

        for (index_t i = 0; i < vc.size(); i++)
            SCIPsetSolVal(_scip, sol, _vars[i], (vc[i]) * 1.0);

        SCIP_Bool kept;
        SCIPaddSol(_scip, sol, &kept);
        SCIPfreeSol(_scip, &sol);

        if (!kept)
            std::cout << "Could not set initial solution!" << std::endl;
    }

    VC solve()
    {
        SCIP_CALL_EXC(SCIPsolve(_scip)); // SCIPsolveParallel(_scip);

        SCIP_SOL* sol = SCIPgetBestSol(_scip);
        VC vc(_data.N());
        for (index_t i = 0; i < vc.size(); i++)
            vc[i] = (int(SCIPgetSolVal(_scip, sol, _vars[i]) + 2e-6) == 1);

        return vc;
    }
};
