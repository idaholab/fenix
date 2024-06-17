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

#include "ParticlesPerElementInitializer.h"

class BoundingBoxInitializer : public ParticlesPerElementInitializer
{
public:
  BoundingBoxInitializer(const InputParameters & parameters);

  static InputParameters validParams();
  /**
   * Initialization that applies a bounding box to
   * ParticlesPerElementInitializer
   * if the bounding box encompasses the whole domain then these are the same
   */
  virtual std::vector<InitialParticleData> getParticleData() const override;

protected:
  /// The Point object constructed from the x1, y1, z1 components for the bottom left BB corner
  const Point _bottom_left;

  /// The Point object constructed from the x2, y2, z2 components for the bottom left BB corner
  const Point _top_right;
  /// Holds the coefficients for each of the planes which are required for checking if an element is contained within the bounding box
  const std::vector<std::vector<Real>> _planes;
};
