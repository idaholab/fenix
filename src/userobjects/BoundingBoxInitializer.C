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

#include "BoundingBoxInitializer.h"
#include "MooseRandom.h"
#include "ElementSampler.h"
#include "Distribution.h"

registerMooseObject("FenixApp", BoundingBoxInitializer);

InputParameters
BoundingBoxInitializer::validParams()
{
  auto params = ParticlesPerElementInitializer::validParams();
  params.addClassDescription(
      "This initializer performs the same task as [ParticlesPerElementInitializer.md], however, "
      "only particles that exist within the bounding box are created.");
  params.addParam<Real>("x1", 0.0, "The x coordinate of the lower left-hand corner of the box");
  params.addParam<Real>("y1", 0.0, "The y coordinate of the lower left-hand corner of the box");
  params.addParam<Real>("z1", 0.0, "The z coordinate of the lower left-hand corner of the box");
  params.addParam<Real>("x2", 0.0, "The x coordinate of the upper right-hand corner of the box");
  params.addParam<Real>("y2", 0.0, "The y coordinate of the upper right-hand corner of the box");
  params.addParam<Real>("z2", 0.0, "The z coordinate of the upper right-hand corner of the box");

  return params;
}

BoundingBoxInitializer::BoundingBoxInitializer(const InputParameters & parameters)
  : ParticlesPerElementInitializer(parameters),
    _x1(getParam<Real>("x1")),
    _y1(getParam<Real>("y1")),
    _z1(getParam<Real>("z1")),
    _x2(getParam<Real>("x2")),
    _y2(getParam<Real>("y2")),
    _z2(getParam<Real>("z2")),
    _bottom_left(_x1, _y1, _z1),
    _top_right(_x2, _y2, _z2),
    _planes({{-1, 0, 0, _x1},
             {1, 0, 0, -_x2},
             {0, -1, 0, _y1},
             {0, 1, 0, -_y2},
             {0, 0, -1, _z1},
             {0, 0, 1, -_z2}})
{
  if (_fe_problem.mesh().dimension() == 1 && (isParamSetByUser("y1") || isParamSetByUser("y2")))
  {
    mooseError("Params 'y1' and 'y2' should not be set in 1D simulations");
  }

  if (_fe_problem.mesh().dimension() != 3 && (isParamSetByUser("z1") || isParamSetByUser("z2")))
  {
    mooseError("Params 'z1' and 'z2' should not be set unless you are performing a 3D simulation");
  }

  const std::vector<std::string> bottom_left_strings = {"x1", "y1", "z1"};
  const std::vector<std::string> top_right_strings = {"x2", "y2", "z2"};
  for (const auto i : make_range(_mesh_dimension))
  {
    if (_top_right(i) <= _bottom_left(i))
      paramError(top_right_stirngs[i],
                 top_right_stirngs[i] + " must be greater than " + bottom_left_stirngs[i]);
  }
}

std::vector<InitialParticleData>
BoundingBoxInitializer::getParticleData() const
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
        InitialParticleData temp_data;
        temp_data.elem = elem;
        temp_data.weight = weight;
        temp_data.species = _species;
        temp_data.mass = _mass;
        temp_data.charge = _charge;
        temp_data.position = point;
        temp_data.velocity = Point(_velocity_distributions[0]->quantile(generator.rand()),
                                   _velocity_distributions[1]->quantile(generator.rand()),
                                   _velocity_distributions[2]->quantile(generator.rand()));
        data.push_back(temp_data);
      }
    }
  }
  return data;
}
