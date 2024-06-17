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

#include "PerElementParticleInitializer.h"
#include "MooseRandom.h"
#include "ElementSampler.h"

registerMooseObject("FenixApp", PerElementParticleInitializer);

InputParameters
PerElementParticleInitializer::validParams()
{
  auto params = ParticleInitializerBase::validParams();
  params.addClassDescription(
      "Particle initializer that uniformly distributes a specified number of particles per "
      "element and calculates the corresponding particle weight based on the requested number "
      "density, particles per element, and the elements \"volume\".");
  params.addRangeCheckedParam<unsigned int>(
      "particles_per_element",
      "particles_per_element != 0",
      "The number of computational particles that should be placed in each element");

  params.addRangeCheckedParam<Real>("number_density",
                                    "number_density > 0.0",
                                    "The number density of particles you want to represent");
  return params;
}

PerElementParticleInitializer::PerElementParticleInitializer(const InputParameters & parameters)
  : ParticleInitializerBase(parameters),
    _number_density(getParam<Real>("number_density")),
    _particles_per_element(getParam<unsigned int>("particles_per_element"))
{
}

std::vector<InitialParticleData>
PerElementParticleInitializer::getParticleData() const
{

  // counting the number of elements this process is responsible for
  // this will allow us to allocated data structures of the appropriate length
  const auto & elem_range = *_fe_problem.mesh().getActiveLocalElementRange();
  const auto num_local_elements = std::distance(elem_range.begin(), elem_range.end());
  // if there are no elements for this processor: do nothing
  if (num_local_elements == 0)
    return {};

  std::vector<InitialParticleData> data =
      std::vector<InitialParticleData>(num_local_elements * _particles_per_element);

  // setting up this to be able to map from reference elements to the physical elements
  ArbitraryQuadrature arbitrary_qrule = ArbitraryQuadrature(_mesh_dimension, FIRST);
  FEType fe_type = FEType(CONSTANT, MONOMIAL);
  std::unique_ptr<FEBase> fe = FEBase::build(_mesh_dimension, fe_type);
  fe->attach_quadrature_rule(&arbitrary_qrule);
  fe->get_xyz();
  // random number generator to be reseeded on each element
  // this only enables parallel consistency if the element ids are consistent across processes
  MooseRandom generator;

  // so long as p is a point sampled from a cube with dimensions
  // [-1, 1] x [-1, 1] x [-1, 1]
  // this lambda will put the point inside of the libmesh reference pyramid
  auto put_particle_in_pyramid = [](Point & p)
  {
    // if the point is not in the pyramids along the x axis
    // we need to find which one it is in
    if (!((p(0) - std::abs(p(2)) < 0.0) && (-p(0) - std::abs(p(2)) < 0.0) &&
          (p(1) - std::abs(p(2)) < 0.0) && (-p(1) - std::abs(p(2)) < 0.0)))
    {
      // checking out the pyramids with the
      // the x axis going through the center
      if (((p(0) - p(1) > 0.0) && (-p(0) - p(1) > 0.0)) ||
          ((p(0) + p(1) > 0.0) && (-p(0) + p(1) > 0.0)))
      {
        auto temporary = p(1);
        p(1) = p(2);
        p(2) = temporary;
      }
      // all the rest of the points are pyramids with the
      // the y axis going through the center
      else
      {
        auto temporary = p(0);
        p(0) = p(1);
        p(1) = p(2);
        p(2) = temporary;
      }
    }
    // now we have all of the pyramids oriented
    // along the x axis but we need to put them in
    // the correct orientation to match the reference element
    if (p(2) > 0)
      p(2) = 1 - p(2);
    else
      p(2) = 1 + p(2);
  };

  unsigned int elem_count = 0;
  unsigned int particle_index;

  MooseRandom generator;
  FENIX::ElementSampler sampler = FENIX::ElementSampler(_fe_problem, _seed, generator);
  // This will store the uniformly distributed points within the reference elements
  for (const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {

    // now that all of the particle locations have been placed we need to
    // set up the data they will need to be made into actual rays
    const auto & physical_points = sampler.sampleElement(elem, _particles_per_element);
    Real weight = _number_density * elem->volume() / (_particles_per_element);
    for (const auto i : make_range(_particles_per_element))
    {
      particle_index = elem_count * _particles_per_element + i;
      data[particle_index].elem = elem;
      data[particle_index].weight = weight;
      data[particle_index].species = _species;
      data[particle_index].mass = _mass;
      data[particle_index].charge = _charge;
      data[particle_index].position = physical_points[i];
      data[particle_index].velocity = Point(_velocity_distributions[0]->quantile(generator.rand()),
                                            _velocity_distributions[1]->quantile(generator.rand()),
                                            _velocity_distributions[2]->quantile(generator.rand()));
    }
    elem_count++;
  }
  return data;
}
