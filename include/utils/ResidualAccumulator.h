//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "AccumulatorBase.h"
#include "TaggingInterface.h"

#include "MooseTypes.h"

class Assembly;

namespace FENIX
{
/**
 * Utility that accumulates values into a nonlinear variable
 */
class ResidualAccumulator : public AccumulatorBase, protected TaggingInterface
{
public:
  ResidualAccumulator(FEProblemBase & problem,
                      const MooseObject * const object,
                      const NonlinearVariableName & variable,
                      const unsigned int nl_sys_num);

  virtual void add(const Elem & elem, const Point & point, const Real & value) override final;

private:
  void addCachedValues() override final;
  void initCachedValues() override final;

  /// The nonlinear variable to contribute to
  const MooseVariableField<Real> & _var;

  /// The Assembly object
  Assembly & _assembly;

  /// Test function
  const typename OutputTools<Real>::VariableTestValue & _test;
};

}
