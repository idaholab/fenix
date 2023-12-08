//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "PICStudyBase.h"
#include "VelocityUpdaterBase.h"

registerMooseObject("FenixApp", PICStudyBase);

InputParameters
PICStudyBase::validParams()
{
  auto params = RayTracingStudy::validParams();
  params.addClassDescription("Base class for PIC studies. Provides some of the basic ray data "
                             "needed for particle tracking."
                             "Basic logic for resetting rays to be used as particles after the "
                             "original generation is complete is also provided");
  params.addRequiredParam<UserObjectName>(
      "velocity_updater",
      "The VelocityUpdater UserObject that has the rules for how particle"
      "velocities should be updated");
  // We're not going to use registration because we don't care to name our rays because
  // we will have a lot of them
  params.set<bool>("_use_ray_registration") = false;

  return params;
}

PICStudyBase::PICStudyBase(const InputParameters & parameters)
  : RayTracingStudy(parameters),
    _banked_rays(
        declareRestartableDataWithContext<std::vector<std::shared_ptr<Ray>>>("_banked_rays", this)),
    _v_x_index(registerRayData("v_x")),
    _v_y_index(registerRayData("v_y")),
    _v_z_index(registerRayData("v_z")),
    _weight_index(registerRayData("weight")),
    _has_generated(declareRestartableData<bool>("has_generated", false)),
    _velocity_updater(getUserObject<VelocityUpdaterBase>("velocity_updater"))
{
}

void
PICStudyBase::generateRays()
{
  // We generate rays the first time only, after that we will
  // pull from the bank and update velocities/max distances
  if (!_has_generated)
  {
    initializeParticles();
    _has_generated = true;
  }
  else
  {
    reinitializeParticles();
    // Add the rays to be traced
    moveRaysToBuffer(_banked_rays);
    _banked_rays.clear();
  }
}

void
PICStudyBase::reinitializeParticles()
{
  // Reset each ray
  for (auto & ray : _banked_rays)
  {
    // Store off the ray's info before we reset it
    const auto elem = ray->currentElem();
    const auto start_point = ray->currentPoint();

    // Reset it (this is required to reuse a ray)
    ray->resetCounters();
    ray->clearStartingInfo();

    // And set the new starting information
    ray->setStart(start_point, elem);
    _velocity_updater.updateVelocity(*ray, getVelocity(*ray), _dt);
  }
}

void
PICStudyBase::postExecuteStudy()
{
  // we are going to be re using the same rays which just took a step so
  // we store them here to reset them in the generateRays method
  _banked_rays = rayBank();
}

Point
PICStudyBase::getVelocity(const Ray & ray) const
{
  return Point(ray.data()[_v_x_index], ray.data()[_v_y_index], ray.data()[_v_z_index]);
}
