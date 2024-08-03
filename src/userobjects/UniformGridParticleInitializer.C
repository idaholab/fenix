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
  params.addClassDescription("Particle initializer that places particles along a line with "
                             "approximate uniform spacing between particles");
  params.addRangeCheckedParam<unsigned int>(
      "total_particles",
      "total_particles != 0",
      "The number of computational particles that should be placed along the line");
  params.addRangeCheckedParam<Real>("number_density",
                                    "number_density > 0.0",
                                    "The number density to represent with computational particles");

  return params;
}

UniformGridParticleInitializer::UniformGridParticleInitializer(const InputParameters & parameters)
  : ParticleInitializerBase(parameters),
    _number_density(getParam<Real>("number_density")),
    _total_particles(getParam<unsigned int>("total_particles"))
{
  if (_mesh_dimension != 1)
    mooseError("The simulation must be in 1D in order to use the UniformGridParticleInitializer");
}

std::vector<InitialParticleData>
UniformGridParticleInitializer::getParticleData() const
{
  Real local_xmin = std::numeric_limits<float>::max();
  Real global_xmax = std::numeric_limits<float>::lowest();
  Real local_volume = 0;
  for (const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    local_volume += elem->volume();
    for (const auto & node : elem->node_ref_range())
    {
      if (node(0) < local_xmin)
        local_xmin = node(0);

      if (node(0) > global_xmax)
        global_xmax = node(0);
    }
  }
  Real global_volume = local_volume;
  Real global_xmin = local_xmin;

  comm().sum(global_volume);
  comm().min(global_xmin);
  comm().max(global_xmax);

  double fraction = local_volume / global_volume;
  double min_frac = fraction;
  double max_frac = fraction;

  comm().min(min_frac);
  comm().max(max_frac);

  // Doing some rounding here to help reduce the cases where the total number of requested particles
  // does not match the total number to be created.
  // Without this rounding, even in cases where the total number of particles requested is divided
  // evenly by the number of processors, the number of particles created does not match the
  // requested number.
  uint local_particle_count = std::round(double(_total_particles) * local_volume / global_volume);
  uint global_particle_count = local_particle_count;

  comm().sum(global_particle_count);

  if (global_particle_count != _total_particles)
  {
    std::ostringstream oss;
    oss << _total_particles << " particles across " << comm().size() << " processes were requested."
        << std::endl;
    oss << "But " << global_particle_count << " will be created because of the mesh partition.";
    mooseWarning(oss.str());
  }

  std::vector<InitialParticleData> data = std::vector<InitialParticleData>(local_particle_count);

  Real dx = (global_xmax - global_xmin) / (_total_particles);

  MooseRandom generator;
  generator.seed(_seed);

  uint particle_count = 0;
  Point curr_point = Point((particle_count + 0.5) * dx + local_xmin);
  for (const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    // the particles that are currently in the element
    auto particle_idxs = std::vector<uint>();
    while (elem->contains_point(curr_point) && particle_count < local_particle_count)
    {
      particle_idxs.push_back(particle_count);
      data[particle_count].elem = elem;
      data[particle_count].species = _species;
      data[particle_count].mass = _mass;
      data[particle_count].charge = _charge;
      data[particle_count].position = curr_point;
      data[particle_count].velocity = Point();
      for (const auto j : make_range(uint(3)))
        data[particle_count].velocity(j) = _velocity_distributions[j]->quantile(generator.rand());
      particle_count++;
      curr_point = Point((particle_count + 0.5) * dx + local_xmin);
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
