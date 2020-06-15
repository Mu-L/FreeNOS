/*
 * Copyright (C) 2020 Niek Linnenbank
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __SERVER_CORE_SUNXICORESERVER_H
#define __SERVER_CORE_SUNXICORESERVER_H

#include <arm/sunxi/SunxiCpuConfig.h>
#include "CoreServer.h"

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup core
 * @{
 */

/**
 * Implements a CoreServer for ARM/Allwinner (sunxi) based System-on-Chips.
 */
class SunxiCoreServer : public CoreServer
{
  public:

    /** Offset of the CoreInfo struct relative to the kernel's physical base address. */
    static const Address SecondaryCoreInfoOffset = (1024 * 1024 * 33);

    /** Physical memory address for CoreInfo passed to secondary cores during bootup. */
    static const Address SecondaryCoreInfoAddress = RAM_ADDR + SecondaryCoreInfoOffset;

  private:

    /** Software Generated Interrupt number for sending/receiving between cores. */
    static const Size SoftwareInterruptNumber = 0x1;

  public:

    /**
     * Class constructor function.
     */
    SunxiCoreServer();

    /**
     * Initialize the server
     *
     * @return Result code
     */
    virtual Result initialize();

    /**
     * Boot a processor core
     *
     * @param coreId Core identifier number
     * @param info CoreInfo pointer containing specific core information
     *
     * @return Result code
     */
    virtual Result bootCore(uint coreId, CoreInfo *info);

    /**
     * Discover processor cores
     *
     * @return Result code
     */
    virtual Result discoverCores();

    /**
     * Wait for Inter-Processor-Interrupt
     */
    virtual void waitIPI() const;

    /**
     * Send Inter-Processor-Interrupt
     *
     * @param coreId Core identifier number
     *
     * @return Result code
     */
    virtual Result sendIPI(uint coreId);

  private:

    /** CPU Configuration Module instance */
    SunxiCpuConfig m_cpuConfig;
};

/**
 * @}
 * @}
 */

#endif /* __SERVER_CORE_SUNXICORESERVER_H */
