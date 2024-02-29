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

#include "libmesh/fe.h"
#include "libmesh/fe_map.h"

class FEProblemBase;
class AuxiliarySystem;
template <typename T>
class MooseVariableFE;

namespace FENIX
{
/**
 * Utility that accumulates values into an aux variable
 */
class AuxAccumulator : public AccumulatorBase
{
public:
  AuxAccumulator(FEProblemBase & problem, const AuxVariableName & variable);

  virtual void add(const Elem & elem, const Point & point, const Real & value) override final;
  virtual void finalize() override final;

private:
  virtual void addCachedValues() override final;
  virtual void initCachedValues() override final;

  /// The mesh dimension
  const unsigned int _dim;

  /// The aux system
  AuxiliarySystem & _aux;

  /// The AuxVariable this AuxRayKernel contributes to
  const MooseVariableFE<Real> & _var;

  /// The FE object for getting phi
  const std::unique_ptr<FEGenericBase<Real>> _fe;
  /// The FEMap object for getting physical to master points
  const std::unique_ptr<FEMap> _fe_map;

  /// The current element accumulation; used for optimization
  std::vector<Real> _current_accumulation;
};

}
