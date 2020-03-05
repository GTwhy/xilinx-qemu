/*
 * QEMU model of the XMPU
 *
 * Copyright (c) 2014 - 2019 Xilinx Inc.
 *
 * Partially autogenerated by xregqemu.py 2019-12-09.
 * Written by Edgar E. Iglesias <edgari@xilinx.com>
 *            Alistair Francis <alistai@xilinx.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "hw/register.h"
#include "qemu/bitops.h"
#include "qapi/error.h"
#include "qemu/log.h"
#include "migration/vmstate.h"
#include "hw/qdev-properties.h"
#include "sysemu/dma.h"
#include "exec/address-spaces.h"

#include "hw/fdt_generic_util.h"
#include "hw/misc/xlnx-xmpu.h"

#ifndef XILINX_XMPU_ERR_DEBUG
#define XILINX_XMPU_ERR_DEBUG 0
#endif

#define TYPE_XILINX_XMPU "xlnx.xmpu"
#define TYPE_XILINX_XMPU_IOMMU_MEMORY_REGION "xlnx.xmpu-iommu-memory-region"

#define XILINX_XMPU(obj) \
     OBJECT_CHECK(XMPU, (obj), TYPE_XILINX_XMPU)

/*
 * Register definitions shared between ZynqMP and Versal are in
 * the XMPU header file
 */
    FIELD(CTRL, POISONCFG, 2, 1)
REG32(ERR_STATUS1, 0x4)
    FIELD(ERR_STATUS1, AXI_ADDR, 0, 28)
REG32(ERR_STATUS2, 0x8)
    FIELD(ERR_STATUS2, AXI_ID, 0, 10)
REG32(POISON, 0xc)
    FIELD(POISON, ATTRIB, 20, 1)
    FIELD(POISON, BASE, 0, 20)
REG32(R00_START, 0x100)
    FIELD(R00_START, ADDR, 0, 28)
REG32(R00_END, 0x104)
    FIELD(R00_END, ADDR, 0, 28)
REG32(R00_MASTER, 0x108)
    FIELD(R00_MASTER, MASK, 16, 10)
    FIELD(R00_MASTER, ID, 0, 10)
REG32(R00_CONFIG, 0x10c)
    FIELD(R00_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R00_CONFIG, REGIONNS, 3, 1)
    FIELD(R00_CONFIG, WRALLOWED, 2, 1)
    FIELD(R00_CONFIG, RDALLOWED, 1, 1)
    FIELD(R00_CONFIG, ENABLE, 0, 1)
REG32(R01_START, 0x110)
    FIELD(R01_START, ADDR, 0, 28)
REG32(R01_END, 0x114)
    FIELD(R01_END, ADDR, 0, 28)
REG32(R01_MASTER, 0x118)
    FIELD(R01_MASTER, MASK, 16, 10)
    FIELD(R01_MASTER, ID, 0, 10)
REG32(R01_CONFIG, 0x11c)
    FIELD(R01_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R01_CONFIG, REGIONNS, 3, 1)
    FIELD(R01_CONFIG, WRALLOWED, 2, 1)
    FIELD(R01_CONFIG, RDALLOWED, 1, 1)
    FIELD(R01_CONFIG, ENABLE, 0, 1)
REG32(R02_START, 0x120)
    FIELD(R02_START, ADDR, 0, 28)
REG32(R02_END, 0x124)
    FIELD(R02_END, ADDR, 0, 28)
REG32(R02_MASTER, 0x128)
    FIELD(R02_MASTER, MASK, 16, 10)
    FIELD(R02_MASTER, ID, 0, 10)
REG32(R02_CONFIG, 0x12c)
    FIELD(R02_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R02_CONFIG, REGIONNS, 3, 1)
    FIELD(R02_CONFIG, WRALLOWED, 2, 1)
    FIELD(R02_CONFIG, RDALLOWED, 1, 1)
    FIELD(R02_CONFIG, ENABLE, 0, 1)
REG32(R03_START, 0x130)
    FIELD(R03_START, ADDR, 0, 28)
REG32(R03_END, 0x134)
    FIELD(R03_END, ADDR, 0, 28)
REG32(R03_MASTER, 0x138)
    FIELD(R03_MASTER, MASK, 16, 10)
    FIELD(R03_MASTER, ID, 0, 10)
REG32(R03_CONFIG, 0x13c)
    FIELD(R03_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R03_CONFIG, REGIONNS, 3, 1)
    FIELD(R03_CONFIG, WRALLOWED, 2, 1)
    FIELD(R03_CONFIG, RDALLOWED, 1, 1)
    FIELD(R03_CONFIG, ENABLE, 0, 1)
REG32(R04_START, 0x140)
    FIELD(R04_START, ADDR, 0, 28)
REG32(R04_END, 0x144)
    FIELD(R04_END, ADDR, 0, 28)
REG32(R04_MASTER, 0x148)
    FIELD(R04_MASTER, MASK, 16, 10)
    FIELD(R04_MASTER, ID, 0, 10)
REG32(R04_CONFIG, 0x14c)
    FIELD(R04_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R04_CONFIG, REGIONNS, 3, 1)
    FIELD(R04_CONFIG, WRALLOWED, 2, 1)
    FIELD(R04_CONFIG, RDALLOWED, 1, 1)
    FIELD(R04_CONFIG, ENABLE, 0, 1)
REG32(R05_START, 0x150)
    FIELD(R05_START, ADDR, 0, 28)
REG32(R05_END, 0x154)
    FIELD(R05_END, ADDR, 0, 28)
REG32(R05_MASTER, 0x158)
    FIELD(R05_MASTER, MASK, 16, 10)
    FIELD(R05_MASTER, ID, 0, 10)
REG32(R05_CONFIG, 0x15c)
    FIELD(R05_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R05_CONFIG, REGIONNS, 3, 1)
    FIELD(R05_CONFIG, WRALLOWED, 2, 1)
    FIELD(R05_CONFIG, RDALLOWED, 1, 1)
    FIELD(R05_CONFIG, ENABLE, 0, 1)
REG32(R06_START, 0x160)
    FIELD(R06_START, ADDR, 0, 28)
REG32(R06_END, 0x164)
    FIELD(R06_END, ADDR, 0, 28)
REG32(R06_MASTER, 0x168)
    FIELD(R06_MASTER, MASK, 16, 10)
    FIELD(R06_MASTER, ID, 0, 10)
REG32(R06_CONFIG, 0x16c)
    FIELD(R06_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R06_CONFIG, REGIONNS, 3, 1)
    FIELD(R06_CONFIG, WRALLOWED, 2, 1)
    FIELD(R06_CONFIG, RDALLOWED, 1, 1)
    FIELD(R06_CONFIG, ENABLE, 0, 1)
REG32(R07_START, 0x170)
    FIELD(R07_START, ADDR, 0, 28)
REG32(R07_END, 0x174)
    FIELD(R07_END, ADDR, 0, 28)
REG32(R07_MASTER, 0x178)
    FIELD(R07_MASTER, MASK, 16, 10)
    FIELD(R07_MASTER, ID, 0, 10)
REG32(R07_CONFIG, 0x17c)
    FIELD(R07_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R07_CONFIG, REGIONNS, 3, 1)
    FIELD(R07_CONFIG, WRALLOWED, 2, 1)
    FIELD(R07_CONFIG, RDALLOWED, 1, 1)
    FIELD(R07_CONFIG, ENABLE, 0, 1)
REG32(R08_START, 0x180)
    FIELD(R08_START, ADDR, 0, 28)
REG32(R08_END, 0x184)
    FIELD(R08_END, ADDR, 0, 28)
REG32(R08_MASTER, 0x188)
    FIELD(R08_MASTER, MASK, 16, 10)
    FIELD(R08_MASTER, ID, 0, 10)
REG32(R08_CONFIG, 0x18c)
    FIELD(R08_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R08_CONFIG, REGIONNS, 3, 1)
    FIELD(R08_CONFIG, WRALLOWED, 2, 1)
    FIELD(R08_CONFIG, RDALLOWED, 1, 1)
    FIELD(R08_CONFIG, ENABLE, 0, 1)
REG32(R09_START, 0x190)
    FIELD(R09_START, ADDR, 0, 28)
REG32(R09_END, 0x194)
    FIELD(R09_END, ADDR, 0, 28)
REG32(R09_MASTER, 0x198)
    FIELD(R09_MASTER, MASK, 16, 10)
    FIELD(R09_MASTER, ID, 0, 10)
REG32(R09_CONFIG, 0x19c)
    FIELD(R09_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R09_CONFIG, REGIONNS, 3, 1)
    FIELD(R09_CONFIG, WRALLOWED, 2, 1)
    FIELD(R09_CONFIG, RDALLOWED, 1, 1)
    FIELD(R09_CONFIG, ENABLE, 0, 1)
REG32(R10_START, 0x1a0)
    FIELD(R10_START, ADDR, 0, 28)
REG32(R10_END, 0x1a4)
    FIELD(R10_END, ADDR, 0, 28)
REG32(R10_MASTER, 0x1a8)
    FIELD(R10_MASTER, MASK, 16, 10)
    FIELD(R10_MASTER, ID, 0, 10)
REG32(R10_CONFIG, 0x1ac)
    FIELD(R10_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R10_CONFIG, REGIONNS, 3, 1)
    FIELD(R10_CONFIG, WRALLOWED, 2, 1)
    FIELD(R10_CONFIG, RDALLOWED, 1, 1)
    FIELD(R10_CONFIG, ENABLE, 0, 1)
REG32(R11_START, 0x1b0)
    FIELD(R11_START, ADDR, 0, 28)
REG32(R11_END, 0x1b4)
    FIELD(R11_END, ADDR, 0, 28)
REG32(R11_MASTER, 0x1b8)
    FIELD(R11_MASTER, MASK, 16, 10)
    FIELD(R11_MASTER, ID, 0, 10)
REG32(R11_CONFIG, 0x1bc)
    FIELD(R11_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R11_CONFIG, REGIONNS, 3, 1)
    FIELD(R11_CONFIG, WRALLOWED, 2, 1)
    FIELD(R11_CONFIG, RDALLOWED, 1, 1)
    FIELD(R11_CONFIG, ENABLE, 0, 1)
REG32(R12_START, 0x1c0)
    FIELD(R12_START, ADDR, 0, 28)
REG32(R12_END, 0x1c4)
    FIELD(R12_END, ADDR, 0, 28)
REG32(R12_MASTER, 0x1c8)
    FIELD(R12_MASTER, MASK, 16, 10)
    FIELD(R12_MASTER, ID, 0, 10)
REG32(R12_CONFIG, 0x1cc)
    FIELD(R12_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R12_CONFIG, REGIONNS, 3, 1)
    FIELD(R12_CONFIG, WRALLOWED, 2, 1)
    FIELD(R12_CONFIG, RDALLOWED, 1, 1)
    FIELD(R12_CONFIG, ENABLE, 0, 1)
REG32(R13_START, 0x1d0)
    FIELD(R13_START, ADDR, 0, 28)
REG32(R13_END, 0x1d4)
    FIELD(R13_END, ADDR, 0, 28)
REG32(R13_MASTER, 0x1d8)
    FIELD(R13_MASTER, MASK, 16, 10)
    FIELD(R13_MASTER, ID, 0, 10)
REG32(R13_CONFIG, 0x1dc)
    FIELD(R13_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R13_CONFIG, REGIONNS, 3, 1)
    FIELD(R13_CONFIG, WRALLOWED, 2, 1)
    FIELD(R13_CONFIG, RDALLOWED, 1, 1)
    FIELD(R13_CONFIG, ENABLE, 0, 1)
REG32(R14_START, 0x1e0)
    FIELD(R14_START, ADDR, 0, 28)
REG32(R14_END, 0x1e4)
    FIELD(R14_END, ADDR, 0, 28)
REG32(R14_MASTER, 0x1e8)
    FIELD(R14_MASTER, MASK, 16, 10)
    FIELD(R14_MASTER, ID, 0, 10)
REG32(R14_CONFIG, 0x1ec)
    FIELD(R14_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R14_CONFIG, REGIONNS, 3, 1)
    FIELD(R14_CONFIG, WRALLOWED, 2, 1)
    FIELD(R14_CONFIG, RDALLOWED, 1, 1)
    FIELD(R14_CONFIG, ENABLE, 0, 1)
REG32(R15_START, 0x1f0)
    FIELD(R15_START, ADDR, 0, 28)
REG32(R15_END, 0x1f4)
    FIELD(R15_END, ADDR, 0, 28)
REG32(R15_MASTER, 0x1f8)
    FIELD(R15_MASTER, MASK, 16, 10)
    FIELD(R15_MASTER, ID, 0, 10)
REG32(R15_CONFIG, 0x1fc)
    FIELD(R15_CONFIG, NSCHECKTYPE, 4, 1)
    FIELD(R15_CONFIG, REGIONNS, 3, 1)
    FIELD(R15_CONFIG, WRALLOWED, 2, 1)
    FIELD(R15_CONFIG, RDALLOWED, 1, 1)
    FIELD(R15_CONFIG, ENABLE, 0, 1)

#define XMPU_R_MAX (R_R15_CONFIG + 1)

static void xmpu_decode_region(XMPU *s, XMPURegion *xr, unsigned int region)
{
    assert(region < NR_XMPU_REGIONS);
    uint32_t config;
    unsigned int offset = region * (R_R01_START - R_R00_START);

    xr->start = s->regs[offset + R_R00_START];
    xr->end = s->regs[offset + R_R00_END];
    xr->start <<= 12;
    xr->end <<= 12;

    xr->size = xr->end - xr->start;
    xr->master.u32 = s->regs[offset + R_R00_MASTER];

    config = s->regs[offset + R_R00_CONFIG];
    xr->config.enable = FIELD_EX32(config, R00_CONFIG, ENABLE);
    xr->config.rdallowed = FIELD_EX32(config, R00_CONFIG, RDALLOWED);
    xr->config.wrallowed = FIELD_EX32(config, R00_CONFIG, WRALLOWED);
    xr->config.regionns = FIELD_EX32(config, R00_CONFIG, REGIONNS);
    xr->config.nschecktype = FIELD_EX32(config, R00_CONFIG, NSCHECKTYPE);
}

static void isr_update_irq(XMPU *s)
{
    bool pending = s->regs[R_ISR] & ~s->regs[R_IMR];
    qemu_set_irq(s->irq_isr, pending);
}

static void isr_postw(RegisterInfo *reg, uint64_t val64)
{
    XMPU *s = XILINX_XMPU(reg->opaque);
    isr_update_irq(s);
}

static uint64_t ien_prew(RegisterInfo *reg, uint64_t val64)
{
    XMPU *s = XILINX_XMPU(reg->opaque);
    uint32_t val = val64;

    s->regs[R_IMR] &= ~val;
    isr_update_irq(s);
    return 0;
}

static uint64_t ids_prew(RegisterInfo *reg, uint64_t val64)
{
    XMPU *s = XILINX_XMPU(reg->opaque);
    uint32_t val = val64;

    s->regs[R_IMR] |= val;
    isr_update_irq(s);
    return 0;
}

static void xmpu_setup_postw(RegisterInfo *reg, uint64_t val64)
{
    XMPU *s = XILINX_XMPU(reg->opaque);
    xmpu_flush(s);
}

static const RegisterAccessInfo xmpu_ddr_regs_info[] = {
    {   .name = "CTRL",  .addr = A_CTRL,
        .reset = 0xb,
        .rsvd = 0xfffffff0,
        .ro = 0xfffffff8,
        .post_write = xmpu_setup_postw,
    },{ .name = "ERR_STATUS1",  .addr = A_ERR_STATUS1,
        .rsvd = 0xf0000000,
        .ro = 0xffffffff,
    },{ .name = "ERR_STATUS2",  .addr = A_ERR_STATUS2,
        .rsvd = 0xfffffc00,
        .ro = 0xffffffff,
    },{ .name = "POISON",  .addr = A_POISON,
        .rsvd = 0xffe00000,
        .ro = 0xffefffff,
        .post_write = xmpu_setup_postw,
    },{ .name = "ISR",  .addr = A_ISR,
        .rsvd = 0xfffffff0,
        .ro = 0xfffffff0,
        .w1c = 0xf,
        .post_write = isr_postw,
    },{ .name = "IMR",  .addr = A_IMR,
        .reset = 0xf,
        .rsvd = 0xfffffff0,
        .ro = 0xffffffff,
    },{ .name = "IEN",  .addr = A_IEN,
        .rsvd = 0xfffffff0,
        .ro = 0xfffffff0,
        .pre_write = ien_prew,
    },{ .name = "IDS",  .addr = A_IDS,
        .rsvd = 0xfffffff0,
        .ro = 0xfffffff0,
        .pre_write = ids_prew,
    },{ .name = "LOCK",  .addr = A_LOCK,
    },{ .name = "R00_START",  .addr = A_R00_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R00_END",  .addr = A_R00_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R00_MASTER",  .addr = A_R00_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R00_CONFIG",  .addr = A_R00_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R01_START",  .addr = A_R01_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R01_END",  .addr = A_R01_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R01_MASTER",  .addr = A_R01_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R01_CONFIG",  .addr = A_R01_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R02_START",  .addr = A_R02_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R02_END",  .addr = A_R02_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R02_MASTER",  .addr = A_R02_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R02_CONFIG",  .addr = A_R02_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R03_START",  .addr = A_R03_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R03_END",  .addr = A_R03_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R03_MASTER",  .addr = A_R03_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R03_CONFIG",  .addr = A_R03_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R04_START",  .addr = A_R04_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R04_END",  .addr = A_R04_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R04_MASTER",  .addr = A_R04_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R04_CONFIG",  .addr = A_R04_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R05_START",  .addr = A_R05_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R05_END",  .addr = A_R05_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R05_MASTER",  .addr = A_R05_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R05_CONFIG",  .addr = A_R05_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R06_START",  .addr = A_R06_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R06_END",  .addr = A_R06_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R06_MASTER",  .addr = A_R06_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R06_CONFIG",  .addr = A_R06_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R07_START",  .addr = A_R07_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R07_END",  .addr = A_R07_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R07_MASTER",  .addr = A_R07_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R07_CONFIG",  .addr = A_R07_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R08_START",  .addr = A_R08_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R08_END",  .addr = A_R08_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R08_MASTER",  .addr = A_R08_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R08_CONFIG",  .addr = A_R08_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R09_START",  .addr = A_R09_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R09_END",  .addr = A_R09_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R09_MASTER",  .addr = A_R09_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R09_CONFIG",  .addr = A_R09_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R10_START",  .addr = A_R10_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R10_END",  .addr = A_R10_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R10_MASTER",  .addr = A_R10_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R10_CONFIG",  .addr = A_R10_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R11_START",  .addr = A_R11_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R11_END",  .addr = A_R11_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R11_MASTER",  .addr = A_R11_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R11_CONFIG",  .addr = A_R11_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R12_START",  .addr = A_R12_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R12_END",  .addr = A_R12_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R12_MASTER",  .addr = A_R12_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R12_CONFIG",  .addr = A_R12_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R13_START",  .addr = A_R13_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R13_END",  .addr = A_R13_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R13_MASTER",  .addr = A_R13_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R13_CONFIG",  .addr = A_R13_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R14_START",  .addr = A_R14_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R14_END",  .addr = A_R14_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R14_MASTER",  .addr = A_R14_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R14_CONFIG",  .addr = A_R14_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    },{ .name = "R15_START",  .addr = A_R15_START,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R15_END",  .addr = A_R15_END,
        .rsvd = 0xf0000000,
        .ro = 0xf0000000,
    },{ .name = "R15_MASTER",  .addr = A_R15_MASTER,
        .rsvd = 0xfc00fc00,
        .ro = 0xfc00fc00,
    },{ .name = "R15_CONFIG",  .addr = A_R15_CONFIG,
        .reset = 0x8,
        .rsvd = 0xffffffe0,
        .ro = 0xffffffe0,
    }
};

static void xmpu_reset(DeviceState *dev)
{
    XMPU *s = XILINX_XMPU(dev);
    unsigned int i;

    for (i = 0; i < ARRAY_SIZE(s->regs_info); ++i) {
        register_reset(&s->regs_info[i]);
    }

    ARRAY_FIELD_DP32(s->regs, CTRL, ALIGNCFG, s->cfg.align);
    isr_update_irq(s);
    xmpu_flush(s);
}

static XMPU *xmpu_from_mr(void *mr_accessor)
{
    RegisterInfoArray *reg_array = mr_accessor;
    Object *obj;

    assert(reg_array != NULL);

    obj = reg_array->mem.owner;
    assert(obj);

    return XILINX_XMPU(obj);
}

static MemTxResult xmpu_read(void *opaque, hwaddr addr, uint64_t *value,
                             unsigned size, MemTxAttrs attr)
{
    XMPU *s = xmpu_from_mr(opaque);

    if (!attr.secure) {
        /* Non secure, return zero */
        *value = 0;
        return MEMTX_ERROR;
    }

    return xmpu_read_common(opaque, s, addr, value, size, attr);
}

static MemTxResult xmpu_write(void *opaque, hwaddr addr, uint64_t value,
                              unsigned size, MemTxAttrs attr)
{
    XMPU *s = xmpu_from_mr(opaque);
    MemTxResult res;
    bool locked;

    locked = ARRAY_FIELD_EX32(s->regs, LOCK, REGWRDIS);
    if (locked && (addr < A_ISR || addr >= A_LOCK)) {
        /* Locked access.  */
        qemu_log_mask(LOG_GUEST_ERROR, "%s: accessing locked register "\
                      "0x%"HWADDR_PRIx"\n",
                      object_get_canonical_path(OBJECT(s)), addr);
        return MEMTX_ERROR;
    }

    res = xmpu_write_common(opaque, s, addr, value, size, attr);

    if (addr > R_R00_MASTER) {
        xmpu_flush(s);
    }

    return res;
}

static const MemoryRegionOps xmpu_ops = {
    .read_with_attrs = xmpu_read,
    .write_with_attrs = xmpu_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = {
        .min_access_size = 1,
        .max_access_size = 8,
    },
};

static MemTxResult zero_read(void *opaque, hwaddr addr, uint64_t *pdata,
                             unsigned size, MemTxAttrs attr)
{
    XMPUMaster *xm = opaque;
    XMPU *s = xm->parent;
    bool poisoncfg = ARRAY_FIELD_EX32(s->regs, CTRL, POISONCFG);
    uint64_t value = 0;
    bool sec_vio;
    IOMMUTLBEntry ret;
    int perm;

    ret = xmpu_master_translate(xm, addr, attr.secure, attr.requester_id,
                                &sec_vio, &perm);
    ret.perm = perm;

    if (ret.perm & IOMMU_RO) {
        dma_memory_read(&xm->down.rw.as, addr, &value, size);
    } else {
        if (!(s->regs[R_ISR] & (7 << 1))) {
            s->regs[R_ERR_STATUS1] = (addr + s->cfg.base) >> 12;
        }
        if (poisoncfg) {
            AddressSpace *as = xm->parent_as;
            addr = (ARRAY_FIELD_EX32(s->regs, POISON, BASE) << 12) |
                    (addr & 0xfff);
            dma_memory_read(as, addr, &value, size);
        }
        ARRAY_FIELD_DP32(s->regs, ERR_STATUS2, AXI_ID, attr.requester_id);
        if (sec_vio) {
            ARRAY_FIELD_DP32(s->regs, ISR, SECURITYVIO, true);
        } else {
            ARRAY_FIELD_DP32(s->regs, ISR, RDPERMVIO, true);
        }
        isr_update_irq(s);
    }

    *pdata = value;
    return MEMTX_OK;
}

static MemTxResult zero_write(void *opaque, hwaddr addr, uint64_t value,
                              unsigned size, MemTxAttrs attr)
{
    XMPUMaster *xm = opaque;
    XMPU *s = xm->parent;
    bool poisoncfg = ARRAY_FIELD_EX32(s->regs, CTRL, POISONCFG);
    bool sec_vio;
    IOMMUTLBEntry ret;
    int perm;

    ret = xmpu_master_translate(xm, addr, attr.secure, attr.requester_id,
                                &sec_vio, &perm);
    ret.perm = perm;

    if (ret.perm & IOMMU_WO) {
        dma_memory_write(&xm->down.rw.as, addr, &value, size);
    } else {
        if (!(s->regs[R_ISR] & (7 << 1))) {
            s->regs[R_ERR_STATUS1] = (addr + s->cfg.base) >> 12;
        }
        if (poisoncfg) {
            AddressSpace *as = xm->parent_as;
            addr = (ARRAY_FIELD_EX32(s->regs, POISON, BASE) << 12) |
                   (addr & 0xfff);
            dma_memory_write(as, addr, &value, size);
        }
        ARRAY_FIELD_DP32(s->regs, ERR_STATUS2, AXI_ID, attr.requester_id);
        if (sec_vio) {
            ARRAY_FIELD_DP32(s->regs, ISR, SECURITYVIO, true);
        } else {
            ARRAY_FIELD_DP32(s->regs, ISR, WRPERMVIO, true);
        }
        isr_update_irq(s);
    }

    return MEMTX_OK;
}

static const MemoryRegionOps zero_ops = {
    .read_with_attrs = zero_read,
    .write_with_attrs = zero_write,
    .endianness = DEVICE_LITTLE_ENDIAN,
    .valid = {
        .min_access_size = 1,
        .max_access_size = 8,
    },
    . impl = {
        .min_access_size = 1,
        .max_access_size = 8,
    }
};

static void xmpu_realize(DeviceState *dev, Error **errp)
{
    XMPU *s = XILINX_XMPU(dev);

    s->prefix = object_get_canonical_path(OBJECT(dev));
    s->addr_shift = s->cfg.align ? 20 : 12;
    s->addr_mask = (1ULL << s->addr_shift) - 1;
    s->decode_region = xmpu_decode_region;
    s->masters[0].parent = s;
}

static void xmpu_init(Object *obj)
{
    XMPU *s = XILINX_XMPU(obj);
    s->regs_size = XMPU_R_MAX;
    xmpu_init_common(s, obj, TYPE_XILINX_XMPU, &xmpu_ops, xmpu_ddr_regs_info,
                     ARRAY_SIZE(xmpu_ddr_regs_info));
}

static bool xmpu_parse_reg(FDTGenericMMap *obj, FDTGenericRegPropInfo reg,
                           Error **errp)
{
    XMPU *s = XILINX_XMPU(obj);

    return xmpu_parse_reg_common(s, TYPE_XILINX_XMPU,
                                 TYPE_XILINX_XMPU_IOMMU_MEMORY_REGION,
                                 &zero_ops, reg, obj, errp);
}

static Property xmpu_properties[] = {
    DEFINE_PROP_BOOL("align", XMPU, cfg.align, 0),
    DEFINE_PROP_BOOL("poison", XMPU, cfg.poison, 0),
    DEFINE_PROP_UINT64("protected-base", XMPU, cfg.base, 0),
    DEFINE_PROP_END_OF_LIST(),
};

static const VMStateDescription vmstate_xmpu = {
    .name = TYPE_XILINX_XMPU,
    .version_id = 1,
    .minimum_version_id = 1,
    .fields = (VMStateField[]) {
        VMSTATE_UINT32_ARRAY(regs, XMPU, XMPU_VERSAL_R_MAX),
        VMSTATE_END_OF_LIST(),
    }
};

static void xmpu_class_init(ObjectClass *klass, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    FDTGenericMMapClass *fmc = FDT_GENERIC_MMAP_CLASS(klass);

    dc->reset = xmpu_reset;
    dc->realize = xmpu_realize;
    dc->vmsd = &vmstate_xmpu;
    dc->props = xmpu_properties;
    fmc->parse_reg = xmpu_parse_reg;
}

static void xmpu_iommu_memory_region_class_init(ObjectClass *klass, void *data)
{
    IOMMUMemoryRegionClass *imrc = IOMMU_MEMORY_REGION_CLASS(klass);

    imrc->translate = xmpu_translate;
    imrc->attrs_to_index = xmpu_attrs_to_index;
    imrc->num_indexes = xmpu_num_indexes;
}

static const TypeInfo xmpu_info = {
    .name          = TYPE_XILINX_XMPU,
    .parent        = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(XMPU),
    .class_init    = xmpu_class_init,
    .instance_init = xmpu_init,
    .interfaces    = (InterfaceInfo[]) {
        { TYPE_FDT_GENERIC_MMAP },
        { },
    },
};

static const TypeInfo xmpu_iommu_memory_region_info = {
    .name = TYPE_XILINX_XMPU_IOMMU_MEMORY_REGION,
    .parent = TYPE_IOMMU_MEMORY_REGION,
    .class_init = xmpu_iommu_memory_region_class_init,
};

static void xmpu_register_types(void)
{
    type_register_static(&xmpu_info);
    type_register_static(&xmpu_iommu_memory_region_info);
}

type_init(xmpu_register_types)