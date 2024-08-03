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

#include "TestVariableSampler.h"

registerMooseObject("FenixTestApp", TestVariableSampler);

InputParameters
TestVariableSampler::validParams()
{
  InputParameters params = TestInitializedPICStudy::validParams();
  params.addRequiredParam<VariableName>("field", "The field that you want to sample");
  return params;
}

TestVariableSampler::TestVariableSampler(const InputParameters & params)
  : TestInitializedPICStudy(params),
    _sampler(FENIX::VariableSampler(_fe_problem, getParam<VariableName>("field"), _tid)),
    _field_idx(registerRayData("field_value"))
{
}

void
TestVariableSampler::postExecuteStudy()
{
  TestInitializedPICStudy::postExecuteStudy();
  for (auto & ray : _banked_rays)
  {
    ray->data(_field_idx) = _sampler.sampleVariable(ray->currentPoint(), ray->currentElem());
  }
}
