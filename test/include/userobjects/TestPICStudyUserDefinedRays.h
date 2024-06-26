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

#include "PICStudyBase.h"

/**
 * Test study for generating rays for a basic particle-in-cell capability,
 * where Rays have propagate a bit each time step
 */
class TestPICStudyUserDefinedRays : public PICStudyBase
{
public:
  TestPICStudyUserDefinedRays(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void initializeParticles() override final;

private:
  /// The starting points
  const std::vector<Point> & _start_points;

  /// The starting velocities
  const std::vector<Point> & _start_velocities;
  /// the mass of the particles being placed
  const Real _mass;
  /// the charge of the particles being placed
  const Real _charge;
  /// the weight of the particles being placed
  const Real _weight;
};
