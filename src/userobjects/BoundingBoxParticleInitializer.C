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

#include "BoundingBoxParticleInitializer.h"
#include "MooseRandom.h"
#include "ElementSampler.h"
#include "Distribution.h"

registerMooseObject("FenixApp", BoundingBoxParticleInitializer);

InputParameters
BoundingBoxParticleInitializer::validParams()
{
  auto params = PerElementParticleInitializer::validParams();
  params.addClassDescription(
      "This initializer performs the same task as [PerElementParticleInitializer.md], however, "
      "only particles that exist within the bounding box are created.");
  params.addRequiredParam<Point>("bottom_left", "The bottom left corner of the bounding box");
  params.addRequiredParam<Point>("top_right", "The top right corner of the bounding box");

  return params;
}

BoundingBoxParticleInitializer::BoundingBoxParticleInitializer(const InputParameters & parameters)
  : PerElementParticleInitializer(parameters),
    _bottom_left(getParam<Point>("bottom_left")),
    _top_right(getParam<Point>("top_right")),
    _planes({{-1, 0, 0, _bottom_left(0)},
             {1, 0, 0, -_top_right(0)},
             {0, -1, 0, _bottom_left(1)},
             {0, 1, 0, -_top_right(1)},
             {0, 0, -1, _bottom_left(2)},
             {0, 0, 1, -_top_right(2)}})
{
  if (_mesh_dimension != Moose::dim)
    mooseWarning(std::to_string(uint(Moose::dim)) +
                 " components are required for libMesh::Point input.\n"
                 "However, your simulation is only " +
                 std::to_string(uint(_mesh_dimension)) +
                 " dimensional.\n"
                 "The extra component" +
                 std::string(_mesh_dimension == uint(2) ? "s" : "") +
                 " of the libMesh::Point input will be ignored.\n");
  for (const auto i : make_range(_mesh_dimension))
  {
    if (_top_right(i) <= _bottom_left(i))
      paramError("top_right",
                 "Component " + std::to_string(uint(i)) + " of 'top_right' is <= component " +
                     std::to_string(uint(i)) + " of 'bottom_left'");
  }
}

std::vector<InitialParticleData>
BoundingBoxParticleInitializer::getParticleData() const
{

  // first collect the elements that we will place particles in
  std::vector<const Elem *> valid_elems;

  for (const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    unsigned int intersection_count = 0;
    unsigned int nodes_within = 0;
    bool elem_added = false;
    // if one of the nodes is in our box or on the edge
    // of the box then we need to add this to the elements we want
    // to place particles in
    for (const auto & node : elem->node_ref_range())
    {
      unsigned int dim_valid = 0;
      // first we can check to see if one node is within the bounding box
      // this is sufficient for 1D
      for (const auto i : make_range(_mesh_dimension))
      {
        if (node(i) >= _bottom_left(i) && node(i) <= _top_right(i))
          dim_valid++;
      }

      if (dim_valid == _mesh_dimension)
      {
        valid_elems.push_back(elem);
        elem_added = true;
        break;
      }

      // if we are in a higher dimension then we need to check for
      // elements which intersect the bounding box but do not have
      // any nodes within the bounding box
      if (_mesh_dimension > 1)
      {
        // checking to see which side of each plane each node is on
        for (const auto i : make_range(2 * _mesh_dimension))
        {
          Real plane_value = 0;
          for (const auto j : make_range(_mesh_dimension))
            plane_value += node(j) * _planes[i][j];

          plane_value += _planes[i][3];

          if (plane_value >= 0)
            nodes_within++;
        }
        if (nodes_within > 1)
          intersection_count++;
      }
    }
    if (elem_added)
      continue;
    // if more than two planes are intersecting the
    // element then we need to put some particles within it as well
    if (intersection_count > 1)
      valid_elems.push_back(elem);
  }

  // if there are no elements for this processor: do nothing
  if (valid_elems.size() == 0)
    return {};

  std::vector<InitialParticleData> data;
  MooseRandom generator;
  // this objects allows us to uniformly sample space in elements
  FENIX::ElementSampler sampler = FENIX::ElementSampler(_fe_problem, _seed, generator);
  for (const auto elem : valid_elems)
  {
    // first sample points in the element like we would if this were a uniform initialization
    // across the whole domain
    const auto & physical_points = sampler.sampleElement(elem, _particles_per_element);
    Real weight = _number_density * elem->volume() / (_particles_per_element);
    for (const auto point : physical_points)
    {
      // we need to check to make sure that every point we have
      // created is actually in the bounding box as well
      unsigned int dim_valid = 0;
      for (const auto i : make_range(_mesh_dimension))
      {
        if (point(i) >= _bottom_left(i) && point(i) <= _top_right(i))
          dim_valid++;
      }
      // if the point is in the box then we can add a particle for this point
      if (dim_valid == _mesh_dimension)
      {
        auto & particle = data.emplace_back();
        particle.elem = elem;
        particle.weight = weight;
        particle.species = _species;
        particle.mass = _mass;
        particle.charge = _charge;
        particle.position = point;
        for (const auto i : make_range(uint(3)))
          particle.velocity(i) = _velocity_distributions[0]->quantile(generator.rand());
      }
    }
  }
  return data;
}
