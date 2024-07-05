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

# Set various flags based on argument input (fast, help, skip update, etc.)
for i in "$@"
do
  shift
  if [ "$i" == "--fast" ]; then
    go_fast=1;
  fi

  if [[ "$i" == "-h" || "$i" == "--help" ]]; then
    help=1;
  fi

  if [ "$i" == "--skip-submodule-update" ]; then
    skip_sub_update=1;
  else # Remove the skip submodule update argument before passing further
    set -- "$@" "$i"
  fi
done

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
echo "Running ${SCRIPT_DIR}/update_and_rebuild_fenix.sh..."
FENIX_DIR=$(realpath $SCRIPT_DIR/..)

if [ -n "$MOOSE_DIR" ]; then
  echo "INFO: MOOSE_DIR set - ${MOOSE_DIR} will be used for both FENIX and Cardinal..."
else
  export MOOSE_DIR=${FENIX_DIR}/moose
fi

if [ -n "$CARDINAL_DIR" ]; then
  echo "INFO: CARDINAL_DIR set - using ${CARDINAL_DIR} and skipping submodule update..."
  skip_sub_update=1
else
  CARDINAL_DIR=${FENIX_DIR}/cardinal
fi

# Checking for HDF5 using HDF5_DIR (set via conda automatically). If this is not set,
# throw an error code and request that the user set it!
if [ -n "$HDF5_DIR" ]; then
  export HDF5_ROOT=$HDF5_DIR
else
  echo -e "${RED}ERROR: HDF5 not found; please set HDF5_DIR in your environment!${NC}"
  exit 1
fi


# Display help if requested
if [[ -n "$help" ]]; then
  echo "Usage: $0 [-h | --help | --fast | --skip-submodule-update ]"
  echo
  echo "-h | --help              Display this message"
  echo "--fast                   Run Cardinal 'make' only, do NOT update Cardinal contribs"
  echo "--skip-submodule-update  Do not update the Cardinal submodule, use the current version"
  echo "*************************************************************************************"
  echo ""

  exit 0
fi

# Print information to the screen if fast is used with any other arguments other than skip-submodule-update
if [[ -n "$go_fast" && $# != 1 ]]; then
  echo -e "${RED}ERROR: --fast can only be used by itself or with --skip-submodule-update."
  echo -e "Try again, removing either --fast or all other conflicting arguments!${NC}"
  exit 1;
fi

# Update Cardinal submodule if we're not skipping
if [ -z "$skip_sub_update" ]; then
  cd ${FENIX_DIR}
  git_dir=`git rev-parse --show-cdup 2>/dev/null`
  if [[ -z "$go_fast" && $? == 0 && "x$git_dir" == "x" ]]; then
    git submodule update --init cardinal
    if [[ $? != 0 ]]; then
      echo "git submodule command failed, are your proxy settings correct?"
      exit 1
    fi
  fi
fi

cd ${CARDINAL_DIR}

# Enable/Disable Cardinal contribs
export ENABLE_NEK=no
export ENABLE_DAGMC=yes # Also enables MOAB

# If we're not going fast, update all dependencies
if [ -z "$go_fast" ]; then
  echo "INFO: Updating OpenMC, DAGMC, and MOAB submodules..."
  git submodule update --init --recursive contrib/openmc
  git submodule update --init contrib/DAGMC
  git submodule update --init contrib/moab
fi

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
# number of jobs.
make -j ${MOOSE_JOBS:-1} MAKEFLAGS=-j${MOOSE_JOBS:-1}

# Remove cardinal executable, not required for FENIX build.
echo -e "${YELLOW}WARNING: Removing cardinal-${METHOD:-opt} since FENIX will be built."
echo -e "If a Cardinal executable is desired, please navigate to ${CARDINAL_DIR} and 'make' again.${NC}"
rm cardinal-"${METHOD:-opt}"
