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

#include "ParticleInitializerBase.h"
#include "Distribution.h"

InputParameters
ParticleInitializerBase::validParams()
{
  auto params = GeneralUserObject::validParams();
  params.addClassDescription("Base class for ParticleStepper. Provides the basic implementation"
                             "for dimensional dependent velocity updating."
                             "And the ability to sample vector fields for use in a particle step");
  params.addParam<unsigned int>("seed", 0, "An additional seed for the random number generators");
  params.addRangeCheckedParam<Real>(
      "mass", 1.0, "mass > 0.0", "The mass of the particles being placed in the mesh");
  params.addParam<Real>("charge", 1, "The charge of the particles being placed in the mesh");
  params.addParam<std::string>("species", "", "The type of particle that is being initialized");
  params.addRequiredParam<std::vector<DistributionName>>(
      "velocity_distributions",
      "The distribution names to be sampled when initializing the velocity of each particle");
  return params;
}

ParticleInitializerBase::ParticleInitializerBase(const InputParameters & parameters)
  : GeneralUserObject(parameters),
    _mass(getParam<Real>("mass")),
    _charge(getParam<Real>("charge")),
    _species(getParam<std::string>("species")),
    _seed(getParam<unsigned int>("seed")),
    _mesh_dimension(_fe_problem.mesh().dimension()),
    _distribution_names(getParam<std::vector<DistributionName>>("velocity_distributions"))
{
  if (_distribution_names.size() != 3)
    paramError("velocity_distributions",
               "You must provide 3 distributions, one for each velocity component.");
}

void
ParticleInitializerBase::initialSetup()
{
  // Needed because distributions are constructed after UserObjects
  for (const DistributionName & name : _distribution_names)
    _velocity_distributions.push_back(&getDistributionByName(name));
}
