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

class NegativeVariableGradientComponent : public AuxKernel
{
public:
  NegativeVariableGradientComponent(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  /**
   * Computes a specific component of the variable gradient
   */
  virtual Real computeValue() override;
  /// The gradient of a coupled variable
  const VariableGradient & _grad_var;
  /// The component of the gradient being accessed
  const unsigned int _component;
};
