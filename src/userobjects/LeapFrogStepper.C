//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "LeapFrogStepper.h"

registerMooseObject("FenixApp", LeapFrogStepper);

InputParameters
LeapFrogStepper::validParams()
{
  auto params = ParticleStepperBase::validParams();
  params.addClassDescription("Particle Stepper which implements a simple leap frog update where "
                             "the velocity and position are updated with a 1/2 dt offset.");
  params.addRequiredParam<std::vector<VariableName>>(
      "field_components",
      "A list of 3 variables which represent the 3 components of the force field acting on "
      "particles");
  return params;
}

LeapFrogStepper::LeapFrogStepper(const InputParameters & parameters)
  : ParticleStepperBase(parameters),
    _field_vars(getParam<std::vector<VariableName>>("field_components"))
{
  if (_field_vars.size() != 3)
    mooseError("LeapFrogStepper with name ",
               name(),
               ": You must provide 3 components representing the force field!");

  for (int i = 0; i < 3; ++i)
  {
    _field_samplers.push_back(FENIX::VariableSampler(_fe_problem, _field_vars[i], _tid));
  }
}

void
LeapFrogStepper::setupStep(Ray & ray, Point & v, const Real q_m_ratio, const Real distance) const
{

  // if it is the particles first step we need to take a half step
  // for the velocity other wise we take a full step
  Point F = sampleField(_field_samplers, ray);

  const auto dt = distance == 0 ? _dt / 2 : _dt;
  v = linearImpulse(v, F, q_m_ratio, dt);
  setMaxDistanceAndDirection(ray, v, dt);
}
