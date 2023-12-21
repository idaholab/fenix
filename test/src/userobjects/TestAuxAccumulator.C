//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TestAuxAccumulator.h"

#include "AuxAccumulator.h"
#include "MooseMesh.h"

registerMooseObject("FenixTestApp", TestAuxAccumulator);

InputParameters
TestAuxAccumulator::validParams()
{
  auto params = GeneralUserObject::validParams();
  params.addParam<AuxVariableName>("variable", "The variable to accumulate into");
  params.addParam<bool>("skip_finalize", false, "Set to test skipping finalize");
  return params;
}

TestAuxAccumulator::TestAuxAccumulator(const InputParameters & params) : GeneralUserObject(params)
{
}

void
TestAuxAccumulator::execute()
{
  FENIX::AuxAccumulator accumulator(_fe_problem, getParam<AuxVariableName>("variable"));

  for (const auto & elem : _fe_problem.mesh().getMesh().active_local_element_ptr_range())
  {
    const auto add = [&elem, &accumulator](const Point & point)
    { return accumulator.add(*elem, point, point(0) + 2 * point(1) + 3 * point(2)); };

    const auto centroid = elem->vertex_average();

    // At the centroid
    add(centroid);

    for (const auto n : elem->node_index_range())
    {
      const auto & node = elem->point(n);

      // At each node
      add(node);

      // Towards the centroid from each node
      add(0.5 * (centroid + node));
    }
  }

  if (!getParam<bool>("skip_finalize"))
    accumulator.finalize();
}
