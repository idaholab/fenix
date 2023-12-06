//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "VelocityUpdaterBase.h"

registerMooseObject("FenixApp", VelocityUpdaterBase);

InputParameters
VelocityUpdaterBase::validParams()
{
  auto params = GeneralUserObject::validParams();
  return params;
}

VelocityUpdaterBase::VelocityUpdaterBase(const InputParameters & parameters)
  : GeneralUserObject(parameters)
{}


void
VelocityUpdaterBase::updateVelocity(Ray & ray, const Point v, const Real dt) const
{
  // temporary point to store the new velocity as we work on it
  Point velocity = Point(0, 0, 0);
  // lets the get the dimension of the problem so we can make sure we don't have
  // rays with higher dimensional velocities than the mesh
  const auto dim = _fe_problem.mesh().dimension();

  if (dim >= 1)
    velocity(0) = v(0);

  if (dim >= 2)
    velocity(1) = v(1);

  if (dim == 3)
    velocity(2) = v(2);

  // max distance is the
  const auto max_dist = std::sqrt(velocity * velocity) * dt;
  // use the unit vector for the rays direction
  const auto direction = velocity.unit();

  ray.setStartingMaxDistance(max_dist);
  ray.setStartingDirection(direction);
}
