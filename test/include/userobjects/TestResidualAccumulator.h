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

#include "TestAccumulatorBase.h"

/**
 * UserObject for testing the ResidualAccumulator
 */
class TestResidualAccumulator : public TestAccumulatorBase
{
public:
  static InputParameters validParams();

  TestResidualAccumulator(const InputParameters & params);

  virtual void execute() override;

protected:
  virtual std::unique_ptr<FENIX::AccumulatorBase> buildAccumulator() override final;
};
