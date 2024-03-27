//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TestPICStudyUserDefinedRays.h"

#include "ClaimRays.h"
#include "ParticleStepperBase.h"

registerMooseObject("FenixTestApp", TestPICStudyUserDefinedRays);

InputParameters
TestPICStudyUserDefinedRays::validParams()
{
  auto params = PICStudyBase::validParams();

  params.addRequiredParam<std::vector<Point>>("start_points",
                                              "The point(s) where the ray(s) start");
  params.addRequiredParam<std::vector<Point>>(
      "start_velocities",
      "The direction(s) that the ray(s) start in (does not need to be normalized)");
  params.addParam<Real>("mass", 0, "The mass of the particles used for during a test");
  params.addParam<Real>("charge", 0, "The charge of the particles used for during a test");
  return params;
}

TestPICStudyUserDefinedRays::TestPICStudyUserDefinedRays(const InputParameters & parameters)
  : PICStudyBase(parameters),
    _start_points(getParam<std::vector<Point>>("start_points")),
    _start_velocities(getParam<std::vector<Point>>("start_velocities")),
    _mass(getParam<Real>("mass")),
    _charge(getParam<Real>("charge"))
{
  if (_start_points.size() != _start_velocities.size())
    paramError("start_velocities", "Must be the same size as 'start_points'");
}

void
TestPICStudyUserDefinedRays::initializeParticles()
{
  // The unclaimed rays that we're going to generate
  // Here we need to "claim" rays because in parallel, we have
  // a list of points but do not know which processor will
  // own the point that that ray starts in. So, we duplicate
  // the rays on all processors and then let one processor pick them.
  // Basically - we fill them here and then pass them to a ClaimRays
  // object to do all of the magic. In a real PIC case, we'll just
  // generate the rays for the local rays that we care about
  // and the claiming probably won't be necessary
  std::vector<std::shared_ptr<Ray>> rays(_start_points.size());
  // Create a ray for each point/velocity triplet
  for (const auto i : index_range(_start_points))
  {
    rays[i] = acquireReplicatedRay();
    rays[i]->setStart(_start_points[i]);
    // saving the inital velocities so we can have 3v (1d or 2d)
    rays[i]->data()[_v_x_index] = _start_velocities[i](0);
    rays[i]->data()[_v_y_index] = _start_velocities[i](1);
    rays[i]->data()[_v_z_index] = _start_velocities[i](2);
    rays[i]->data()[_mass_index] = _mass;
    rays[i]->data()[_charge_index] = _charge;
    // lets set the particle up for its first timestep
  }

  // Claim the rays
  std::vector<std::shared_ptr<Ray>> claimed_rays;
  ClaimRays claim_rays(*this, rays, claimed_rays, false);
  claim_rays.claim();
  // lets loop through the claimed rays and set them up for the step
  // we need to do so because before they are claimed the ray does not
  // know what element it is in
  for (auto & ray : claimed_rays)
  {
    getVelocity(*ray, _temporary_velocity);
    _stepper.setupStep(
        *ray, _temporary_velocity, ray->data()[_charge_index] / ray->data()[_mass_index]);
    setVelocity(*ray, _temporary_velocity);
  }
  // ...and then add them to be traced
  moveRaysToBuffer(claimed_rays);
}
