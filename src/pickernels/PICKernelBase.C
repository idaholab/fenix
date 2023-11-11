//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
// MOOSE includes
#include "PICKernelBase.h"

registerMooseObject("FenixApp", PICKernelBase);

InputParameters
PICKernelBase::validParams()
{
  InputParameters params = GeneralRayKernel::validParams();

  params.addClassDescription("Compute the value of a variable at a specified location.");
  params.addRequiredParam<std::vector<VariableName>>(
      "components", "The name of the vector components that act on the particles");
  params.addClassDescription("Compute a force on the particle due a vector");
  return params;
}

PICKernelBase::PICKernelBase(const InputParameters & params)
  : GeneralRayKernel(params),
    _v_x_index(_study.getRayDataIndex("v_x")),
    _v_y_index(_study.getRayDataIndex("v_y")),
    _v_z_index(_study.getRayDataIndex("v_z"))
{
}

void
PICKernelBase::preTrace()
{
  auto ray = currentRay();
  Point v = Point(ray->data()[_v_x_index], ray->data()[_v_y_index], ray->data()[_v_z_index]);
  auto dt = _fe_problem.dt();
  setDirectionAndMaxDistance(*ray, v, dt);
}

void
PICKernelBase::setDirectionAndMaxDistance(Ray & ray, const Point v, const Real dt)
{
  libMesh::Point velocity = Point(0, 0, 0);
  // calculating max distance for the correct problem dimention
  auto dim = _fe_problem.mesh().dimension();

  if (dim >= 1)
    velocity(0) = v(0);

  if (dim >= 2)
    velocity(1) = v(1);

  if (dim == 3)
    velocity(2) = v(2);

  const auto max_dist = std::sqrt(velocity * velocity) * dt;
  const auto direction = velocity.unit();

  ray.setStartingMaxDistance(max_dist);
  ray.setStartingDirection(direction);
}

void
PICKernelBase::onSegment()
{
}
