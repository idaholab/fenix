//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "BorisStepper.h"

registerMooseObject("FenixApp", BorisStepper);

InputParameters
BorisStepper::validParams()
{
  auto params = ParticleStepperBase::validParams();
  params.addClassDescription(
      "Electromagnetic particle stepper method implementing the Boris Algorithm.");
  params.addRequiredParam<std::vector<VariableName>>(
      "efield_components",
      "A list of 3 variables which represent the 3 components of the electric field");
  params.addRequiredParam<std::vector<VariableName>>(
      "bfield_components",
      "A list of 3 variables which represent the 3 components of the electric field");
  return params;
}

BorisStepper::BorisStepper(const InputParameters & parameters)
  : ParticleStepperBase(parameters),
    _efield_vars(getParam<std::vector<VariableName>>("efield_components")),
    _bfield_vars(getParam<std::vector<VariableName>>("bfield_components"))
{
  if (_efield_vars.size() != 3)
    mooseError("BorisStepper with name ",
               name(),
               ": You must provide 3 components representing the electric field!");

  if (_bfield_vars.size() != 3)
    mooseError("BorisStepper with name ",
               name(),
               ": You must provide 3 components representing the magnetic field!");

  for (int i = 0; i < 3; ++i)
  {
    _efield_samplers.push_back(FENIX::VariableSampler(_fe_problem, _efield_vars[i], _tid));
    _bfield_samplers.push_back(FENIX::VariableSampler(_fe_problem, _bfield_vars[i], _tid));
  }
}

void
BorisStepper::setupStep(Ray & ray, Point & v, const Real q_m_ratio, const Real distance) const
{

  Real dt = _dt;
  // if it is the particles first step we need to take a half step
  // otherwise we take a full step
  if (distance == 0)
    dt /= 2;
  // let's sample the fields at the ray location so we can update the velocity properly
  Point E = sampleField(_efield_samplers, ray);
  Point B = sampleField(_bfield_samplers, ray);
  // calculate v^-
  v = linearImpulse(v, E, q_m_ratio, dt / 2);
  // calculate v^+
  v = magneticImpulse(v, B, q_m_ratio, dt);
  // calculate the final velocity and setup the particle for the step
  v = linearImpulse(v, E, q_m_ratio, dt / 2);
  setMaxDistanceAndDirection(ray, v, _dt);
}

Point
BorisStepper::magneticImpulse(const Point & v,
                              const Point & B,
                              const Real q_m_ratio,
                              const Real dt) const
{
  auto l = q_m_ratio * B * dt / 2;
  auto s = 2 * l / (1 + l * l);

  auto v_prime = v + v.cross(l);

  return v + v_prime.cross(s);
}
