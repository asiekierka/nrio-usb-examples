// SPDX-License-Identifier: CC0-1.0
//
// SPDX-FileContributor: Adrian "asie" Siekierka, 2024

#include <stdio.h>
#include <nds.h>
#include "nrio_detect.h"

int main(int argc, char **argv)
{
    consoleDemoInit();

    nrio_usb_type_t usb = nrio_usb_detect();

    printf("Board type: %d\n", usb.board_type);
    printf("Chip ID: %02X\n", usb.chip_id);
    printf("Chip revision: %02X\n", usb.chip_rev);

    while (1)
    {
        swiWaitForVBlank();

        scanKeys();
        uint16_t keys = keysDown();
        if (keys & KEY_START)
            break;
    }

    return 0;
}
