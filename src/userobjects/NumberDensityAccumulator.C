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

#include "NumberDensityAccumulator.h"
#include "ResidualAccumulator.h"
#include "PICStudyBase.h"
#include "MooseMesh.h"

registerMooseObject("FenixApp", NumberDensityAccumulator);

InputParameters
NumberDensityAccumulator::validParams()
{
  auto params = ParticleQuantityResidualAccumulatorBase::validParams();
  return params;
}

NumberDensityAccumulator::NumberDensityAccumulator(const InputParameters & params)
  : ParticleQuantityResidualAccumulatorBase(params)
{
}

void
NumberDensityAccumulator::execute()
{
  if (_fe_problem.currentlyComputingResidual())
  {
    std::unique_ptr<FENIX::AccumulatorBase> accumulator =
        std::make_unique<FENIX::ResidualAccumulator>(_fe_problem, this, _var_name, 0);

    auto particles = _study.getBankedRays();

    for (auto & p : particles)
    {
      accumulator->add(
          *p->currentElem(), p->currentPoint(), p->data(_weight_index));
    }

    accumulator->finalize();
  }
}
