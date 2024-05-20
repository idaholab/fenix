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

struct InitialParticleData {
  Point position;
  Point velocity;
  Real weight;
  Real charge;
  Real mass;
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
  const unsigned int _seed;
  const Real _mesh_dimension;
};
