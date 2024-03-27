//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ParticleStepperBase.h"

InputParameters
ParticleStepperBase::validParams()
{
  auto params = GeneralUserObject::validParams();
  params.addClassDescription("Base class for ParticleStepper. Provides the basic implementation"
                             "for dimensional dependent velocity updating."
                             "And the ability to sample vector fields for use in a particle step");
  return params;
}

ParticleStepperBase::ParticleStepperBase(const InputParameters & parameters)
  : GeneralUserObject(parameters), _dim(_fe_problem.mesh().dimension())
{
}

void
ParticleStepperBase::setMaxDistanceAndDirection(Ray & ray, const Point & v, const Real dt) const
{

  if (v.absolute_fuzzy_equals(Point(0, 0, 0)))
  {
    ray.setStationary();
    return;
  }
  // temporary point to store the new velocity as we work on it
  Point velocity = Point(0, 0, 0);

  if (_dim >= 1)
    velocity(0) = v(0);

  if (_dim >= 2)
    velocity(1) = v(1);

  if (_dim == 3)
    velocity(2) = v(2);

  // max distance is v^2 dt
  const auto max_distance = std::sqrt(velocity * velocity) * dt;

  ray.setStartingMaxDistance(max_distance);
  ray.setStartingDirection(velocity);
}

Point
ParticleStepperBase::sampleField(const std::vector<FENIX::VariableSampler> & field_samplers,
                                 const Ray & ray) const
{
  const auto p = ray.currentPoint();
  const auto e = ray.currentElem();
  return Point(field_samplers[0].sampleVariable(p, e),
               field_samplers[1].sampleVariable(p, e),
               field_samplers[2].sampleVariable(p, e));
}

Point
ParticleStepperBase::linearImpulse(const Point & v,
                                   const Point & F,
                                   const Real q_m_ratio,
                                   const Real dt) const
{
  return v + q_m_ratio * F * dt;
}
