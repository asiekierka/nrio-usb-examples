// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Adrian "asie" Siekierka, 2024

#include <nds.h>
#include "nrio_detect.h"

#define NRIO_D12_DATA (*((volatile uint16_t*) 0x9FDFFFE))
#define NRIO_D12_CMD  (*((volatile uint16_t*) 0x9FFFFFE))

#define NRIO_D12_CMD_READ_ID         0xFD

static inline uint16_t nrio_d12_read_chip_id(void) {
    uint16_t result;
    NRIO_D12_CMD = NRIO_D12_CMD_READ_ID;
    result = NRIO_D12_DATA & 0xFF;
    result |= (NRIO_D12_DATA & 0xFF) << 8;
    return result;
}

#define NRIO_D14_REG(index) GBA_BUS[(index) * 0x10000]

#define NRIO_D14_CHIP_IDL  NRIO_D14_REG(0x70)
#define NRIO_D14_CHIP_IDH  NRIO_D14_REG(0x72)

static inline uint32_t nrio_d14_read_chip_id(void) {
    return ((NRIO_D14_CHIP_IDH << 16) | NRIO_D14_CHIP_IDL) & 0xFFFFFF;
}

nrio_usb_type_t nrio_usb_detect(void) {
    nrio_usb_type_t result = {0, 0, 0};

    if (isDSiMode())
        return result;

    // Configure GBA cartridge bus
#ifdef ARM9
    sysSetCartOwner(BUS_OWNER_ARM9);
#endif
    REG_EXMEMCNT = (REG_EXMEMCNT & ~0x1F) | EXMEMCNT_ROM_TIME1_6_CYCLES
        | EXMEMCNT_ROM_TIME2_4_CYCLES | EXMEMCNT_SRAM_TIME_6_CYCLES;

    // Check if the chip is an ISP1581/82/83
    uint32_t d14_chip_id = nrio_d14_read_chip_id();
    if (d14_chip_id >= 0x158100 && d14_chip_id < 0x158400) {
        result.board_type = NRIO_USB_BOARD_TYPE_D14;
        result.chip_id = d14_chip_id >> 8;
        result.chip_rev = d14_chip_id & 0xFF;
        return result;
    }

    // The D12 supports a maximum readout speed of 2 MHz.
    REG_EXMEMCNT = (REG_EXMEMCNT & ~0x1F) | EXMEMCNT_ROM_TIME1_18_CYCLES
        | EXMEMCNT_ROM_TIME2_6_CYCLES | EXMEMCNT_SRAM_TIME_18_CYCLES;

    // Check if the chip is an PDIUSBD12 (do other compatible chips exist?)
    uint16_t d12_chip_id = nrio_d12_read_chip_id();
    if (d12_chip_id != 0x0000 && d12_chip_id != 0xFFFF) {
        result.board_type = NRIO_USB_BOARD_TYPE_D12;
        result.chip_id = d12_chip_id;
        return result;
    }

    return result;
}
