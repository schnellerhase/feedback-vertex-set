#pragma once

#include <iostream>
#include <scip/scip.h>
#include <scip/scip_exception.hpp>
#include <scip/scipdefplugins.h>

#include <array>
#include <span>
#include <vector>

#include "fvs/discrete/discrete.hpp"
#include "fvs/solvers/detail/util.hpp"

namespace fvs::detail {

class VCoverSolver
{
  private:
    SCIP* _scip{};
    std::span<SCIP_VAR*> _vars;
    const UndirectedGraph& _data;

  public:
    VCoverSolver() = delete;
    VCoverSolver(const VCoverSolver& other) = delete;
    VCoverSolver(VCoverSolver&& other) = delete;

    VCoverSolver& operator=(VCoverSolver other) = delete;
    VCoverSolver& operator=(VCoverSolver&& other) = delete;

    explicit VCoverSolver(const UndirectedGraph& data)
      : _vars(new SCIP_Var*[data.N()], data.N())
      , _data(data)
    {
        SCIP_CALL_EXC(SCIPcreate(&_scip));
        SCIP_CALL_EXC(SCIPcreateProbBasic(_scip, "vertex cover problem"));
        SCIP_CALL_EXC(SCIPsetObjsense(_scip, SCIP_OBJSENSE_MINIMIZE));
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "constraints/agelimit", 10));
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "constraints/obsoleteage", 10));
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "separating/cutagelimit", 20));

        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "display/verblevel", 0));
        SCIP_CALL_EXC(SCIPsetIntParam(_scip, "display/freq", 0));

        // Turn off presolving
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "presolving/maxrounds", 0))
        // SCIP_CALL_EXC(SCIPsetRealParam(_scip, "limits/time", 600))

        SCIP_CALL_EXC(SCIPincludeDefaultPlugins(_scip));
        // SCIP_CALL_EXC(SCIPsetIntParam(_scip, "branching/pscost/priority",
        // 536870911))

        for (auto& var : _vars)
        {
            constexpr char* name = nullptr;
            constexpr SCIP_Real lb = 0.0;
            constexpr SCIP_Real ub = 1.0;
            constexpr SCIP_Real obj = 1.0;
            SCIP_CALL_EXC(SCIPcreateVarBasic(_scip, &var, name, lb, ub, obj, SCIP_VARTYPE_BINARY));

            SCIP_CALL_EXC(SCIPaddVar(_scip, var));
            // SCIP_CALL_EXC(SCIPreleaseVar(_scip, &var));
        }

        for (index_t e = 0; e < _data.M(); ++e) {
            std::array<SCIP_Real, 2> cvals = { 1.0, 1.0 };
            std::array<SCIP_VAR*, 2> cvars = { _vars[_data.tails()[e]],
                                               _vars[_data.heads()[e]] };

            SCIP_CONS* cons = nullptr;
            SCIPcreateConsBasicLinear(_scip,
                                      &cons,
                                      "",
                                      2,
                                      cvars.data(),
                                      cvals.data(),
                                      1.0,
                                      SCIPinfinity(_scip));
            SCIPaddCons(_scip, cons);
            SCIPreleaseCons(_scip, &cons);
        }
    }

    ~VCoverSolver()
    {
        for (auto& var : _vars)
            SCIP_CALL_EXC(SCIPreleaseVar(_scip, &var));

        try {
            SCIPfree(&_scip);

            BMScheckEmptyMemory();
        } catch (SCIPException& /* e */) {
        }

        delete[] _vars.data();
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

        SCIP_SOL* sol = nullptr;
        SCIPcreateSol(_scip, &sol, nullptr);

        for (index_t i = 0; i < vc.size(); i++)
            SCIPsetSolVal(_scip, sol, _vars[i], (vc[i]) * 1.0);

        SCIP_Bool kept = 0;
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
            vc[i] = round_to_bool(SCIPgetSolVal(_scip, sol, _vars[i]));

        return vc;
    }
};
}