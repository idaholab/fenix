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

#include "GeneralUserObject.h"

class Distribution;

struct InitialParticleData
{
  /// the location where the particle will be placed
  Point position;
  /// the velocity that the particle will be given
  Point velocity;
  /// the type of physical particle this particle represents
  std::string species;
  /// the number of physical particles that this particle represents
  Real weight;
  /// the charge of the physical particle this computational particle represents
  Real charge;
  /// the mass of the physical particle this computational particle represents
  Real mass;
  /// the element that this particle will be placed into
  const Elem * elem;
};

class InitializerBase : public GeneralUserObject
{
public:
  InitializerBase(const InputParameters & parameters);

  static InputParameters validParams();

  /**
   * This method will be called by PIC studies to give them all of the data needed to create and
   * place particles on the finite element mesh
   * */
  virtual std::vector<InitialParticleData> getParticleData() const = 0;

  /**
   * overridden to be able to pull the distribution objects into this class
   */
  virtual void initialSetup() override;

  /**
   * Unused methods
   */
  ///@{
  virtual void initialize() override final {}
  virtual void finalize() override final {}
  virtual void execute() override final {}

protected:
  /// the mass of the particles being created
  const Real _mass;
  /// the charge of the particles being created
  const Real _charge;
  /// the type of particle being created
  const std::string & _species;
  /// a user specified seed for changing random number generator seeds
  const unsigned int _seed;
  /// the dimension of the finite element mesh
  const Real _mesh_dimension;
  /// the distributions that will be used for set the initial particle velocities
  std::vector<Distribution const *> _velocity_distributions;
  /// Velocity distribution names
  const std::vector<DistributionName> & _distribution_names;
};
