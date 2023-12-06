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
#include "Function.h"

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

  return params;
}

TestPICStudyUserDefinedRays::TestPICStudyUserDefinedRays(const InputParameters & parameters)
  : PICStudyBase(parameters),
    _start_points(getParam<std::vector<Point>>("start_points")),
    _start_velocities(getParam<std::vector<Point>>("start_velocities"))
{
  if (_start_points.size() != _start_velocities.size())
    paramError("start_velocities", "Must be the same size as 'start_points'");
}

void
TestPICStudyUserDefinedRays::generateRays()
{
  // We generate rays the first time only, after that we will
  // pull from the bank and update velocities/max distances
  if (!_has_generated)
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
      // lets just give the weight index a dummy value since we don't need it for this test
      rays[i]->data()[_weight_index] = 0;
      _velocity_updater.updateVelocity(*rays[i], getVelocity(*rays[i]), _dt);
    }

    // Claim the rays
    std::vector<std::shared_ptr<Ray>> claimed_rays;
    ClaimRays claim_rays(*this, rays, claimed_rays, false);
    claim_rays.claim();
    // ...and then add them to be traced
    moveRaysToBuffer(claimed_rays);
  }

  PICStudyBase::generateRays();
}
