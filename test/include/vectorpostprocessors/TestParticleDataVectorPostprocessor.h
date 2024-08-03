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
#include "ParticleDataVectorPostprocessor.h"

class TestParticleDataVectorPostprocessor : public ParticleDataVectorPostprocessor
{
public:
  static InputParameters validParams();

  TestParticleDataVectorPostprocessor(const InputParameters & parameters);

  /**
   * adds the ray id to the particle data for sorting results
   * This will help ensure testability
   */
  virtual void execute() override;
  /**
   * In the testing version of this the finalize not only
   * communicates between processors but also sorts the data by ray
   * id so that it will be consistent in testing
   */
  virtual void finalize() override;
};
