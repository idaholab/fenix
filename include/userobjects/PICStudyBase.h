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
class PICRayStudy : public RayTracingStudy
{
public:
  PICRayStudy(const InputParameters & parameters);

  static InputParameters validParams();

  virtual void generateRays() override;
  /** Ray Data for storing all components of velocity */
  const RayDataIndex _v_x_index;
  const RayDataIndex _v_y_index;
  const RayDataIndex _v_z_index;

  // The banked rays to be used on the next timestep (restartable)
  std::vector<std::shared_ptr<Ray>> & _banked_rays;

protected:
  virtual void postExecuteStudy() override;

private:
  // The starting points
  const std::vector<Point> & _start_points;

  // The starting directions
  const std::vector<Point> & _start_velocities;
  // Whether or not we've generated rays yet (restartable)
  bool & _has_generated;
};
