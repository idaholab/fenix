//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TestSimpleVelocityUpdater.h"

registerMooseObject("FenixTestApp", TestSimpleVelocityUpdater);

InputParameters
TestSimpleVelocityUpdater::validParams()
{
  auto params = VelocityUpdaterBase::validParams();
  return params;
}


TestSimpleVelocityUpdater::TestSimpleVelocityUpdater(const InputParameters & parameters): VelocityUpdaterBase(parameters) {}

void
TestSimpleVelocityUpdater::updateVelocity(Ray & ray, const Point v, const Real dt) const
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
