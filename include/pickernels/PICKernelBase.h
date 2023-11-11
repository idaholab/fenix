//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once
// Local Includes
#include "GeneralRayKernel.h"

class PICKernelBase : public GeneralRayKernel
{
public:
  PICKernelBase(const InputParameters & params);

  static InputParameters validParams();

  virtual void preTrace() override;

  virtual void onSegment() override;

protected:
  /**
   * Method for giving the user an easy way to update the particles direction and
   * maximum distance based on its velocity, time step and the dimension of the mesh
   * @param ray the ray you would like to update the velocity of
   * @param v the new velocity you want to assign to the ray
   * @param dt the time step we want the particle to take. This is required as some
   *           particle steps requires a half step for the first step and a full step
   *           afterwards
   */
  void setDirectionAndMaxDistance(Ray & ray, const Point v, const Real dt);

private:
  /** Data where the particle 3 componet velocity is stored */
  const RayDataIndex _v_x_index;
  const RayDataIndex _v_y_index;
  const RayDataIndex _v_z_index;
};
