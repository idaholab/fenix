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
#include "libmesh/enum_to_string.h"
#include "MooseRandom.h"
#include "Distribution.h"

registerMooseObject("FenixApp", ParticlesPerElementInitializer);

InputParameters
ParticlesPerElementInitializer::validParams()
{
  auto params = InitializerBase::validParams();
  params.addClassDescription(
      "Particle initializer that uniformly distributes a specified number of particles per "
      "element and calculates the corresponding particle weight based on the requested number "
      "density, particles per element, and the elements \"volume\".");
  params.addRequiredParam<unsigned int>(
      "particles_per_element",
      "The number of computational particles that should be placed in each element");
  params.addRequiredParam<std::vector<DistributionName>>(
      "velocity_distributions",
      "The distribution names to be sampled when initializing the velocity of each particle");

  params.addRequiredParam<Real>("number_density",
                                "The number density of particles you want to represent");
  return params;
}

ParticlesPerElementInitializer::ParticlesPerElementInitializer(const InputParameters & parameters)
  : InitializerBase(parameters),
    _number_density(getParam<Real>("number_density")),
    _particles_per_element(getParam<unsigned int>("particles_per_element")),
    _distribution_names(getParam<std::vector<DistributionName>>("velocity_distributions"))
{
  if (_particles_per_element == 0)
    paramError("particles_per_element", "The number of particles in each element must be > 0.");

  if (_number_density <= 0.0)
    paramError("number_density", "The requested number density must be > 0.");
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

  // counting the number of elements this process is responsible for
  // this will allow us to allocated data structures of the appropriate length
  unsigned int num_local_elements = 0;
  // c++ note [[maybe_unused]] tells the compiler not to give a warning
  // that the variable elem is not used in this loop
  for ([[maybe_unused]] const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
    num_local_elements++;

  // if there are no elements for this processor: do nothing
  if (num_local_elements == 0)
    return {};

  std::vector<InitialParticleData> data =
      std::vector<InitialParticleData>(num_local_elements * _particles_per_element);

  // setting up this to be able to map from reference elements to the physical elements
  ArbitraryQuadrature arbitrary_qrule = ArbitraryQuadrature(_mesh_dimension, FIRST);
  FEType fe_type = FEType(CONSTANT, MONOMIAL);
  std::unique_ptr<FEBase> fe = FEBase::build(_mesh_dimension, fe_type);
  fe->attach_quadrature_rule(&arbitrary_qrule);
  fe->get_xyz();
  // random number generator to be reseeded on each element
  // this only enables parallel consistency if the element ids are consistent across processes
  MooseRandom generator;

  // so long as p is a point sampled from a cube with dimensions
  // [-1, 1] x [-1, 1] x [-1, 1]
  // this lambda will put the point inside of the libmesh reference pyramid
  auto put_particle_in_pyramid = [](Point & p)
  {
    // if the point is not in the pyramids along the x axis
    // we need to find which one it is in
    if (!((p(0) - std::abs(p(2)) < 0.0) && (-p(0) - std::abs(p(2)) < 0.0) &&
          (p(1) - std::abs(p(2)) < 0.0) && (-p(1) - std::abs(p(2)) < 0.0)))
    {
      // checking out the pyramids with the
      // the x axis going through the center
      if (((p(0) - p(1) > 0.0) && (-p(0) - p(1) > 0.0)) ||
          ((p(0) + p(1) > 0.0) && (-p(0) + p(1) > 0.0)))
      {
        auto temporary = p(1);
        p(1) = p(2);
        p(2) = temporary;
      }
      // all the rest of the points are pyramids with the
      // the y axis going through the center
      else
      {
        auto temporary = p(0);
        p(0) = p(1);
        p(1) = p(2);
        p(2) = temporary;
      }
    }
    // now we have all of the pyramids oriented
    // along the x axis but we need to put them in
    // the correct orientation to match the reference element
    if (p(2) > 0)
      p(2) = 1 - p(2);
    else
      p(2) = 1 + p(2);
  };

  unsigned int elem_count = 0;
  unsigned int particle_index;
  // This will store the uniformly distributed points within the reference elements
  std::vector<Point> reference_points = std::vector<Point>(_particles_per_element);
  for (const auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    // reseeding with the element id with an additional
    // user selected seed for consistency
    // note that this is only consistent across process counts when element ids are
    // also consistent across processor counts which in general is not the case
    generator.seed(elem->id() + _seed);
    switch (elem->type())
    {
      // 1D reference elements x = [-1, 1]
      case EDGE2:
      {
        for (unsigned int i = 0; i < _particles_per_element; ++i)
          reference_points[i](0) = 2.0 * generator.rand() - 1.0;
        break;
      }
      // 2D trianglular element where the vertices are at
      // (0,0)  (0,1), (0, 1)
      case TRI3:
      {
        for (unsigned int i = 0; i < _particles_per_element; ++i)
        {
          // sample on a square x = [0, 1] and y = [0, 1]
          reference_points[i](0) = generator.rand();
          reference_points[i](1) = generator.rand();
          // , generator.rand(), 0.0);
          // if our points are not in the triangle we mirror them into the triangle
          if (reference_points[i](1) > 1 - reference_points[i](0))
          {
            Real distance =
                std::abs(-reference_points[i](0) - reference_points[i](1) + 1) / std::sqrt(2);
            reference_points[i](0) = reference_points[i](0) - 2 * distance / std::sqrt(2);
            reference_points[i](1) = reference_points[i](1) - 2 * distance / std::sqrt(2);
          }
        }
        break;
      }
      // 2D square reference element where x = [-1, 1] and y = [-1, 1]
      case QUAD4:
      {
        for (unsigned int i = 0; i < _particles_per_element; ++i)
        {
          reference_points[i](0) = 2.0 * generator.rand() - 1.0;
          reference_points[i](1) = 2.0 * generator.rand() - 1.0;
        }
        break;
      }
      // 3D cubic basis element where x = [-1, 1] and y = [-1, 1] and z = [-1, 1]
      case HEX8:
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
          reference_points[i](0) = 2.0 * generator.rand() - 1.0;
          reference_points[i](1) = 2.0 * generator.rand() - 1.0;
          reference_points[i](2) = 2.0 * generator.rand() - 1.0;

          put_particle_in_pyramid(reference_points[i]);
        }
        break;
      }
      // 3D elements with all triangular faces with nodes at
      // (0,0,0), (1,0,0), (0,1,0), (0,0,1)
      case TET4:
      {
        for (unsigned int i = 0; i < _particles_per_element; ++i)
        {
          reference_points[i](0) = 2.0 * generator.rand() - 1.0;
          reference_points[i](1) = 2.0 * generator.rand() - 1.0;
          reference_points[i](2) = 2.0 * generator.rand() - 1.0;

          // start by folding the cube into a pyramid
          put_particle_in_pyramid(reference_points[i]);

          // now we are going to fold the pyramid into a single tet
          if (reference_points[i](0) < 0.0)
            reference_points[i](0) *= -1;

          if (reference_points[i](1) < 0.0)
            reference_points[i](1) *= -1;

          // at this point we have two tetrahedrons and we need to reflect one of them
          // over the line y = x if they are not in the selected tet
          if (reference_points[i](1) > reference_points[i](0))
          {
            auto distance = (reference_points[i](1) - reference_points[i](0)) / std::sqrt(2);
            reference_points[i](0) += 2 * distance / std::sqrt(2);
            reference_points[i](1) -= 2 * distance / std::sqrt(2);
          }

          // now all of our points are in a tet that is bounded by
          // (0,0,0), (1,0,0), (1,1,0), (0,0,1)
          // to make this our reference tet we perform an affine transformation
          reference_points[i](0) -= reference_points[i](1);
        }
        break;
      }
      case PRISM6:
      {
        for (unsigned int i = 0; i < _particles_per_element; ++i)
        {
          // sample on a rectangular prism x = [0, 1] and y = [0, 1] z = [-1,1]
          reference_points[i](0) = generator.rand();
          reference_points[i](1) = generator.rand();
          reference_points[i](2) = 2 * generator.rand() - 1;
          // if our points are not in the triangle we mirror them into the prism
          if (reference_points[i](1) > 1 - reference_points[i](0))
          {
            Real distance =
                std::abs(-reference_points[i](0) - reference_points[i](1) + 1) / std::sqrt(2);
            reference_points[i](0) = reference_points[i](0) - 2 * distance / std::sqrt(2);
            reference_points[i](1) = reference_points[i](1) - 2 * distance / std::sqrt(2);
          }
        }
        break;
      }
      default:
        mooseError("Particle Initialization has not been implemented for elements of type " +
                   Utility::enum_to_string(elem->type()) + ".\n" +
                   "If your problem requires this element type please reach out to us at\n" +
                   "https://github.com/idaholab/fenix/discussions");
    }
    // mapping our points from the reference elements to the actual physical elements
    arbitrary_qrule.setPoints(reference_points);
    fe->reinit(elem);
    // now that all of the particle locations have been placed we need to
    // set up the data they will need to be made into actual rays
    const auto & physical_points = fe->get_xyz();
    Real weight = _number_density * elem->volume() / (_particles_per_element);
    for (unsigned int i = 0; i < _particles_per_element; ++i)
    {
      particle_index = elem_count * _particles_per_element + i;
      data[particle_index].elem = elem;
      data[particle_index].weight = weight;
      data[particle_index].species = _species;
      data[particle_index].mass = _mass;
      data[particle_index].charge = _charge;
      data[particle_index].position = physical_points[i];
      data[particle_index].velocity = Point(_velocity_distributions[0]->quantile(generator.rand()),
                                            _velocity_distributions[1]->quantile(generator.rand()),
                                            _velocity_distributions[2]->quantile(generator.rand()));
    }
    elem_count++;
  }
  return data;
}
