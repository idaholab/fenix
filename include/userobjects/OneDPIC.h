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

#include "PICStudyBase.h"

/**
 * Test study for generating rays for a basic particle-in-cell capability,
 * where Rays have propagate a bit each time step
 */
class OneDPIC : public PICStudyBase
{
public:
  OneDPIC(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void initializeParticles() override final;
};
