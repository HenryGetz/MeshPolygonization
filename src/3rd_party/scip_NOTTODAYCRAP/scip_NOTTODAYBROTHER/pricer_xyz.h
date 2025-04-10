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

/**@file   pricer_xyz.h
 * @ingroup PRICERS
 * @brief  xyz variable pricer
 * @author Tobias Achterberg
 */

/*---+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9----+----0----+----1----+----2*/

#ifndef __SCIP_PRICER_XYZ_H__
#define __SCIP_PRICER_XYZ_H__




#ifdef __cplusplus
extern "C" {
#endif

/** creates the xyz variable pricer and includes it in SCIP
 *
 *  @ingroup PricerIncludes
 */
EXTERN
SCIP_RETCODE SCIPincludePricerXyz(
   SCIP*                 scip                /**< SCIP data structure */
   );

/**@addtogroup PRICERS
 *
 * @{
 */

/** TODO: add public methods to this group for documentation purposes

/* @} */

#ifdef __cplusplus
}
#endif

#endif
