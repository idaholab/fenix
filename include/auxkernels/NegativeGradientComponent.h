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

#include "AuxKernel.h"

class NegativeGradientComponent : public AuxKernel
{
public:
  NegativeGradientComponent(const InputParameters & parameters);

  static InputParameters validParams();

protected:

  virtual Real computeValue() override;
  /// The gradient of a coupled variable
  const VariableGradient & _grad_var;
  /// the component of the gradient you will access
  const unsigned int _component;
};
