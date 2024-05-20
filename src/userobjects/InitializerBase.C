 //* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "InitializerBase.h"

InputParameters
InitializerBase::validParams()
{
  auto params = GeneralUserObject::validParams();
  params.addClassDescription("Base class for ParticleStepper. Provides the basic implementation"
                             "for dimensional dependent velocity updating."
                             "And the ability to sample vector fields for use in a particle step");
  params.addParam<unsigned int>("seed", 0, "An additional seed for the random number generators");
  return params;
}

InitializerBase::InitializerBase(const InputParameters & parameters)
  : GeneralUserObject(parameters),
  _seed(getParam<unsigned int>("seed")),
  _mesh_dimension(_fe_problem.mesh().dimension())
{
}

