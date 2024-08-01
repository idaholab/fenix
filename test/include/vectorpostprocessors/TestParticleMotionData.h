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

class TestParticleMotionData : public GeneralVectorPostprocessor
{
public:
  static InputParameters validParams();

  TestParticleMotionData(const InputParameters & parameters);

  virtual void initialize() override;
  virtual void execute() override;
  virtual void finalize() override;

protected:
  /// the study that we are collecting data from
  const PICStudyBase & _study;
  /// Ray Data indicies for the velocity data
  ///@{
  const RayDataIndex _v_x_index;
  const RayDataIndex _v_y_index;
  const RayDataIndex _v_z_index;
  ///@}
  /// the time at which position is calculated
  VectorPostprocessorValue & _t_pos;
  /// Collector for particle position data
  ///@{
  VectorPostprocessorValue & _x;
  VectorPostprocessorValue & _y;
  VectorPostprocessorValue & _z;
  ///@}
  /// the time at which velocity is calculated
  VectorPostprocessorValue & _t_vel;
  /// Collector for particle velocity data
  ///@{
  VectorPostprocessorValue & _v_x;
  VectorPostprocessorValue & _v_y;
  VectorPostprocessorValue & _v_z;
  ///@}
  /// the ray id
  VectorPostprocessorValue & _id;
};
