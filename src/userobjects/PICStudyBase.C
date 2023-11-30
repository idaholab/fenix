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

registerMooseObject("FenixApp", PICStudyBase);

InputParameters
PICStudyBase::validParams()
{
  auto params = RayTracingStudy::validParams();
  params.addClassDescription("Base class for PIC studies. Base use object only registers the ray "
                             "data needed and provides a basic postnexecute method");
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
    _direction_set_index(registerRayData("direction_set")),
    _has_generated(declareRestartableData<bool>("has_generated", false))
{
}

void
PICStudyBase::generateRays()
{
}

void
PICStudyBase::postExecuteStudy()
{
  // we are going to be re using the same rays which just took a step so
  // we store them here to re use them on the next trace
  _banked_rays = rayBank();
}
