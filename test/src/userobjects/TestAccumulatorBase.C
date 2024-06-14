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

#include "TestAccumulatorBase.h"

#include "MooseMesh.h"

InputParameters
TestAccumulatorBase::validParams()
{
  auto params = GeneralUserObject::validParams();
  params.addParam<bool>("skip_finalize", false, "Set to test skipping finalize");
  return params;
}

TestAccumulatorBase::TestAccumulatorBase(const InputParameters & params) : GeneralUserObject(params)
{
}

void
TestAccumulatorBase::execute()
{
  auto accumulator = buildAccumulator();

  const auto add = [&accumulator](const Elem & elem, const Point & point)
  { return accumulator->add(elem, point, point(0) + 2 * point(1) + 3 * point(2)); };

  for (const auto & elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    const auto centroid = elem->vertex_average();

    // At the centroid
    add(*elem, centroid);

    // Every node but the first (see next elem loop for why)
    for (const auto n : elem->node_index_range())
      if (n != 0)
        add(*elem, elem->point(n));

    // Towards the centroid from each node
    for (const auto n : elem->node_index_range())
      add(*elem, 0.5 * (centroid + elem->point(n)));
  }

  // Node 0 for every element
  // We do a second loop here so we test the caching capability
  // (calling for the same element out of order, only one per element)
  for (const auto & elem : *_fe_problem.mesh().getActiveLocalElementRange())
    add(*elem, elem->point(0));

  if (!getParam<bool>("skip_finalize"))
    accumulator->finalize();
}
