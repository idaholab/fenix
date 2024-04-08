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
#include "UniformDistribution.h"

registerMooseObject("FenixApp", OneDPIC);

InputParameters
OneDPIC::validParams()
{
  auto params = PICStudyBase::validParams();

  return params;
}

OneDPIC::OneDPIC(const InputParameters & parameters)
  : PICStudyBase(parameters)
{
  // auto params =
  // _fe_problem->addDistribution(UniformDistribution, "test")
}

void
OneDPIC::initializeParticles()
{
  for (auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    auto nodes = elem->get_nodes();

    auto left_x = (*nodes[0])(0);
    auto right_x = (*nodes[1])(0);

    // std::cout << "Left: " << left_x;
    // std::cout << "  Right: " << right_x << std::endl;
    std::cout << std::endl << std::endl;
  }
}
