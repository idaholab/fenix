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


InputParameters
ProjectionKernel::validParams()
{
  InputParameters params = ADKernelValue::validParams();
  params..addClassDescription("Kernel for projecting discrete particle quantities onto the finite element mesh");
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
