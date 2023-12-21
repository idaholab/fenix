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

#include "MooseTypes.h"

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
class AuxAccumulator
{
public:
  AuxAccumulator(FEProblemBase & problem, const AuxVariableName & variable);
  ~AuxAccumulator();

  /**
   * Accumulates a value into the aux field
   *
   * @param elem The element
   * @param point The point at which to add the value (should be in \p elem)
   * @param value The value at the point
   */
  void add(const Elem & elem, const Point & point, const Real & value);

  /**
   * Finalizes the accumulation; must be called once complete
   */
  void finalize();

private:
  /**
   * Adds the cached values for the current element
   *
   * This enables not calling DofMap::dof_indices for each point that is added
   * when successive points are in the same element
   */
  void addCachedValues();
  /**
   * Initializes the cached values for the given element
   */
  void initCachedValues(const Elem & elem, const std::size_t size);

  /// The FE problem
  FEProblemBase & _problem;

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

  /// The current element we're working on; used for optimization
  const libMesh::Elem * _current_elem;
  /// The current element accumulation; used for optimization
  std::vector<Real> _current_accumulation;

  /// Whether or not finalize was called
  bool _finalized;
};

}
