//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "RayTracingStudy.h"

/**
 * Test study for generating rays for a basic particle-in-cell capability,
 * where Rays have propagate a bit each time step
 */
class PICStudyBase : public RayTracingStudy
{
public:
  PICStudyBase(const InputParameters & parameters);

  static InputParameters validParams();

  virtual void generateRays() override;
  // The banked rays to be used on the next timestep (restartable)
  std::vector<std::shared_ptr<Ray>> & _banked_rays;

protected:
  virtual void postExecuteStudy() override;
  /** Ray Data for storing all components of velocity */
  const RayDataIndex _v_x_index;
  const RayDataIndex _v_y_index;
  const RayDataIndex _v_z_index;
  /** Keep a second counter so that we don't try and set the
   * ray direction again when it is passed from one processor to another
   */
  const RayDataIndex _direction_set_index;
  // Whether or not we've generated rays yet (restartable)
  bool & _has_generated;
};
