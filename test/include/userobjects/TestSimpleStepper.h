//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ParticleStepperBase.h"

class TestSimpleStepper : public ParticleStepperBase
{
public:
  TestSimpleStepper(const InputParameters & parameters);

  static InputParameters validParams();

  /**
   * A simple particle stepper which will not modify the velocity of the particle
   * it will simply update the velocity with the rule v^2 * dt
   */
  virtual void
  setupStep(Ray & ray, Point & v, const Real q_m_ratio, const Real distance = 0) const override;
};
