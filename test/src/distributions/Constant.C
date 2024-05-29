//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "Constant.h"

registerMooseObject("FenixTestApp", Constant);

InputParameters
Constant::validParams()
{
  InputParameters params = Distribution::validParams();
  params.addClassDescription("Distribution that returns a constant for all random numbers sampled, "
                             "pdf and cdf are not intended to be used");
  params.addRequiredParam<Real>(
      "value", "the constant that will be returned when the quantile method is called");
  return params;
}

Constant::Constant(const InputParameters & parameters)
  : Distribution(parameters), _value(getParam<Real>("value"))
{
}

Real
Constant::pdf(const Real & /*x*/) const
{
  return 0.0;
}

Real
Constant::cdf(const Real & /*x*/) const
{
  return 0.0;
}

Real
Constant::quantile(const Real & /*y*/) const
{
  return _value;
}