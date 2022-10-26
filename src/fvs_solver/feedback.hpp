#pragma once

#include <iostream>
#include <scip/scip.h>
#include <scip/scipdefplugins.h>
#include <vector>

#include "conshdlr_cycles.hpp"
#include "discrete/discrete.hpp"
#include "scip/scip_exception.hpp"

class FeedbackSolver
{
  private:
    /** @brief pointer to scip structure
     *
     *  SCIP organizes all the problem informations by itself, we can access
     * them by the SCIP * pointer
     */
    SCIP* _scip{};

    /** @brief Graph DataStructure  */
    const Graph& _data;

    /** @brief one binary variable for each edge
     *
     * To access variable information (objective value, bounds,
     * etc.) use the SCIP_VAR * pointer. Since we want to know the
     * value of each variable in the solution, we have to store
     * these pointers.
     */

    SCIP_VAR** _vars;

    /** @brief constraints for each edge
     *
     * To access constraint information (right hand side, left hand
     * side, dual values, etc.) use the SCIP_CONS * pointer. For the
     * n-queens problem we do not really need to store them but we
     * do for illustration.
     */

    SCIP_CONS* _cons;

    ConshdlrCycles* _cycleConsHandler;

  public:
    /** @brief constructs the BP model for the max cut problem
     *
     * @param[in] n the number of nodes
     */
    // explicit FeedbackSolver(Graph &data);

    explicit FeedbackSolver(const Graph& data)
      : _data(data)
      , _vars(new SCIP_Var*[_data.N()])
      , _solution(new int[_data.N()])
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

        _cycleConsHandler = new ConshdlrCycles(_data, _scip, _vars);

        // Turn off presolving
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "presolving/maxrounds", 0))
        //    SCIP_CALL_EXC(SCIPsetRealParam(_scip, "limits/time", 600))
        // Add Clique constraint handler
        SCIP_CALL_EXC(SCIPincludeObjConshdlr(_scip, _cycleConsHandler, FALSE));

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

        /*
         * Add constraint handlers base constraint
         * This is needed, as otherwise the constraint handlers callbacks will
         * never be invoked
         * */

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

    int* _solution;

    void set_UB(int var, double ub)
    {
        SCIP_CALL_EXC(SCIPchgVarUb(_scip, _vars[var], ub));
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

    // fvs shall have the indices of only those vertex which
    // are in the feedback vertex set, nfvs specifies the number of these.
    void set_initial_solution(const FVS& fvs)
    {
        SCIP_SOL* sol;
        SCIPcreateSol(_scip, &sol, NULL);

        for (index_t i = 0; i < fvs.size(); ++i)
            SCIPsetSolVal(_scip, sol, _vars[i], (fvs[i]) * 1.0);

        SCIP_Bool kept;
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
                _solution[v] = int(
                  SCIPgetSolVal(_scip, SCIPgetBestSol(_scip), _vars[v]) + 2e-6);
                //	std::cout << "(" << SCIPvarGetName(_vars[v]) << ": " <<
                //_solution[v]  << ") " << std::endl;
            }

            //    std::cout << "Callback calls: " <<
            //    _cycleConsHandler->callback_calls << std::endl;

            //        _cycleConsHandler->printStatistics(std::cout);

            return true;
        } else {
            return false;
        }
    }

    /** @brief destructor this is the place to release the SCIP_VAR
     * and SCIP_CONS pointers and to free the SCIP pointer
     * afterwards
     */
    ~FeedbackSolver()
    {
        try {
            SCIP_CALL_EXC(SCIPreleaseCons(_scip, &_cons));

            SCIPfree(&_scip);

            BMScheckEmptyMemory();
        } catch (SCIPException& e) {
        }

        delete[] _vars;
        delete _cycleConsHandler;
        delete[] _solution;
    }
};
