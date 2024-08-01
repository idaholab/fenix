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

#include "ChargeDensityAccumulator.h"
#include "ResidualAccumulator.h"
#include "PICStudyBase.h"
#include "MooseMesh.h"

registerMooseObject("FenixApp", ChargeDensityAccumulator);

InputParameters
ChargeDensityAccumulator::validParams()
{
  auto params = ParticleQuantityResidualAccumulatorBase::validParams();
  params.addClassDescription(
      "Accumulator used to evaluate the inner product of the particle charge density and the test function "
      "required for solving electromagnetic equations");
  return params;
}

ChargeDensityAccumulator::ChargeDensityAccumulator(const InputParameters & params)
  : ParticleQuantityResidualAccumulatorBase(params)
{
}

void
ChargeDensityAccumulator::execute()
{
  if (_fe_problem.currentlyComputingResidual())
  {
    std::unique_ptr<FENIX::AccumulatorBase> accumulator =
        std::make_unique<FENIX::ResidualAccumulator>(_fe_problem, this, _var_name, 0);

    auto particles = _study.getBankedRays();

    for (auto & p : particles)
    {
      accumulator->add(
          *p->currentElem(), p->currentPoint(), p->data(_charge_index) * p->data(_weight_index));
    }

    accumulator->finalize();
  }
}
