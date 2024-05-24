//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "GeneralUserObject.h"

class Ray;

/** This struct stores all of the data needed to create a ray representing a computational particle*/
struct InitialParticleData {
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

  virtual std::vector<InitialParticleData> getParticleData() const = 0;

  /**
   * Unused methods
   */
  ///@{
  virtual void initialize() override final {}
  virtual void finalize() override final {}
  virtual void execute() override final {}
protected:
  const Real _mass;
  const Real _charge;
  const std::string & _species;
  const unsigned int _seed;
  const Real _mesh_dimension;
};

