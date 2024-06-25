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

#include "ElementSampler.h"
#include "MooseRandom.h"
#include "libmesh/enum_to_string.h"
#include "FEProblemBase.h"

namespace FENIX
{

ElementSampler::ElementSampler(FEProblemBase & problem,
                               const unsigned int seed,
                               MooseRandom & generator)
  : _seed(seed),
    _generator(generator),
    _arbitrary_qrule(problem.mesh().dimension(), FIRST),
    _fe(FEBase::build(problem.mesh().dimension(), FEType(CONSTANT, MONOMIAL)))
{
  _fe->attach_quadrature_rule(&_arbitrary_qrule);
  _fe->get_xyz();
}

std::vector<Point>
ElementSampler::sampleElement(const Elem * elem, const unsigned int samples)
{
  mooseAssert(samples != 0, "You must request a non-zero number of samples");
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

  std::vector<Point> reference_points = std::vector<Point>(samples);

  // reseeding with the element id with an additional
  // user selected seed for consistency
  // note that this is only consistent across process counts when element ids are
  // also consistent across processor counts which in general is not the case
  _generator.seed(elem->id() + _seed);

  switch (elem->type())
  {
    // 1D reference elements x = [-1, 1]
    case EDGE2:
    {
      for (const auto i : make_range(samples))
        reference_points[i](0) = 2.0 * _generator.rand() - 1.0;
      break;
    }
    // 2D trianglular element where the vertices are at
    // (0,0)  (0,1), (0, 1)
    case TRI3:
    {
      for (const auto i : make_range(samples))
      {
        // sample on a square x = [0, 1] and y = [0, 1]
        reference_points[i](0) = _generator.rand();
        reference_points[i](1) = _generator.rand();
        // if our points are not in the triangle we mirror them into the triangle
        if (reference_points[i](1) > 1 - reference_points[i](0))
        {
          reference_points[i](0) = 1 - reference_points[i](0);
          reference_points[i](1) = 1 - reference_points[i](1);
        }
      }
      break;
    }
    // 2D square reference element where x = [-1, 1] and y = [-1, 1]
    case QUAD4:
    {
      for (const auto i : make_range(samples))
      {
        reference_points[i](0) = 2.0 * _generator.rand() - 1.0;
        reference_points[i](1) = 2.0 * _generator.rand() - 1.0;
      }
      break;
    }
    // 3D cubic basis element where x = [-1, 1] and y = [-1, 1] and z = [-1, 1]
    case HEX8:
    {
      for (const auto i : make_range(samples))
      {
        reference_points[i](0) = 2.0 * _generator.rand() - 1.0;
        reference_points[i](1) = 2.0 * _generator.rand() - 1.0;
        reference_points[i](2) = 2.0 * _generator.rand() - 1.0;
      }
      break;
    }
    case PYRAMID5:
    {
      for (const auto i : make_range(samples))
      {
        reference_points[i](0) = 2.0 * _generator.rand() - 1.0;
        reference_points[i](1) = 2.0 * _generator.rand() - 1.0;
        reference_points[i](2) = 2.0 * _generator.rand() - 1.0;

        put_particle_in_pyramid(reference_points[i]);
      }
      break;
    }
    // 3D elements with all triangular faces with nodes at
    // (0,0,0), (1,0,0), (0,1,0), (0,0,1)
    case TET4:
    {
      Real temporary;
      for (const auto i : make_range(samples))
      {
        reference_points[i](0) = 2.0 * _generator.rand() - 1.0;
        reference_points[i](1) = 2.0 * _generator.rand() - 1.0;
        reference_points[i](2) = 2.0 * _generator.rand() - 1.0;

        // start by folding the cube into a pyramid
        put_particle_in_pyramid(reference_points[i]);

        // now we are going to fold the pyramid into a single tet
        if (reference_points[i](0) < 0.0)
          reference_points[i](0) *= -1;

        if (reference_points[i](1) < 0.0)
          reference_points[i](1) *= -1;

        // at this point we have two tetrahedrons and we need to Translate them over the proper
        // tet
        if (reference_points[i](1) > reference_points[i](0))
        {
          temporary = reference_points[i](0);
          reference_points[i](0) = reference_points[i](1);
          reference_points[i](1) = temporary;
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
      for (const auto i : make_range(samples))
      {
        // sample on a rectangular prism x = [0, 1] and y = [0, 1] z = [-1,1]
        reference_points[i](0) = _generator.rand();
        reference_points[i](1) = _generator.rand();
        reference_points[i](2) = 2 * _generator.rand() - 1;
        // if our points are not in the triangle we mirror them into the prism
        if (reference_points[i](1) > 1 - reference_points[i](0))
        {
          reference_points[i](0) = 1 - reference_points[i](0);
          reference_points[i](1) = 1 - reference_points[i](1);
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

  _arbitrary_qrule.setPoints(reference_points);
  _fe->reinit(elem);
  // now that all of the particle locations have been placed we need to
  // set up the data they will need to be made into actual rays
  const auto physical_points = _fe->get_xyz();

  return physical_points;
}
}
