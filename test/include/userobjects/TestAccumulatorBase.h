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
