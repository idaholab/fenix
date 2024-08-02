//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TestParticleDataVectorPostprocessor.h"
#include "PICStudyBase.h"

registerMooseObject("FenixTestApp", TestParticleDataVectorPostprocessor);

InputParameters
TestParticleDataVectorPostprocessor::validParams()
{
  InputParameters params = ParticleDataVectorPostprocessor::validParams();
  return params;
}

TestParticleDataVectorPostprocessor::TestParticleDataVectorPostprocessor(const InputParameters & parameters)
  : ParticleDataVectorPostprocessor(parameters) {}


void
TestParticleDataVectorPostprocessor::finalize()
{
  // this will communicate the data between procs
  ParticleDataVectorPostprocessor::finalize();
  // now we will sort the data in the csv files by ray id
  // this is only really needed for testing and also requires
  // that we use a parallel consistent ray indexing scheme
  std::vector<size_t> indicies;
  indicies.resize(_data_values.front()->size());
  std::iota(indicies.begin(), indicies.end(), 0);
  std::sort(indicies.begin(),
            indicies.end(),
            [&](size_t a, size_t b) -> bool
            {
              return (*_data_values.front())[a] < (*_data_values.front())[b];
            });

  for (auto data : _data_values)
    Moose::applyIndices(*data, indicies);
}
