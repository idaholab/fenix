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

#include "UniformGridParticleInitializer.h"
#include "MooseRandom.h"
#include <limits>

registerMooseObject("FenixApp", UniformGridParticleInitializer);

InputParameters
UniformGridParticleInitializer::validParams()
{
  auto params = ParticleInitializerBase::validParams();
  params.addClassDescription(
      "particle initializer that places particles along a line with uniform spacing between particles");
  params.addRangeCheckedParam<unsigned int>(
      "total_particles",
      "total_particles != 0",
      "The number of computational particles that should be placed along the line");
  params.addRangeCheckedParam<Real>("number_density",
                                    "number_density > 0.0",
                                    "The number density of particles you want to represent");

  return params;
}

UniformGridParticleInitializer::UniformGridParticleInitializer(const InputParameters & parameters)
  : ParticleInitializerBase(parameters),
    _number_density(getParam<Real>("number_density")),
    _total_particles(getParam<unsigned int>("total_particles"))
{
  if (_mesh_dimension != 1)
    mooseError("The simulation must be in 1D in order to the UniformGridParticleInitializer");
}

std::vector<InitialParticleData>
UniformGridParticleInitializer::getParticleData() const
{
  std::vector<InitialParticleData> data = std::vector<InitialParticleData>(_total_particles);

  Real xmin = std::numeric_limits<float>::max();
  Real xmax = std::numeric_limits<float>::lowest();
  for (const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    for (const auto & node : elem->node_ref_range())
    {
      if (node(0) < xmin)
        xmin = node(0);

      if(node(0) > xmax)
        xmax = node(0);
    }
  }

  // std::cout << "Min: " << xmin << std::endl;
  // std::cout << "Max: " << xmax << std::endl;
  // std::cout << "dx: " << (xmax - xmin) / (_total_particles) << std::endl;

  Real dx = (xmax - xmin) / (_total_particles);
  MooseRandom generator;
  generator.seed(_seed);

  uint particle_count = 0;
  Point curr_point = Point((particle_count + 0.5) * dx);
  for (const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    // the particles that are currently in the element
    auto particle_idxs = std::set<uint>();
    while (elem->contains_point(curr_point) && particle_count < _total_particles)
    {
      particle_idxs.insert(particle_count);
      data[particle_count].elem = elem;
      data[particle_count].species = _species;
      data[particle_count].mass = _mass;
      data[particle_count].charge = _charge;
      data[particle_count].position = curr_point;
      data[particle_count].velocity = Point();
      for (const auto j : make_range(uint(3)))
        data[particle_count].velocity(j) = _velocity_distributions[j]->quantile(generator.rand());
      particle_count++;
      curr_point = Point((particle_count + 0.5) * dx);
    }

    for (const auto idx : particle_idxs)
    {
      data[idx].weight = _number_density * elem->volume() / (particle_idxs.size());
    }

    particle_idxs.clear();

    if (particle_count == _total_particles)
      break;
  }

  return data;
}
