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

#include "VariableSampler.h"
#include "TestPICStudyUserDefinedRays.h"

class TestVariableSampler : public TestPICStudyUserDefinedRays
{
public:
  TestVariableSampler(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void postExecuteStudy() override;

private:
  /// The utility used to sample a moose variable as a ray is traveling
  const FENIX::VariableSampler _sampler;
  /// The ray data index where the value of sampled field will be stored
  const RayDataIndex _field_idx;
};
