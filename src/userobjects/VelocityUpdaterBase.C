//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "VelocityUpdaterBase.h"



InputParameters
VelocityUpdaterBase::validParams()
{
  auto params = GeneralUserObject::validParams();
  return params;
}

VelocityUpdaterBase::VelocityUpdaterBase(const InputParameters & parameters)
  : GeneralUserObject(parameters)
{}

