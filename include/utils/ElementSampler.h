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

#include "ArbitraryQuadrature.h"
#include "MooseTypes.h"
class MooseRandom;
class FEProblemBase;

namespace FENIX
{
class ElementSampler
{
public:
  ElementSampler(FEProblemBase & problem, const unsigned int seed, MooseRandom & generator);

  /**
   * Uniformly samples space in the provided element
   * @param elem the element you want to sample points within
   * @param samples the number of points to generate within elem
   */
  std::vector<Point> sampleElement(const Elem * elem, const unsigned int samples);

protected:
  /// the seed that will be used in addition to the element id when reseeding the generator
  const unsigned int _seed;
  /// the generator that will be used for sampling points
  MooseRandom & _generator;
  /// the quadrature rule that maps points from the reference element to the physical element
  ArbitraryQuadrature _arbitrary_qrule;
  /// the fe object that helps mapping points from the reference element to the physical element
  UniquePtr<FEBase> _fe;
};
}
