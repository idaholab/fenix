//* This file is part of FENIX: Fusion ENergy Integrated multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
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

#include "ProjectionKernel.h"

registerMooseObject("FenixApp", ProjectionKernel);

InputParameters
ProjectionKernel::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params.addClassDescription(
      "Kernel for projecting discrete particle quantities onto the finite element mesh");
  return params;
}

ProjectionKernel::ProjectionKernel(const InputParameters & parameters) : ADKernelValue(parameters)
{
}

ADReal
ProjectionKernel::precomputeQpResidual()
{
  return _u[_qp];
}
