//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PICStudyBase.h"

#include "ClaimRays.h"
#include "Function.h"

registerMooseObject("FenixApp", PICRayStudy);

InputParameters
PICRayStudy::validParams()
{
  auto params = RayTracingStudy::validParams();

  params.addRequiredParam<std::vector<Point>>("start_points",
                                              "The point(s) where the ray(s) start");
  params.addRequiredParam<std::vector<Point>>(
      "start_velocities",
      "The direction(s) that the ray(s) start in (does not need to be normalized)");
  // We're not going to use registration because we don't care to name our rays because
  // we will have a lot of them
  params.set<bool>("_use_ray_registration") = false;

  return params;
}

PICRayStudy::PICRayStudy(const InputParameters & parameters)
  : RayTracingStudy(parameters),
    _v_x_index(registerRayData("v_x")),
    _v_y_index(registerRayData("v_y")),
    _v_z_index(registerRayData("v_z")),
    _banked_rays(
        declareRestartableDataWithContext<std::vector<std::shared_ptr<Ray>>>("_banked_rays", this)),
    _start_points(getParam<std::vector<Point>>("start_points")),
    _start_velocities(getParam<std::vector<Point>>("start_velocities")),
    _has_generated(declareRestartableData<bool>("has_generated", false))
{
  if (_start_points.size() != _start_velocities.size())
    paramError("start_velocities", "Must be the same size as 'start_points'");
}

void
PICRayStudy::generateRays()
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
    }

    // Claim the rays
    std::vector<std::shared_ptr<Ray>> claimed_rays;
    ClaimRays claim_rays(*this, rays, claimed_rays, false);
    claim_rays.claim();
    // ...and then add them to be traced
    moveRaysToBuffer(claimed_rays);
  }
  // Rays are in the bank: reset them
  else
  {
    // Reset each ray
    for (auto it = _banked_rays.begin(); _banked_rays.size() != 0 && it != _banked_rays.end(); ++it)
    {
      auto ray = *it;
      // Store off the ray's info before we reset it
      const auto start_point = ray->currentPoint();

      const auto elem = ray->currentElem();

      // Reset it (this is required to reuse a ray)
      ray->resetCounters();
      ray->clearStartingInfo();

      // And set the new starting information
      ray->setStart(start_point, elem);
      // setDirectionAndMaxDistance(*ray);
    }
  }
  _has_generated = true;
}

void
PICRayStudy::postExecuteStudy()
{
  // Copy the rays that are banked in the study into our own bank
  _banked_rays = rayBank();
}
