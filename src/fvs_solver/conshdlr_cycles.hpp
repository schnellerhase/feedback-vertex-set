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

/** separates triangle cuts */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"
static SCIP_RETCODE
sepaCycle(SCIP* scip,              /**< SCIP data structure */
          SCIP_CONSHDLR* conshdlr, /**< the constraint handler itself */
          SCIP_CONS** conss,       /**< array of constraints to process */
          int nconss,              /**< number of constraints to process */
          int nusefulconss, /**< number of useful (non-obsolete) constraints to
                               process */
          SCIP_SOL* sol,    /**< primal solution that should be separated */
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

    double* x = new double[data.M()];
    for (index_t e = 0; e < data.M(); ++e) {
        int v = data.tails()[e];
        x[e] = 1.0 - SCIPgetSolVal(scip, sol, vars[v]);
    }

    index_t nCuts = csep->separate(x);

    delete[] x;

    bool one_efficacious(false);

    int effi(0);
    for (index_t i = 0; i < nCuts && (*result != SCIP_CUTOFF); ++i) {
        // build row
        SCIP_ROW* row;
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

        //	std::cout << "Cycle " << i << " of length " << csep->_cutlens[i]
        //<<  ": ";
        // double lhs = 0.0;
        for (index_t s = 0; s < csep->_cutlens[i]; ++s) {
            int e = csep->_cutsupport[i][s];
            //	    std::cout << "(" << data.tails()[e] << "," <<
            // data.heads()[e] << ")  ";
            SCIP_CALL(SCIPaddVarToRow(scip, row, vars[data.tails()[e]], 1.0));
            // lhs += SCIPgetSolVal(scip, sol, vars[data.tails()[e]]);
        }
        //	std::cout << std::endl;
        //	std::cout << " with " << lhs << " < " << 1 << std::endl;

        SCIP_CALL(SCIPflushRowExtensions(scip, row));

        // add row
        if (SCIPisCutEfficacious(scip, sol, row)) {
            one_efficacious = true;
            SCIP_Bool infeasible = false;
            SCIP_CALL(SCIPaddRow(scip, row, FALSE, &infeasible));
            // SCIP_CALL(SCIPaddPoolCut(scip, row));
            if (infeasible) {
                // std::cout << "Cutoff because of cycle!" << std::endl;
                *result = SCIP_CUTOFF;
                //                std::cout << " returning SCIP_CUTOFF" <<
                //                std::endl;
            } else {
                if (enfo) {
                    //                    std::cout << " returning
                    //                    SCIP_CONSADDED" << std::endl;
                    *result = SCIP_CONSADDED;
                } else {
                    //                    std::cout << " returning
                    //                    SCIP_SEPARATED" << std::endl;
                    *result = SCIP_SEPARATED;
                }
            }
            ++effi;
        }
        SCIP_CALL(SCIPreleaseRow(scip, &row));

        if (effi > 20000)
            break;
    }

    // DB(std::cout << "nCuts = " << nCuts << std::endl;)

    if (0 == nCuts) {
        if (enfo) {
            *result = SCIP_FEASIBLE;
            //               std::cout << " returning SCIP_FEASIBLE" <<
            //               std::endl;
        } else {
            *result = SCIP_DIDNOTFIND;
            //                std::cout << " returning SCIP_DIDNOTFIND" <<
            //                std::endl;
        }
    }

    // Can happen if all cut violation(s) are numerically unstable
    if (nCuts && one_efficacious == false) {
        if (enfo) {
            /*
            const double _eps = 1e-6;

            for (index_t i = 0 ; i < nCuts ; ++i)
            {
                for (index_t s = 0 ; s < csep->_cutlens[i] ; ++s)
                {
                    int e = csep->_cutsupport[i][s];
                    double x_e = SCIPgetSolVal(scip, sol,
            vars[data.tails()[e]]); double inf_e = (x_e < 0.5 ? x_e : 1.0 -
            x_e);

                    if (x_e > _eps && x_e < 1.0 - _eps)
                    {
                        std::cout << "Added x_" << e << " = " << x_e << " to
            candidates." << std::endl; SCIP_CALL(SCIPaddExternBranchCand(scip,
            vars[data.tails()[e]], inf_e, x_e));
                    }
                }
            }
            std::cout << " returning SCIP_INFEASIBLE" << std::endl;
            */
            *result = SCIP_INFEASIBLE;
        } else {
            *result = SCIP_DIDNOTFIND;
            // std::cout << " returning SCIP_DIDNOTFIND" << std::endl;
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

    /** destructor */
    virtual ~ConshdlrCycles() { delete _csep; }

    /****
     * FUNDAMENTAL Callbacks as of SCIP Documentation
     * CONSCHECK (global feasibility of primal solution)
     * CONSENFOLP (after price-and-cut for lp solution feasibility)
     * CONSENFOPS (similar to above CONSENFOLP but for pseudo solutions)
     * CONSENFORELAX (similar to above CONSENFOLP but for relaxation solutions)
     * CONSLOCK
     ****/

    /** feasibility check method of constraint handler for primal solutions
     *
     *  The given solution has to be checked for feasibility.
     *
     *  The check methods of the active constraint handlers are called in
     * decreasing order of their check priorities until the first constraint
     * handler returned with the result SCIP_INFEASIBLE. The integrality
     * constraint handler has a check priority of zero. A constraint handler
     * which can (or wants) to check its constraints only for integral solutions
     * should have a negative check priority (e.g. the alldiff-constraint can
     * only operate on integral solutions). A constraint handler which wants to
     * check feasibility even on non-integral solutions must have a check
     * priority greater than zero (e.g. if the check is much faster than testing
     * all variables for integrality).
     *
     *  In some cases, integrality conditions or rows of the current LP don't
     * have to be checked, because their feasibility is already checked or
     * implicitly given. In these cases, 'checkintegrality' or 'checklprows' is
     * FALSE.
     *
     *  possible return values for *result:
     *  - SCIP_INFEASIBLE : at least one constraint of the handler is infeasible
     *  - SCIP_FEASIBLE   : all constraints of the handler are feasible
     */
    virtual SCIP_DECL_CONSCHECK(scip_check)
    { /*lint --e{715}*/
        num_calls_check++;

        assert(result != nullptr);

        // std::cout << "CONSCHECK" << std::endl;

        /*
        for (index_t i  = 0 ; i < nconss; ++i)
        {
            SCIP_CONSDATA *consdata;

            assert(conss != nullptr);
            assert(conss[i] != nullptr);
            consdata = SCIPconsGetData(conss[i]);
            //assert(consdata != nullptr);
        }
        */

        double* x = new double[_data.M()];
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

    /** constraint enforcing method of constraint handler for LP solutions
     *
     *  The method is called at the end of the node processing loop for a node
     * where the LP was solved. The LP solution has to be checked for
     * feasibility. If possible, an infeasibility should be resolved by
     *  branching, reducing a variable's domain to exclude the solution or
     * separating the solution with a valid cutting plane.
     *
     *  The enforcing methods of the active constraint handlers are called in
     * decreasing order of their enforcing priorities until the first constraint
     * handler returned with the value SCIP_CUTOFF, SCIP_SEPARATED,
     *  SCIP_REDUCEDDOM, SCIP_CONSADDED, or SCIP_BRANCHED.
     *  The integrality constraint handler has an enforcing priority of zero. A
     * constraint handler which can (or wants) to enforce its constraints only
     * for integral solutions should have a negative enforcing priority (e.g.
     * the alldiff-constraint can only operate on integral solutions). A
     * constraint handler which wants to incorporate its own branching strategy
     * even on non-integral solutions must have an enforcing priority greater
     * than zero (e.g. the SOS-constraint incorporates SOS-branching on
     * non-integral solutions).
     *
     *  The first nusefulconss constraints are the ones, that are identified to
     * likely be violated. The enforcing method should process the useful
     * constraints first. The other nconss - nusefulconss constraints should
     * only be enforced, if no violation was found in the useful constraints.
     *
     *  possible return values for *result (if more than one applies, the first
     * in the list should be used):
     *  - SCIP_CUTOFF     : the node is infeasible in the variable's bounds and
     * can be cut off
     *  - SCIP_CONSADDED  : an additional constraint was generated
     *  - SCIP_REDUCEDDOM : a variable's domain was reduced
     *  - SCIP_SEPARATED  : a cutting plane was generated
     *  - SCIP_BRANCHED   : no changes were made to the problem, but a branching
     * was applied to resolve an infeasibility
     *  - SCIP_INFEASIBLE : at least one constraint is infeasible, but it was
     * not resolved
     *  - SCIP_FEASIBLE   : all constraints of the handler are feasible
     */
    virtual SCIP_DECL_CONSENFOLP(scip_enfolp)
    { /*lint --e{715}*/
        num_calls_enfolp++;
        assert(result != nullptr);

        // std::cout << "CONSENFOLP" << std::endl;

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

        /*
        for (int i = 0; i < nconss; ++i) {
            SCIP_CONSDATA *consdata;
            SCIP_Bool found;

            assert(conss != nullptr);
            assert(conss[i] != nullptr);
            consdata = SCIPconsGetData(conss[i]);
            assert(consdata != nullptr);
        }
        */

        return SCIP_OKAY;
    }

    /** constraint enforcing method of constraint handler for pseudo solutions
     *
     *  The method is called at the end of the node processing loop for a node
     * where the LP was not solved. The pseudo solution has to be checked for
     * feasibility. If possible, an infeasibility should be resolved by
     *  branching, reducing a variable's domain to exclude the solution or
     * adding an additional constraint. Separation is not possible, since the LP
     * is not processed at the current node. All LP informations like LP
     * solution, slack values, or reduced costs are invalid and must not be
     * accessed.
     *
     *  Like in the enforcing method for LP solutions, the enforcing methods of
     * the active constraint handlers are called in decreasing order of their
     * enforcing priorities until the first constraint handler returned with the
     * value SCIP_CUTOFF, SCIP_REDUCEDDOM, SCIP_CONSADDED, SCIP_BRANCHED, or
     * SCIP_SOLVELP.
     *
     *  The first nusefulconss constraints are the ones, that are identified to
     * likely be violated. The enforcing method should process the useful
     * constraints first. The other nconss - nusefulconss constraints should
     * only be enforced, if no violation was found in the useful constraints.
     *
     *  If the pseudo solution's objective value is lower than the lower bound
     * of the node, it cannot be feasible and the enforcing method may skip it's
     * check and set *result to SCIP_DIDNOTRUN. However, it can also process its
     * constraints and return any other possible result code.
     *
     *  possible return values for *result (if more than one applies, the first
     * in the list should be used):
     *  - SCIP_CUTOFF     : the node is infeasible in the variable's bounds and
     * can be cut off
     *  - SCIP_CONSADDED  : an additional constraint was generated
     *  - SCIP_REDUCEDDOM : a variable's domain was reduced
     *  - SCIP_BRANCHED   : no changes were made to the problem, but a branching
     * was applied to resolve an infeasibility
     *  - SCIP_SOLVELP    : at least one constraint is infeasible, and this can
     * only be resolved by solving the SCIP_LP
     *  - SCIP_INFEASIBLE : at least one constraint is infeasible, but it was
     * not resolved
     *  - SCIP_FEASIBLE   : all constraints of the handler are feasible
     *  - SCIP_DIDNOTRUN  : the enforcement was skipped (only possible, if
     * objinfeasible is true)
     */
    virtual SCIP_DECL_CONSENFOPS(scip_enfops)
    { /*lint --e{715}*/
        num_calls_enfops++;
        assert(result != nullptr);

        // std::cout << "CONSENFOPS" << std::endl;

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

        /*
        for (int i = 0; i < nconss; ++i) {
            SCIP_CONSDATA *consdata;
            SCIP_Bool found;

            assert(conss != nullptr);
            assert(conss[i] != nullptr);
            consdata = SCIPconsGetData(conss[i]);
            assert(consdata != nullptr);
        }
        */

        return SCIP_OKAY;
    }

    /** variable rounding lock method of constraint handler
     *
     *  This method is called, after a constraint is added or removed from the
     * transformed problem. It should update the rounding locks of all
     * associated variables with calls to SCIPaddVarLocksType(), depending on
     * the way, the variable is involved in the constraint:
     *  - If the constraint may get violated by decreasing the value of a
     * variable, it should call SCIPaddVarLocksType(scip, var,
     * SCIP_LOCKTYPE_MODEL, nlockspos, nlocksneg), saying that rounding down is
     *    potentially rendering the (positive) constraint infeasible and
     * rounding up is potentially rendering the negation of the constraint
     * infeasible.
     *  - If the constraint may get violated by increasing the value of a
     * variable, it should call SCIPaddVarLocksType(scip, var,
     * SCIP_LOCKTYPE_MODEL, nlocksneg, nlockspos), saying that rounding up is
     *    potentially rendering the constraint's negation infeasible and
     * rounding up is potentially rendering the constraint itself infeasible.
     *  - If the constraint may get violated by changing the variable in any
     * direction, it should call SCIPaddVarLocksType(scip, var,
     * SCIP_LOCKTYPE_MODEL, nlockspos + nlocksneg, nlockspos + nlocksneg).
     *
     *  Consider the linear constraint "3x -5y +2z <= 7" as an example. The
     * variable rounding lock method of the linear constraint handler should
     * call SCIPaddVarLocksType(scip, x, SCIP_LOCKTYPE_MODEL, nlocksneg,
     * nlockspos), SCIPaddVarLocksType(scip, y, SCIP_LOCKTYPE_MODEL, nlockspos,
     * nlocksneg) and SCIPaddVarLocksType(scip, z, SCIP_LOCKTYPE_MODEL,
     * nlocksneg, nlockspos) to tell SCIP, that rounding up of x and z and
     * rounding down of y can destroy the feasibility of the constraint, while
     * rounding down of x and z and rounding up of y can destroy the feasibility
     * of the constraint's negation "3x -5y +2z > 7". A linear constraint "2 <=
     * 3x -5y +2z <= 7" should call SCIPaddVarLocksType(scip, ...,
     * SCIP_LOCKTYPE_MODEL, nlockspos + nlocksneg, nlockspos + nlocksneg) on all
     * variables, since rounding in both directions of each variable can destroy
     * both the feasibility of the constraint and it's negation "3x -5y +2z < 2
     * or  3x -5y +2z > 7".
     *
     *  If the constraint itself contains other constraints as sub constraints
     * (e.g. the "or" constraint concatenation "c(x) or d(x)"), the rounding
     * lock methods of these constraints should be called in a proper way.
     *  - If the constraint may get violated by the violation of the sub
     * constraint c, it should call SCIPaddConsLocks(scip, c, nlockspos,
     * nlocksneg), saying that infeasibility of c may lead to infeasibility of
     *    the (positive) constraint, and infeasibility of c's negation (i.e.
     * feasibility of c) may lead to infeasibility of the constraint's negation
     * (i.e. feasibility of the constraint).
     *  - If the constraint may get violated by the feasibility of the sub
     * constraint c, it should call SCIPaddConsLocks(scip, c, nlocksneg,
     * nlockspos), saying that infeasibility of c may lead to infeasibility of
     *    the constraint's negation (i.e. feasibility of the constraint), and
     * infeasibility of c's negation (i.e. feasibility of c) may lead to
     * infeasibility of the (positive) constraint.
     *  - If the constraint may get violated by any change in the feasibility of
     * the sub constraint c, it should call SCIPaddConsLocks(scip, c, nlockspos
     * + nlocksneg, nlockspos + nlocksneg).
     *
     *  Consider the or concatenation "c(x) or d(x)". The variable rounding lock
     * method of the or constraint handler should call SCIPaddConsLocks(scip, c,
     * nlockspos, nlocksneg) and SCIPaddConsLocks(scip, d, nlockspos, nlocksneg)
     *  to tell SCIP, that infeasibility of c and d can lead to infeasibility of
     * "c(x) or d(x)".
     *
     *  As a second example, consider the equivalence constraint "y <-> c(x)"
     * with variable y and constraint c. The constraint demands, that y == 1 if
     * and only if c(x) is satisfied. The variable lock method of the
     * corresponding constraint handler should call SCIPaddVarLocksType(scip, y,
     * SCIP_LOCKTYPE_MODEL, nlockspos + nlocksneg, nlockspos + nlocksneg) and
     *  SCIPaddConsLocks(scip, c, nlockspos + nlocksneg, nlockspos + nlocksneg),
     * because any modification to the value of y or to the feasibility of c can
     * alter the feasibility of the equivalence constraint.
     */
    virtual SCIP_DECL_CONSLOCK(scip_lock)
    { /*lint --e{715}*/
        num_calls_lock++;

        /*
        SCIP_CONSDATA *consdata;

        consdata = SCIPconsGetData(cons);
        assert(consdata != nullptr);
        */

        for (index_t v = 0; v < _data.N(); ++v) {
            SCIPaddVarLocksType(
              scip, _vars[v], SCIP_LOCKTYPE_MODEL, nlockspos, nlocksneg);
        }

        return SCIP_OKAY;
    }

    /****
     * NON FUNDAMENTAL Callbacks as of SCIP Documentation
     ****/

    /** transforms constraint data into data belonging to the transformed
     * problem */
    // Fully virtual, therefore needs to be implemented for project to compile,
    // but docu states otherwise
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

    /** clone method which will be used to copy a objective plugin */
    virtual SCIP_DECL_CONSHDLRCLONE(
      scip::ObjProbCloneable* clone) /*lint !e665*/
    {
        assert(valid != nullptr);
        *valid = true;
        return new ConshdlrCycles(_data, scip, _vars);
    }
    /** separation method of constraint handler for arbitrary primal solution
     *
     *  Separates all constraints of the constraint handler. The method is
     * called outside the LP solution loop (e.g., by a relaxator or a primal
     * heuristic), which means that there is no valid LP solution. Instead, the
     * method should produce cuts that separate the given solution.
     *
     *  The first nusefulconss constraints are the ones, that are identified to
     * likely be violated. The separation method should process only the useful
     * constraints in most runs, and only occasionally the remaining nconss -
     * nusefulconss constraints.
     *
     *  possible return values for *result (if more than one applies, the first
     * in the list should be used):
     *  - SCIP_CUTOFF     : the node is infeasible in the variable's bounds and
     * can be cut off
     *  - SCIP_CONSADDED  : an additional constraint was generated
     *  - SCIP_REDUCEDDOM : a variable's domain was reduced
     *  - SCIP_SEPARATED  : a cutting plane was generated
     *  - SCIP_DIDNOTFIND : the separator searched, but did not find domain
     * reductions, cutting planes, or cut constraints
     *  - SCIP_DIDNOTRUN  : the separator was skipped
     *  - SCIP_DELAYED    : the separator was skipped, but should be called
     * again
     */
    virtual SCIP_DECL_CONSSEPASOL(scip_sepasol)
    {
        num_calls_sepasol++;

        // std::cout << "CONSSEPASOL" << std::endl;

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

    /** separation method of constraint handler for LP solution
     *
     *  Separates all constraints of the constraint handler. The method is
     * called in the LP solution loop, which means that a valid LP solution
     * exists.
     *
     *  The first nusefulconss constraints are the ones, that are identified to
     * likely be violated. The separation method should process only the useful
     * constraints in most runs, and only occasionally the remaining nconss -
     * nusefulconss constraints.
     *
     *  possible return values for *result (if more than one applies, the first
     * in the list should be used):
     *  - SCIP_CUTOFF     : the node is infeasible in the variable's bounds and
     * can be cut off
     *  - SCIP_CONSADDED  : an additional constraint was generated
     *  - SCIP_REDUCEDDOM : a variable's domain was reduced
     *  - SCIP_SEPARATED  : a cutting plane was generated
     *  - SCIP_DIDNOTFIND : the separator searched, but did not find domain
     * reductions, cutting planes, or cut constraints
     *  - SCIP_DIDNOTRUN  : the separator was skipped
     *  - SCIP_DELAYED    : the separator was skipped, but should be called
     * again
     */
    virtual SCIP_DECL_CONSSEPALP(scip_sepalp)
    {
        num_calls_sepalp++;

        // std::cout << "CONSSEPALP" << std::endl;

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

    /** frees specific constraint data */
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

/** creates and captures a TSP subtour constraint */
SCIP_RETCODE
SCIPcreateConsCycle(
  SCIP* scip,        /**< SCIP data structure */
  SCIP_CONS** cons,  /**< pointer to hold the created constraint */
  const char* name,  /**< name of constraint */
  SCIP_Bool initial, /**< should the LP relaxation of constraint be in the
                        initial LP? */
  SCIP_Bool
    separate, /**< should the constraint be separated during LP processing? */
  SCIP_Bool
    enforce, /**< should the constraint be enforced during node processing? */
  SCIP_Bool check,     /**< should the constraint be checked for feasibility? */
  SCIP_Bool propagate, /**< should the constraint be propagated during node
                          processing? */
  SCIP_Bool local,     /**< is constraint only valid locally? */
  SCIP_Bool
    modifiable, /**< is constraint modifiable (subject to column generation)? */
  SCIP_Bool dynamic,  /**< is constraint dynamic? */
  SCIP_Bool removable /**< should the constraint be removed from the LP due to
                         aging or cleanup? */
)
{
    SCIP_CONSHDLR* conshdlr;
    SCIP_CONSDATA* consdata;

    /* find correct constraint handler */
    conshdlr = SCIPfindConshdlr(scip, "Cycles");
    if (conshdlr == nullptr) {
        SCIPerrorMessage("constraint handler not found\n");
        return SCIP_PLUGINNOTFOUND;
    }

    /* create constraint data */
    SCIP_CALL(SCIPallocBlockMemory(scip, &consdata)); /*lint !e530*/

    /* create constraint */
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
