//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "OneDPIC.h"

#include "ClaimRays.h"
#include "ParticleStepperBase.h"
#include "Sampler.h"

registerMooseObject("FenixApp", OneDPIC);

InputParameters
OneDPIC::validParams()
{
  auto params = PICStudyBase::validParams();
  params.addParam<Real>("mass", 0, "The mass of the particles used for a test");
  params.addParam<Real>("charge", 0, "The charge of the particles used for a test");
  params.addParam<Real>("particles_per_element", 200, "The number of particles that will be put in each element");
  params.addRequiredParam<SamplerName>("sampler",
                                       "The sample from which to extract distribution data.");

  return params;
}

OneDPIC::OneDPIC(const InputParameters & parameters)
  : PICStudyBase(parameters),
    _mass(getParam<Real>("mass")),
    _charge(getParam<Real>("charge")),
    _particles_per_element(getParam<Real>("particles_per_element")),
    _num_local_elements(0),
    _sampler(nullptr)
{
  for ([[maybe_unused]] const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    _num_local_elements++;
  }
}

void
OneDPIC::initialSetup()
{
  RayTracingStudy::initialSetup();
  _sampler = &getSampler(getParam<SamplerName>("sampler"));
}

void
OneDPIC::initializeParticles()
{
  std::vector<std::shared_ptr<Ray>> rays(_num_local_elements * _particles_per_element);

  unsigned int ray_count = 0;
  for (auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {

    auto nodes = elem->get_nodes();
    auto left_x = (*nodes[0])(0);
    auto right_x = (*nodes[1])(0);

    for (int i = 0; i < _particles_per_element; ++i)
    {
      auto data = _sampler->getNextLocalRow();
      rays[ray_count] = acquireReplicatedRay();
      rays[ray_count]->setStart(Point((right_x - left_x) * data[0] + left_x, 0.0, 0.0));
      rays[ray_count]->data(_v_x_index) = 0.0;
      rays[ray_count]->data(_v_y_index) = 0.0;
      rays[ray_count]->data(_v_z_index) = 0.0;
      rays[ray_count]->data(_mass_index) = _mass;
      rays[ray_count]->data(_charge_index) = _charge;
      ray_count++;
    }
  }

  // Claim the rays
  std::vector<std::shared_ptr<Ray>> claimed_rays;
  ClaimRays claim_rays(*this, rays, claimed_rays,  false);
  claim_rays.claim();

  for (auto & ray : claimed_rays)
  {
    // ray->setStationary();
    getVelocity(*ray, _temporary_velocity);
    _stepper.setupStep(
        *ray, _temporary_velocity, ray->data(_charge_index) / ray->data(_mass_index));
    setVelocity(*ray, _temporary_velocity);
  }
  moveRaysToBuffer(claimed_rays);
}
