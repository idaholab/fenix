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

registerMooseObject("FenixApp", PICStudyBase);

InputParameters
PICStudyBase::validParams()
{
  auto params = RayTracingStudy::validParams();
  params.addClassDescription(
      "Base class for PIC studies. Provides some of the basic ray data needed"
      "And the basic logic for resetting rays after the original generation is complete");
  params.addRequiredParam<UserObjectName>("velocity_updater",
                                          "The RayTracingStudy that owns the Ray");
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
  if (_has_generated)
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
    // Add the rays to be traced
    moveRaysToBuffer(_banked_rays);
    _banked_rays.clear();
  }

  if (!_has_generated)
    _has_generated = true;
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
