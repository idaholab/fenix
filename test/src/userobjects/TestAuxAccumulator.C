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

#include "TestAuxAccumulator.h"

#include "AuxAccumulator.h"

registerMooseObject("FenixTestApp", TestAuxAccumulator);

InputParameters
TestAuxAccumulator::validParams()
{
  auto params = TestAccumulatorBase::validParams();
  params.addParam<AuxVariableName>("variable", "The variable to accumulate into");
  return params;
}

TestAuxAccumulator::TestAuxAccumulator(const InputParameters & params) : TestAccumulatorBase(params)
{
}

std::unique_ptr<FENIX::AccumulatorBase>
TestAuxAccumulator::buildAccumulator()
{
  std::unique_ptr<FENIX::AccumulatorBase> accumulator =
      std::make_unique<FENIX::AuxAccumulator>(_fe_problem, getParam<AuxVariableName>("variable"));
  return accumulator;
}
