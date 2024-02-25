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

#include "GeneralUserObject.h"

#include "AccumulatorBase.h"

/**
 * UserObject for testing the AuxAccumulator
 */
class TestAccumulatorBase : public GeneralUserObject
{
public:
  static InputParameters validParams();

  TestAccumulatorBase(const InputParameters & params);

  virtual void initialize() override {}
  virtual void execute() override;
  virtual void finalize() override {}

protected:
  virtual std::unique_ptr<FENIX::AccumulatorBase> buildAccumulator() = 0;
};
