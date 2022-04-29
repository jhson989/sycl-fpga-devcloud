#!/bin/bash
source /opt/intel/inteloneapi/setvars.sh --force
make clean
make DEVICE=EMU

