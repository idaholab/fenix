 //* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ParticlesPerElementInitializer.h"
#include "ArbitraryQuadrature.h"

registerMooseObject("FenixApp", ParticlesPerElementInitializer);

InputParameters
ParticlesPerElementInitializer::validParams()
{
  auto params = InitializerBase::validParams();
  params.addClassDescription(
      "PIC particle initializer that uniformly distributes a specified number of particles per "
      "element and calculates the corrisponding particle weight");
  params.addRequiredParam<unsigned int>(
      "particles_per_element",
      "The number of computational particles that should be placed in each element");
  params.addRequiredParam<std::vector<DistributionName>>(
      "velocity_distributions",
      "The distribution names to be sampled when initializing the velocity of each particle");

  params.addParam<Real>("mass", 1, "The mass of the particles used for a test");
  params.addParam<Real>("charge", 1, "The charge of the particles used for a test");
  params.addRequiredParam<Real>("charge_density", "The charge density you want to initalize to");
  return params;
}

ParticlesPerElementInitializer::ParticlesPerElementInitializer(const InputParameters & parameters)
  : InitializerBase(parameters),
    _mass(getParam<Real>("mass")),
    _charge(getParam<Real>("charge")),
    _charge_density(getParam<Real>("charge_density")),
    _particles_per_element(getParam<unsigned int>("particles_per_element")),
    _distribution_names(getParam<std::vector<DistributionName>>("velocity_distributions"))
{
  if (_particles_per_element <= 0)
    paramError("particles_per_element", "The number of particles in each element must be >= 0");

  if (_charge_density == 0.0)
    paramError("charge_density", "The prodivded charge density cannot be zero.");
}

void
ParticlesPerElementInitializer::initialSetup()
{
  for (const DistributionName & name : _distribution_names)
    _velocity_distributions.push_back(&getDistributionByName(name));

  if (_velocity_distributions.size() != 3)
    paramError("velocity_distributions",
               "You must provide 3 distributions, one for each velocity component.");
}


std::vector<InitialParticleData>
ParticlesPerElementInitializer::getParticleData() const
{

  unsigned int num_local_elements = 0;
  for ([[maybe_unused]] const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    num_local_elements++;
  }

  // in case there are no elements for this processor do nothing
  if (num_local_elements == 0)
    return {};

  std::vector<InitialParticleData> data = std::vector<InitialParticleData>(num_local_elements * _particles_per_element);

  // setting up this to be able to map from reference elemnts to the physical elemnts
  // this only enables parallel consistency if the element ids are consistent across core counts
  ArbitraryQuadrature arbitrary_qrule = ArbitraryQuadrature(_mesh_dimension, FIRST);
  FEType fe_type = FEType(CONSTANT, MONOMIAL);
  UniquePtr<FEBase> fe = FEBase::build(_mesh_dimension, fe_type);
  fe->attach_quadrature_rule(&arbitrary_qrule);
  fe->get_xyz();
  // random number generator to be reseeded on each element
  MooseRandom generator;

  unsigned int elem_count = 0;
  unsigned int particle_index;
  Point p = Point();
  for (const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    Real weight = _charge_density * elem->volume() / _particles_per_element;

    generator.seed(elem->id() + _seed);
    std::vector<Point> reference_points = std::vector<Point>(_particles_per_element);
    switch (elem->type()) {
      // 1D reference elements x = [-1, 1]
      case EDGE2:
      case EDGE3:
      case EDGE4:
      {
        for (unsigned int i = 0; i < _particles_per_element; ++i)
          reference_points[i](0) = 2.0 * generator.rand() - 1.0;
        break;
      }
      // 2D trianglular element where the vertices are at
      // (0,0)  (0,1), (0, 1)
      case TRI3:
      case TRI6:
      {
        for (unsigned int i = 0; i < _particles_per_element; ++i)
        {
          // sample on a square x = [0, 1] and y = [0, 1]
          reference_points[i] = Point(generator.rand(), generator.rand(), 0.0);
          // if our points are not in the triangle we mirror them into the triangle
          if (reference_points[i](1) > 1 - reference_points[i](0))
          {
            Real distance = std::abs(-reference_points[i](0) - reference_points[i](1) + 1) / std::sqrt(2);
            reference_points[i](0) = reference_points[i](0) - 2 * distance / std::sqrt(2);
            reference_points[i](1) = reference_points[i](1) - 2 * distance / std::sqrt(2);
          }
        }
        break;
      }
      // 2D square reference element where x = [-1, 1] and y = [-1, 1]
      case QUAD4:
      case QUAD8:
      case QUAD9:
      {
        for (unsigned int i = 0; i < _particles_per_element; ++i)
        {
          reference_points[i](0) = 2.0 * generator.rand() - 1.0;
          reference_points[i](1) = 2.0 * generator.rand() - 1.0;
        }
        break;
      }
      // 3D elements with all trianglular faces vertices at
      // (0,0,0), (1,0,0), (0,1,0), (0,0,1)
      // case TET4:
      // case TET10:
      // {
      //   break;
      // }
      // 3D cubic basis element where x = [-1, 1] and y = [-1, 1] and z = [-1, 1]
      case HEX8:
      case HEX20:
      case HEX27:
      {
        for (unsigned int i = 0; i < _particles_per_element; ++i)
        {
          reference_points[i](0) = 2.0 * generator.rand() - 1.0;
          reference_points[i](1) = 2.0 * generator.rand() - 1.0;
          reference_points[i](2) = 2.0 * generator.rand() - 1.0;
        }
        break;
      }
      case PYRAMID5:
      {
        for (unsigned int i = 0; i < _particles_per_element; ++i)
        {
          p(0) = 2.0 * generator.rand() - 1.0;
          p(1) = 2.0 * generator.rand() - 1.0;
          p(2) = 2.0 * generator.rand() - 1.0;

          if ((p(0) - std::abs(p(2)) < 0.0) &&
              (-p(0) - std::abs(p(2)) < 0.0) &&
              (p(1) - std::abs(p(2)) < 0.0) &&
              (-p(1) - std::abs(p(2)) < 0.0))
          {
            if (p(2) > 0)
            {
              p(2) = 1 - p(2);
            } else {
              p(2) = 1 + p(2);
            }
          }
          else if (
                  ((p(0) - p(1) > 0.0) &&
                  (-p(0) - p(1) > 0.0)) ||
                  ((p(0) + p(1) > 0.0) &&
                  (-p(0) + p(1) > 0.0)))
          {
            auto temp = p(1);
            p(1) = p(2);
            p(2) = temp;
            if (p(2) > 0)
            {
              p(2) = 1 - p(2);
            }
            else
            {
              p(2) = 1 + p(2);
            }
          } else {
            auto temp = p(0);
            p(0) = p(1);
            p(1) = p(2);
            p(2) = temp;
            if (p(2) > 0)
            {
              p(2) = 1 - p(2);
            }
            else
            {
              p(2) = 1 + p(2);
            }
          }

          reference_points[i] = p;
        }
        break;
      }
      default:
        mooseError("Particle Initialization has not been implemented for ");
        // + libMesh::ElementTypes::name(elem) + " elements");
    }
    // mapping our points from the reference elements to the actual physical elements
    arbitrary_qrule.setPoints(reference_points);
    fe->reinit(elem);

    const auto & physical_points = fe->get_xyz();
    for (unsigned int i = 0; i < _particles_per_element; ++i)
    {
    // for (Point p : physical_points)
    // {
      particle_index = elem_count * _particles_per_element + i;
      // InitialParticleData temp;
      // temp.elem = elem;
      // temp.weight = weight;
      // temp.mass = _mass * weight;
      // temp.charge = _charge * weight;
      // temp.position = p;
      // // temp.position = physical_points[i];
      // temp.velocity = Point(_velocity_distributions[0]->quantile(generator.rand()),
      //                                       _velocity_distributions[1]->quantile(generator.rand()),
      //                                       _velocity_distributions[2]->quantile(generator.rand()));
      // data.push_back(temp);
      data[particle_index].elem = elem;
      data[particle_index].weight = weight;
      data[particle_index].mass = _mass * weight;
      data[particle_index].charge = _charge * weight;
      data[particle_index].position = physical_points[i];
      data[particle_index].velocity = Point(_velocity_distributions[0]->quantile(generator.rand()),
                                            _velocity_distributions[1]->quantile(generator.rand()),
                                            _velocity_distributions[2]->quantile(generator.rand()));

    }
    elem_count++;
  }
  // std::cout << "Particles in elem " << data.size() << std::endl;
  // std::cout << "Particles left " << num_local_elements * _particles_per_element  - data.size() << std::endl;
  return data;
}
