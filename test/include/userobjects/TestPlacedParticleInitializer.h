//* This file is part of FENIX: Fusion ENergy Integrated multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ParticleInitializerBase.h"

class TestPlacedParticleInitializer : public ParticleInitializerBase
{
public:
  TestPlacedParticleInitializer(const InputParameters & parameters);

  static InputParameters validParams();
  /**
   * Particle initializer for uniform densities.
   * The particle weight will be based on a constant charge density and
   * the specified number of particles per element.
   */
  virtual std::vector<InitialParticleData> getParticleData() const override;

  /**
   * Overridden to be able to pull the distribution objects into this class
   */
  virtual void initialSetup() override {}

protected:
  /// The starting points
  const std::vector<Point> & _start_points;
  /// The starting velocities
  const std::vector<Point> & _start_velocities;
  /// The mass of the particles being placed
  const Real _mass;
  /// the charge of the particles being placed
  const Real _charge;
  /// the weight of the particles being placed
  const Real _weight;
};
