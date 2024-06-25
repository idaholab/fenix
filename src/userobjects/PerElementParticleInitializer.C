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

  // random number generator to be used for sampling the elements
  MooseRandom generator;
  FENIX::ElementSampler sampler = FENIX::ElementSampler(_fe_problem, _seed, generator);
  // This will store the uniformly distributed points within the reference elements
  uint elem_count = 0;
  for (const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {

    // now that all of the particle locations have been placed we need to
    // set up the data they will need to be made into actual rays
    const auto & physical_points = sampler.sampleElement(elem, _particles_per_element);
    Real weight = _number_density * elem->volume() / (_particles_per_element);
    for (const auto i : make_range(_particles_per_element))
    {
      uint particle_index = elem_count * _particles_per_element + i;
      data[particle_index].elem = elem;
      data[particle_index].weight = weight;
      data[particle_index].species = _species;
      data[particle_index].mass = _mass;
      data[particle_index].charge = _charge;
      data[particle_index].position = physical_points[i];
      data[particle_index].velocity = Point();
      for (const auto i : make_range(uint(3)))
        data[particle_index].velocity(i) = _velocity_distributions[i]->quantile(generator.rand());
    }
    elem_count++;
  }
  return data;
}
