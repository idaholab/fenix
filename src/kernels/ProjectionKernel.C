//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ProjectionKernel.h"

registerMooseObject("FenixApp", ProjectionKernel);

/**
 * This macro defines the valid parameters for
 * this Kernel and their default values
 */
InputParameters
ProjectionKernel::validParams()
{
  InputParameters params = ADKernelValue::validParams();
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
