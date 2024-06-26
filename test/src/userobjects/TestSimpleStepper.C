//* This file is part of FENIX: Fusion ENergy Integrated multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//* https://mooseframework.inl.gov/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
//*
//* Copyright 2024, Battelle Energy Alliance, LLC
//* ALL RIGHTS RESERVED
//*

#include "TestSimpleStepper.h"

registerMooseObject("FenixTestApp", TestSimpleStepper);

InputParameters
TestSimpleStepper::validParams()
{
  auto params = ParticleStepperBase::validParams();
  params.addClassDescription("Simple test stepper which does not modify the particle velocity just "
                             "updates the direction and maximum distance");
  return params;
}

TestSimpleStepper::TestSimpleStepper(const InputParameters & parameters)
  : ParticleStepperBase(parameters)
{
}

void
TestSimpleStepper::setupStep(Ray & ray,
                             Point & v,
                             const Real /*q_m_ratio*/,
                             const Real /*disatnce*/) const
{
  setMaxDistanceAndDirection(ray, v, _dt);
}
