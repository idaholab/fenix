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

#include "MooseTypes.h"

class FEProblemBase;

namespace FENIX
{
/**
 * Base class for a utility that accumulates point values into a variable
 */
class AccumulatorBase
{
public:
  AccumulatorBase(FEProblemBase & problem);
  virtual ~AccumulatorBase();

  /**
   * Accumulates a value into the field
   *
   * @param elem The element
   * @param point The point at which to add the value (should be in \p elem)
   * @param value The value at the point
   */
  virtual void add(const Elem & elem, const Point & point, const Real & value) = 0;

  /**
   * Finalizes the accumulation; must be called once complete
   */
  virtual void finalize();

protected:
  /**
   * @return The current element
   */
  const Elem & currentElem() const;

  /**
   * Prepares for accumulation; should be called in add()
   */
  void prepare(const Elem & elem);

  /**
   * Adds the cached values for the current element
   */
  virtual void addCachedValues() = 0;

  /**
   * Initializes the cached values for the given element
   */
  virtual void initCachedValues() = 0;

  /// The FE problem
  FEProblemBase & _problem;

  /// The thread id
  const THREAD_ID _tid = 0;

private:
  /// The current element we're working on; used for optimization
  const Elem * _current_elem;

  /// Whether or not finalize was called
  bool _finalized;
};

}
