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

/**@file   nodesel_restartdfs.h
 * @ingroup NODESELECTORS
 * @brief  node selector for depth first search with periodical selection of the best node
 * @author Tobias Achterberg
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#ifndef __SCIP_NODESEL_RESTARTDFS_H__
#define __SCIP_NODESEL_RESTARTDFS_H__




#ifdef __cplusplus
extern "C" {
#endif

/** creates the node selector for restarting depth first search and includes it in SCIP
 *
 *  @ingroup NodeSelectorIncludes
 */
EXTERN
SCIP_RETCODE SCIPincludeNodeselRestartdfs(
   SCIP*                 scip                /**< SCIP data structure */
   );

#ifdef __cplusplus
}
#endif

#endif
