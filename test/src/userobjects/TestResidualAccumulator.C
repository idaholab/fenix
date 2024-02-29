//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TestResidualAccumulator.h"

#include "ResidualAccumulator.h"

registerMooseObject("FenixTestApp", TestResidualAccumulator);

InputParameters
TestResidualAccumulator::validParams()
{
  auto params = TestAccumulatorBase::validParams();

  // Make sure that you add these params when using the ResidualAccumulator
  params += TaggingInterface::validParams();

  // You need to add this exec flag so it can contribute to residuals
  ExecFlagEnum & exec_enum = params.set<ExecFlagEnum>("execute_on", true);
  exec_enum.addAvailableFlags(EXEC_PRE_KERNELS);

  params.addParam<NonlinearVariableName>("variable", "The variable to accumulate into");

  return params;
}

TestResidualAccumulator::TestResidualAccumulator(const InputParameters & params)
  : TestAccumulatorBase(params)
{
}

void
TestResidualAccumulator::execute()
{
  if (_fe_problem.currentlyComputingResidual())
    TestAccumulatorBase::execute();
}

std::unique_ptr<FENIX::AccumulatorBase>
TestResidualAccumulator::buildAccumulator()
{
  std::unique_ptr<FENIX::AccumulatorBase> accumulator =
      std::make_unique<FENIX::ResidualAccumulator>(
          _fe_problem, this, getParam<NonlinearVariableName>("variable"), 0);
  return accumulator;
}
