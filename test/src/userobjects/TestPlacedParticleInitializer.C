//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TestPlacedParticleInitializer.h"

registerMooseObject("FenixTestApp", TestPlacedParticleInitializer);

InputParameters
TestPlacedParticleInitializer::validParams()
{
  auto params = ParticleInitializerBase::validParams();
  params.addRequiredParam<std::vector<Point>>("start_points",
                                              "The point(s) where the ray(s) start");
  params.addRequiredParam<std::vector<Point>>(
      "start_velocities",
      "The direction(s) that the ray(s) start in (does not need to be normalized)");
  params.addParam<Real>("mass", 1, "The mass of the particles used for a test");
  params.addParam<Real>("charge", 1, "The charge of the particles used for a test");
  params.addParam<Real>(
      "weight", 0, "The number of physical particles a computational particle represents");
  // these are unused in this class but the base class checks the size of this to make sure it is 3
  params.setParameters<std::vector<DistributionName>>("velocity_distributions",
                                                      std::vector<DistributionName>{"a", "a", "a"});
  return params;
}

TestPlacedParticleInitializer::TestPlacedParticleInitializer(const InputParameters & parameters)
  : ParticleInitializerBase(parameters),
    _start_points(getParam<std::vector<Point>>("start_points")),
    _start_velocities(getParam<std::vector<Point>>("start_velocities")),
    _mass(getParam<Real>("mass")),
    _charge(getParam<Real>("charge")),
    _weight(getParam<Real>("weight"))
{
  if (_start_points.size() != _start_velocities.size())
    paramError("start_velocities", "Must be the same size as 'start_points'");
}


std::vector<InitialParticleData>
TestPlacedParticleInitializer::getParticleData() const
{

  std::vector<InitialParticleData> data = std::vector<InitialParticleData>(_start_points.size());

  for (unsigned int i = 0; i < _start_points.size(); ++i)
  {
    data[i].position = _start_points[i];
    data[i].velocity = _start_velocities[i];
    data[i].mass = _mass;
    data[i].charge = _charge;
    data[i].weight = _weight;
    data[i].elem = nullptr;
    data[i].species = "";
  }

  return data;
}
