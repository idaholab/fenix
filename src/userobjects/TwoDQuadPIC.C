//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TwoDQuadPIC.h"

#include "ClaimRays.h"
#include "ParticleStepperBase.h"
#include "ArbitraryQuadrature.h"
#include "Sampler.h"

registerMooseObject("FenixApp", TwoDQuadPIC);

InputParameters
TwoDQuadPIC::validParams()
{
  auto params = PICStudyBase::validParams();
  params.addParam<Real>("mass", 1, "The mass of the particles used for a test");
  params.addParam<Real>("charge", 1, "The charge of the particles used for a test");
  params.addParam<Real>("charge_density", "The charge density you want to create");
  params.addParam<Real>(
      "particles_per_element", 200, "The number of particles that will be put in each element");
  params.addRequiredParam<SamplerName>("sampler",
                                       "The sample from which to extract distribution data.");

  return params;
}

TwoDQuadPIC::TwoDQuadPIC(const InputParameters & parameters)
  : PICStudyBase(parameters),
    _mass(getParam<Real>("mass")),
    _charge(getParam<Real>("charge")),
    _charge_density(getParam<Real>("charge_density")),
    _particles_per_element(getParam<Real>("particles_per_element")),
    _num_local_elements(0),
    _sampler(nullptr)
{
  //
  for ([[maybe_unused]] const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    _num_local_elements++;
  }



}

void
TwoDQuadPIC::initialSetup()
{
  RayTracingStudy::initialSetup();
  _sampler = &getSampler(getParam<SamplerName>("sampler"));
}

void
TwoDQuadPIC::initializeParticles()
{
  std::vector<std::shared_ptr<Ray>> rays(_num_local_elements * _particles_per_element);

  ArbitraryQuadrature arbitrary_qrule = ArbitraryQuadrature(_mesh.getMesh().mesh_dimension(), FIRST);
  FEType fe_type = FEType(CONSTANT, MONOMIAL);
  UniquePtr<FEBase> fe = FEBase::build(_mesh.getMesh().mesh_dimension(), fe_type);
  fe->attach_quadrature_rule(&arbitrary_qrule);
  fe->get_xyz();

  std::vector<Point> reference_points(_num_local_elements * _particles_per_element);

  unsigned int ray_count = 0;
  for (auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    for (int i = 0; i < _particles_per_element; ++i)
    {
      auto data = _sampler->getNextLocalRow();
      reference_points[i] = Point(data[0], data[1], 0.0);
    }

    std::cout << reference_points[0] << std::endl;

    arbitrary_qrule.setPoints(reference_points);
    fe->reinit(elem);

    const auto & physical_points = fe->get_xyz();
    Real weight = _charge_density * elem->volume() / (_particles_per_element);

    std::cout << physical_points[1] << std::endl;
    for (int i = 0; i < _particles_per_element; ++i)
    {
      rays[ray_count] = acquireRay();
      rays[ray_count]->setStart(physical_points[i], elem);
      rays[ray_count]->data(_v_x_index) = 1e-5;
      rays[ray_count]->data(_v_y_index) = 0.0;
      rays[ray_count]->data(_v_z_index) = 0.0;
      rays[ray_count]->data(_mass_index) = _mass;
      rays[ray_count]->data(_charge_index) = _charge * weight;
      rays[ray_count]->data(_weight_index) = weight;
      ray_count++;
    }
  }

  for (auto & ray : rays)
  {
    getVelocity(*ray, _temporary_velocity);
    _stepper.setupStep(
        *ray, _temporary_velocity, ray->data(_charge_index) / ray->data(_mass_index));
    setVelocity(*ray, _temporary_velocity);
  }
  moveRaysToBuffer(rays);
}
