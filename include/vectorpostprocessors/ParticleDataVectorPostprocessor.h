//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once
#include "Ray.h"
#include "GeneralVectorPostprocessor.h"
// Forward declarations
class PICStudyBase;

class ParticleDataVectorPostprocessor : public GeneralVectorPostprocessor
{
public:
  static InputParameters validParams();

  ParticleDataVectorPostprocessor(const InputParameters & parameters);

  /**
   * clears the data vector postprocessors before each
   * execution
   */
  virtual void initialize() override;
  /**
   * collects all of the data requested from particles
   */
  virtual void execute() override;
  /**
   * The finalize method simply communicates the data that exist on
   * individual procs to the root proc
   */
  virtual void finalize() override;

protected:
  /// the study that we are collecting data from
  const PICStudyBase & _study;
  /// location where the ray data indicies will be stored
  std::vector<RayDataIndex> _ray_data_indices;
  /// vector post processor values
  std::vector<VectorPostprocessorValue*> _data_values;
};
