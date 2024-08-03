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
#include "Ray.h"
#include "GeneralVectorPostprocessor.h"
// Forward declarations
class PICStudyBase;

class ParticleDataVectorPostprocessor : public GeneralVectorPostprocessor
{
public:
  static InputParameters validParams();

  ParticleDataVectorPostprocessor(const InputParameters & parameters);

  /**
   * clears the data vector postprocessors before each
   * execution
   */
  virtual void initialize() override;
  /**
   * collects all of the data requested from particles
   */
  virtual void execute() override;
  /**
   * The finalize method simply communicates the data that exists on
   * individual processors to the root processor
   */
  virtual void finalize() override;

protected:
  /// the study that we are collecting data from
  const PICStudyBase & _study;
  /// location where the ray data indicies will be stored
  std::vector<RayDataIndex> _ray_data_indices;
  /// vector postprocessor values
  std::vector<VectorPostprocessorValue *> _data_values;
};
