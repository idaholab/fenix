//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "Distribution.h"

/**
 * A class that acts like a distribution but when sampled only returns
 * Constants for testing
 */
class Constant : public Distribution
{
public:
  static InputParameters validParams();

  Constant(const InputParameters & parameters);

  /**
   * Unused and not implemented methods
   */
  ///@{
  virtual Real pdf(const Real & x) const override final;
  virtual Real cdf(const Real & x) const override final;
  ///@}

  /**
   * Called when random sampling this will only ever return the chosen value
   */
  virtual Real quantile(const Real & y) const override final;

protected:
  /// the constant value this distribution will return
  const Real _value;
};
