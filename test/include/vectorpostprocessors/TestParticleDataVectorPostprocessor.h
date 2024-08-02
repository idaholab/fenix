//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once
#include "ParticleDataVectorPostprocessor.h"

class TestParticleDataVectorPostprocessor : public ParticleDataVectorPostprocessor
{
public:
  static InputParameters validParams();

  TestParticleDataVectorPostprocessor(const InputParameters & parameters);

  /**
   * adds the ray id to the particle data for sorting results
   * This will help ensure testability
   */
  virtual void execute() override;
  /**
   * In the testing version of this the finalize not only
   * communicates between procs but also sorts the data by ray
   * id so that it will be consistent in testing
   */
  virtual void finalize() override;

};
