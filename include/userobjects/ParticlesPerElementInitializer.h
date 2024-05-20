//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Fr amework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once
#include "MooseRandom.h"
#include "Distribution.h"
#include "InitializerBase.h"

class ParticlesPerElementInitializer : public InitializerBase
{
public:
  ParticlesPerElementInitializer(const InputParameters & parameters);

  static InputParameters validParams();

  virtual std::vector<InitialParticleData> getParticleData() const override;
  virtual void initialSetup() override;

  unsigned int getParticlesPerElement() const {return _particles_per_element;}

protected:
  const Real _mass;
  const Real _charge;
  const Real _charge_density;
  /// the number of particles that will be placed in each element
  const unsigned int _particles_per_element;

  /// Storage for distribution objects to be utilized
  std::vector<Distribution const *> _velocity_distributions;

  /// Distribution names
  const std::vector<DistributionName> & _distribution_names;
};
