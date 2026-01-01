/*****************************************************************************
 * Copyright (c) 2021 Rowley Associates Limited.                             *
 *                                                                           *
 * This file may be distributed under the terms of the License Agreement     *
 * provided with this software.                                              *
 *                                                                           *
 * THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, INCLUDING THE   *
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. *
 *                                                                           *
 *****************************************************************************/

/*****************************************************************************
 *                                                                           *
 * This file includes the stage 2 bootloader selected using the "Build >     *
 * Second Stage Bootloader" project property.                                *
 *                                                                           *
 *****************************************************************************/

#if !defined(RAM_PLACEMENT)

#if defined(BOOT_STAGE2_ADESTO_AT25SF128A)

#include "boot2_at25sf128a.S"

#elif defined(BOOT_STAGE2_GENERIC_03)

#include "boot2_generic_03h.S"

#elif defined(BOOT_STAGE2_ISSI_IS25LP080D)

#include "boot2_is25lp080.S"

#elif defined(BOOT_STAGE2_WINBOND_W25Q080)

#include "boot2_w25q080.S"

#elif defined(BOOT_STAGE2_WINBOND_W25X10CL)

#include "boot2_w25x10cl.S"

#endif

#endif
