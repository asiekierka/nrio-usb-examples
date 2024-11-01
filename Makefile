# SPDX-License-Identifier: CC0-1.0
#
# SPDX-FileContributor: Adrian "asie" Siekierka, 2024

export WONDERFUL_TOOLCHAIN ?= /opt/wonderful
export BLOCKSDS ?= /opt/blocksds/core

# Verbose flag
# ------------

ifeq ($(V),1)
_V		:=
else
_V		:= @
endif

.PHONY: all clean

all:
	$(_V)make -f Makefile.project PROJECT=examples/cdc_dual_ports
	$(_V)make -f Makefile.project PROJECT=examples/cdc_msc
	$(_V)make -f Makefile.project PROJECT=examples/msc_dual_lun

clean:
	$(_V)make -f Makefile.project PROJECT=examples/cdc_dual_ports clean
	$(_V)make -f Makefile.project PROJECT=examples/cdc_msc clean
	$(_V)make -f Makefile.project PROJECT=examples/msc_dual_lun clean
