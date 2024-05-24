//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TestInitializedPICStudy.h"

#include "ClaimRays.h"
#include "InitializerBase.h"
#include "ParticleStepperBase.h"

registerMooseObject("FenixTestApp", TestInitializedPICStudy);

InputParameters
TestInitializedPICStudy::validParams()
{
  auto params = PICStudyBase::validParams();
  params.addRequiredParam<UserObjectName>("initializer",
                                          "The initializer that will place particles");
  params.addRequiredParam<unsigned int>(
      "particles_per_element", "The number of particles that will be placed in each element");

  return params;
}

TestInitializedPICStudy::TestInitializedPICStudy(const InputParameters & parameters)
  : PICStudyBase(parameters),
    _initializer(getUserObject<InitializerBase>("initializer")),
    _particles_per_element(getParam<unsigned int>("particles_per_element"))
{
}

void
TestInitializedPICStudy::initializeParticles()
{
  auto initial_data = _initializer.getParticleData();
  // incase there are no rays on this processor, do nothing
  if (initial_data.size() == 0)
    return;

  std::vector<std::shared_ptr<Ray>> rays(initial_data.size());

  // keeping track of the current element so we can base
  // ray ids off of the element id
  // this makes it parallel consistent if `allow_renumbering = false`
  _curr_elem_id = initial_data[0].elem->id();
  // this counter will be used to incriment the ray id on a per element basis
  _curr_elem_ray_count = 0;
  for (unsigned int i = 0; i < initial_data.size(); ++i)
  {
    // if we have moved onto a new element reset the counter and switch to the new element
    if (_curr_elem_id != initial_data[i].elem->id())
    {
      _curr_elem_id = initial_data[i].elem->id();
      _curr_elem_ray_count = 0;
    }

    rays[i] = acquireRay();
    rays[i]->setStart(initial_data[i].position, initial_data[i].elem);
    rays[i]->data(_v_x_index) = initial_data[i].velocity(0);
    rays[i]->data(_v_y_index) = initial_data[i].velocity(1);
    rays[i]->data(_v_z_index) = initial_data[i].velocity(2);
    rays[i]->data(_mass_index) = initial_data[i].mass;
    rays[i]->data(_weight_index) = initial_data[i].weight;
    rays[i]->data(_charge_index) = initial_data[i].charge;
    getVelocity(*rays[i], _temporary_velocity);
    _stepper.setupStep(
        *rays[i], _temporary_velocity, rays[i]->data(_charge_index) / rays[i]->data(_mass_index));
    setVelocity(*rays[i], _temporary_velocity);
  }

  moveRaysToBuffer(rays);
}

RayID
TestInitializedPICStudy::generateUniqueRayID(const THREAD_ID /*tid*/)
{
  return _curr_elem_id * _particles_per_element + _curr_elem_ray_count++;
}
