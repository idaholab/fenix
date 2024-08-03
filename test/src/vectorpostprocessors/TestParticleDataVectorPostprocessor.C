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

#include "TestParticleDataVectorPostprocessor.h"
#include "PICStudyBase.h"

registerMooseObject("FenixTestApp", TestParticleDataVectorPostprocessor);

InputParameters
TestParticleDataVectorPostprocessor::validParams()
{
  InputParameters params = ParticleDataVectorPostprocessor::validParams();
  return params;
}

TestParticleDataVectorPostprocessor::TestParticleDataVectorPostprocessor(
    const InputParameters & parameters)
  : ParticleDataVectorPostprocessor(parameters)
{
  _data_values.push_back(&declareVector("id"));
}

void
TestParticleDataVectorPostprocessor::execute()
{
  // This does end up looping over the rays twice,
  // but since it will only be for testing, this is fine.
  // It helps to test the behaviour of the object people will actually
  // use without requiring it to store meta data that is not important for them.
  ParticleDataVectorPostprocessor::execute();
  const auto rays = _study.getBankedRays();
  for (const auto & ray : rays)
    _data_values.back()->push_back(ray->id());
}

void
TestParticleDataVectorPostprocessor::finalize()
{
  // This will communicate the data between processors
  ParticleDataVectorPostprocessor::finalize();
  // Now we will sort the data in the csv files by ray id.
  // This is only really needed for testing and also requires
  // that we use a parallel consistent ray indexing scheme.
  std::vector<size_t> indicies;
  indicies.resize(_data_values.front()->size());
  std::iota(indicies.begin(), indicies.end(), 0);
  std::sort(indicies.begin(),
            indicies.end(),
            [&](size_t a, size_t b) -> bool
            { return (*_data_values.back())[a] < (*_data_values.back())[b]; });

  for (auto data : _data_values)
    Moose::applyIndices(*data, indicies);
}
