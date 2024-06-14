//* This file is part of FENIX: Fusion ENergy Integrated multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
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

#pragma once

#include "TestAccumulatorBase.h"

/**
 * UserObject for testing the AuxAccumulator
 */
class TestAuxAccumulator : public TestAccumulatorBase
{
public:
  static InputParameters validParams();

  TestAuxAccumulator(const InputParameters & params);

protected:
  virtual std::unique_ptr<FENIX::AccumulatorBase> buildAccumulator() override final;
};
