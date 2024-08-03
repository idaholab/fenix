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
#include "ParticleQuantityResidualAccumulatorBase.h"

class NumberDensityAccumulator : public ParticleQuantityResidualAccumulatorBase
{
public:
  static InputParameters validParams();

  NumberDensityAccumulator(const InputParameters & params);

  /**
   * Accumulates the contribution to the residual the inner product
   * of the particle number density and the test function
   */
  virtual void execute() override;
};
