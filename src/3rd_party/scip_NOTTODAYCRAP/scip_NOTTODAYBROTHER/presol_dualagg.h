/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*                                                                           */
/*                  This file is part of the program and library             */
/*         SCIP --- Solving Constraint Integer Programs                      */
/*                                                                           */
/*    Copyright (C) 2002-2018 Konrad-Zuse-Zentrum                            */
/*                            fuer Informationstechnik Berlin                */
/*                                                                           */
/*  SCIP is distributed under the terms of the ZIB Academic License.         */
/*                                                                           */
/*  You should have received a copy of the ZIB Academic License              */
/*  along with SCIP; see the file COPYING. If not email to scip@zib.de.      */
/*                                                                           */
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/**@file   presol_dualagg.h
 * @ingroup PRESOLVERS
 * @brief  aggregate variables by dual arguments
 * @author Dieter Weninger
 *
 * This presolver looks for variables which could not be handled by
 * duality fixing because of one up-/downlock.
 * If the constraint which delivers the up-/downlock has
 * a specific structure, we can aggregate the corresponding variable.
 *
 * In more detail (for a minimization problem and the case of only one uplock):
 *
 * Given a variable \f$x_i\f$ with \f$c_i \leq 0\f$ and only one up lock (originating from a constraint c),
 * we are looking for a binary variable \f$x_j\f$ such that:
 * 1. if \f$x_j = 0\f$, constraint c can only be fulfilled for \f$x_i = lb_i\f$, and
 * 2. if \f$x_j = 1\f$, constraint c becomes redundant and \f$x_i\f$ can be dual-fixed to its upper bound \f$ub_i\f$
 * (or vice versa). Then we can perform the following aggregation: \f$x_i = lb_i + x_j (ub_i - lb_i)\f$.
 *
 * Similar arguments apply for the case of only one down lock and \f$c_i \geq 0\f$.
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#ifndef __SCIP_PRESOL_DUALAGG_H__
#define __SCIP_PRESOL_DUALAGG_H__




#ifdef __cplusplus
extern "C" {
#endif

/** creates the dualagg presolver and includes it in SCIP
 *
 * @ingroup PresolverIncludes
 */
EXTERN
SCIP_RETCODE SCIPincludePresolDualagg(
   SCIP*                 scip                /**< SCIP data structure */
   );

#ifdef __cplusplus
}
#endif

#endif
