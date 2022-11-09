#pragma once

#include <iostream>
#include <memory>
#include <scip/scip.h>
#include <scip/scipdefplugins.h>
#include <vector>

#include "conshdlr_cycles.hpp"
#include "discrete/discrete.hpp"
#include "scip/scip_exception.hpp"

class FeedbackSolver
{
  private:
    const Graph& _data;

    SCIP* _scip{};
    SCIP_VAR** _vars;
    SCIP_CONS* _cons;
    int* _solution;

    std::unique_ptr<ConshdlrCycles> _cycleConsHandler;

  public:
    FeedbackSolver() = delete;
    FeedbackSolver(const FeedbackSolver& other) = delete;
    FeedbackSolver(FeedbackSolver&& other) = delete;

    FeedbackSolver& operator=(FeedbackSolver other) = delete;
    FeedbackSolver& operator=(FeedbackSolver&& other) = delete;

    explicit FeedbackSolver(const Graph& data)
      : _data(data)
      , _vars(new SCIP_Var*[_data.N()])
      , _solution(new int[_data.N()])
      , _cons(nullptr)
    {
        SCIP_CALL_EXC(SCIPcreate(&_scip));
        SCIP_CALL_EXC(SCIPcreateProbBasic(_scip, "feedback problem"));
        SCIP_CALL_EXC(SCIPsetObjsense(_scip, SCIP_OBJSENSE_MINIMIZE));

        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "display/verblevel", 0));
        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "display/freq", 0));
        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "constraints/agelimit", 10));
        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "constraints/obsoleteage", 10));
        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "separating/cutagelimit", 20));
        //    SCIP_CALL_EXC(SCIPsetCharParam(_scip,
        //    "estimation/restarts/restartpolicy", 'a'));
        //    SCIP_CALL_EXC(SCIPsetIntParam(_scip,
        //    "estimation/restarts/restartlimit", -1));
        //    SCIP_CALL_EXC(SCIPsetLongintParam(_scip,
        //    "estimation/restarts/minnodes", 60));
        //    SCIP_CALL_EXC(SCIPsetIntParam(_scip, "limits/autorestartnodes/",
        //    150));

        // ToDo: Check if necessary
        SCIPenableDebugSol(_scip);

        _cycleConsHandler =
          std::make_unique<ConshdlrCycles>(_data, _scip, _vars);

        // Turn off presolving
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "presolving/maxrounds", 0))
        //    SCIP_CALL_EXC(SCIPsetRealParam(_scip, "limits/time", 600))
        // Add Clique constraint handler
        SCIP_CALL_EXC(
          SCIPincludeObjConshdlr(_scip, _cycleConsHandler.get(), FALSE));

        SCIP_CALL_EXC(SCIPincludeDefaultPlugins(_scip));
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "branching/pscost/priority",
        // 536870911))

        for (index_t v = 0; v < _data.N(); ++v) {
            SCIP_VAR* var = nullptr;
            SCIP_CALL_EXC(SCIPcreateVarBasic(
              _scip, &var, nullptr, 0.0, 1.0, 1.0, SCIP_VARTYPE_BINARY));
            _vars[v] = var; // NOLINT
            SCIP_CALL_EXC(SCIPaddVar(_scip, var));
            SCIP_CALL_EXC(SCIPreleaseVar(_scip, &var));
        }

        SCIP_CALL_EXC(SCIPcreateConsCycle(_scip,
                                          &_cons,
                                          "cycle",
                                          FALSE,
                                          TRUE,
                                          TRUE,
                                          TRUE,
                                          TRUE,
                                          FALSE,
                                          FALSE,
                                          FALSE,
                                          TRUE));
        SCIP_CALL_EXC((SCIPaddCons(_scip, _cons)));

        // Turn off presolving
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "presolving/maxrounds", 0))
        //    SCIP_CALL_EXC(SCIPsetRealParam(_scip, "limits/time", 600))
        // Add Clique constraint handler
    }

    void set_UB(int var, double ub)
    {
        SCIP_CALL_EXC(SCIPchgVarUb(_scip, _vars[var], ub)); // NOLINT
    }

    void set_time_limit(double seconds)
    {
        SCIP_CALL_EXC(SCIPsetRealParam(_scip, "limits/time", seconds))
    }

    void set_verbosity(int level)
    {
        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "display/verblevel", 4));
        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "display/freq", 100));
    }

    void set_initial_solution(const FVS& fvs)
    {
        SCIP_SOL* sol = nullptr;
        SCIPcreateSol(_scip, &sol, nullptr);

        for (index_t i = 0; i < fvs.size(); ++i)
            SCIPsetSolVal(_scip, sol, _vars[i], (fvs[i]) * 1.0); // NOLINT

        SCIP_Bool kept = 0;
        SCIPaddSol(_scip, sol, &kept);
        SCIPfreeSol(_scip, &sol);

        if (!kept)
            std::cout << "Could not set initial solution!" << std::endl;
    }

    bool solve()
    {
        SCIP_RETCODE ret = SCIPsolve(_scip);
        //    SCIP_RETCODE ret = SCIPsolveParallel(_scip);

        if (ret == SCIP_OKAY) {
            for (index_t v = 0; v < _data.N(); ++v) {
                _solution[v] = int( // NOLINT
                  SCIPgetSolVal(
                    _scip, SCIPgetBestSol(_scip), _vars[v]) + // NOLINT
                  2e-6);                                      // NOLINT
            }
            return true;
        } else {
            return false;
        }
    }

    [[nodiscard]] int* solution() { return _solution; }

    ~FeedbackSolver()
    {
        try {
            SCIP_CALL_EXC(SCIPreleaseCons(_scip, &_cons));

            SCIPfree(&_scip);

            BMScheckEmptyMemory();
        } catch (SCIPException& e) {
        }

        delete[] _vars;
        delete[] _solution;
    }
};
