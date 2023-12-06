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

// MOOSE Includes
#include "Ray.h"
#include "GeneralUserObject.h"
#include "RayTracingStudy.h"

class VelocityUpdaterBase: public GeneralUserObject
{
public:

  VelocityUpdaterBase(const InputParameters & parameters);

  static InputParameters validParams();

  virtual void initialSetup() override {}
  virtual void residualSetup() override {}
  virtual void jacobianSetup() override {}
  virtual void meshChanged() override {}
  virtual void timestepSetup() override {}
  virtual void initialize() override {}
  virtual void finalize() override {}
  virtual void execute() override {}

  virtual void updateVelocity(Ray & ray, const Point v, const Real dt) const = 0;
};
