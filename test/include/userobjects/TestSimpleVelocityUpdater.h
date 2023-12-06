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

#include "VelocityUpdaterBase.h"


class TestSimpleVelocityUpdater: public VelocityUpdaterBase
{
public:

  TestSimpleVelocityUpdater(const InputParameters & parameters);

  static InputParameters validParams();

  virtual void updateVelocity(Ray & ray, const Point v, const Real dt) const;
};
