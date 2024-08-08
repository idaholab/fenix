#!/usr/bin/env bash
#* This file is part of FENIX: Fusion ENergy Integrated multiphys-X,
#* A multiphysics application for modeling plasma facing components
#* https://github.com/idaholab/fenix
#* https://mooseframework.inl.gov/fenix
#*
#* FENIX is powered by the MOOSE Framework
#* https://www.mooseframework.inl.gov
#*
#* Licensed under LGPL 2.1, please see LICENSE for details
#* https://www.gnu.org/licenses/lgpl-2.1.html
#*
#* Copyright 2024, Battelle Energy Alliance, LLC
#* ALL RIGHTS RESERVED
#*

# Colors for bash output
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "Running ${SCRIPT_DIR}/build_cardinal.sh..."
FENIX_DIR=$(realpath $SCRIPT_DIR/..)

if [ -n "$MOOSE_DIR" ]; then
  echo "INFO: MOOSE_DIR set - ${MOOSE_DIR} will be used for both FENIX and Cardinal..."
else
  export MOOSE_DIR=${FENIX_DIR}/moose
fi

if [ -n "$CARDINAL_DIR" ]; then
  echo "INFO: CARDINAL_DIR set - using ${CARDINAL_DIR}..."
else
  CARDINAL_DIR=${FENIX_DIR}/cardinal
fi

# Checking for HDF5 using HDF5_DIR (set via conda automatically). If this is not set,
# check if PETSC_DIR is defined (indicates that PETSc is pre-installed, and HDF5 is
# installed via the PETSc script if not available using conda). If all else fails,
# throw an error code and request that the user set it!
if [ -n "$HDF5_DIR" ]; then
  export HDF5_ROOT=$HDF5_DIR
elif [ -n "$PETSC_DIR" ]; then
  export HDF5_ROOT=$PETSC_DIR
else
  echo -e "${RED}ERROR: HDF5 not found; please set HDF5_DIR in your environment!${NC}"
  exit 1
fi

cd ${CARDINAL_DIR}

# Enable/Disable Cardinal contribs
export ENABLE_NEK=no
export ENABLE_DAGMC=yes # Also enables MOAB

if [[ -z "${OPENMC_CROSS_SECTIONS}" ]]; then
  echo -e "${YELLOW}"
  echo "If you are using OpenMC, remember that you need to set OPENMC_CROSS_SECTIONS to point to a cross_sections.xml file!"
  echo "To get the ENDF/b-7.1 dataset, please run:"
  echo ""
  echo "$CARDINAL_DIR/scripts/download-openmc-cross-sections.sh"
  echo -e "${NC}"
fi

# If using conda, set LD_LIBRARY_PATH to workaround linking issue for some systems.
# Cardinal dev team is working on a more elegant fix, and this should be removed
# when that occurs.
if [ -n "$CONDA_PREFIX" ]; then
  export LD_LIBRARY_PATH=$CONDA_PREFIX/lib:$LD_LIBRARY_PATH
fi

# Run make, letting the number of jobs be either MOOSE_JOBS or 1 if MOOSE_JOBS is not set.
# MAKEFLAGS is used to make sure that CMake within Cardinal also uses the appropriate
# number of jobs. 'sh -c' is used to invoke make since this script is used within a
# Makefile, where it would be treated as a "submake". We want to de-couple this make
# invokation with the parent process.
sh -c make -j ${MOOSE_JOBS:-1} MAKEFLAGS=-j${MOOSE_JOBS:-1}
