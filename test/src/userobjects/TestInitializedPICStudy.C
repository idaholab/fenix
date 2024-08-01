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

#include "TestInitializedPICStudy.h"

#include "ClaimRays.h"
#include "ParticleInitializerBase.h"
#include "ParticleStepperBase.h"

registerMooseObject("FenixTestApp", TestInitializedPICStudy);

InputParameters
TestInitializedPICStudy::validParams()
{
  auto params = PICStudyBase::validParams();
  params.addRequiredParam<UserObjectName>("initializer",
                                          "The initializer that will place particles");
  params.addParam<unsigned int>(
      "particles_per_element", 0, "The number of particles that will be placed in each element");

  params.addParam<bool>("use_custom_rayids", true, "Wether or not to use the implemented ray id generation scheme or the default one");

  return params;
}

TestInitializedPICStudy::TestInitializedPICStudy(const InputParameters & parameters)
  : PICStudyBase(parameters),
    _initializer(getUserObject<ParticleInitializerBase>("initializer")),
    _use_custom_id_scheme(getParam<bool>("use_custom_rayids")),
    _particles_per_element(getParam<unsigned int>("particles_per_element")),
    _curr_elem_id(0)
{
  if (_use_custom_id_scheme && _particles_per_element == 0)
    paramError("particles_per_element", "particles per element cannot be zero when using the custom ray id generation scheme");
}

void
TestInitializedPICStudy::initializeParticles()
{
  auto initial_data = _initializer.getParticleData();
  // if there are no rays on this processor: do nothing
  if (initial_data.size() == 0)
    return;

  bool replicated_rays = false;

  if (initial_data[0].elem == nullptr)
    replicated_rays = true;

  std::vector<std::shared_ptr<Ray>> rays(initial_data.size());

  // keeping track of the current element so we can base
  // ray ids off of the element id
  // this makes it parallel consistent if `allow_renumbering = false`
  if (!replicated_rays)
    _curr_elem_id = initial_data[0].elem->id();
  // this counter will be used to increment the ray id on a per element basis
  _curr_elem_ray_count = 0;

  for (unsigned int i = 0; i < initial_data.size(); ++i)
  {
    // if we have moved onto a new element reset the counter and switch to the new element
    if (!replicated_rays)
    {
      if (_curr_elem_id != initial_data[i].elem->id())
      {
        if (!replicated_rays)
          _curr_elem_id = initial_data[i].elem->id();
        _curr_elem_ray_count = 0;
      }
      rays[i] = acquireRay();
    }
    else
    {
      rays[i] = acquireReplicatedRay();
    }

    setInitialParticleData(rays[i], initial_data[i]);

    if (!replicated_rays)
    {
      getVelocity(*rays[i], _temporary_velocity);
      _stepper.setupStep(
          *rays[i], _temporary_velocity, rays[i]->data(_charge_index) / rays[i]->data(_mass_index));
      setVelocity(*rays[i], _temporary_velocity);
    }
  }

  if (!replicated_rays)
  {
    moveRaysToBuffer(rays);
    return;
  }
  // The unclaimed rays that we're going to generate
  // Here we need to "claim" rays because in parallel, we have
  // a list of points but do not know which processor will
  // own the point that that ray starts in. So, we duplicate
  // the rays on all processors and then let one processor pick them.
  // Basically - we fill them here and then pass them to a ClaimRays
  // object to do all of the magic. In a real PIC case, we'll just
  // generate the rays for the local rays that we care about
  // and the claiming probably won't be necessary
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

RayID
TestInitializedPICStudy::generateUniqueRayID(const THREAD_ID tid)
{
  if (_use_custom_id_scheme)
    return _curr_elem_id * _particles_per_element + _curr_elem_ray_count++;
  return RayTracingStudy::generateUniqueRayID(tid);
}
