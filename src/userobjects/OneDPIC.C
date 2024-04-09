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
#include "Uniform.h"
#include "LatinHypercubeSampler.h"

registerMooseObject("FenixApp", OneDPIC);

InputParameters
OneDPIC::validParams()
{
  auto params = GeneralUserObject::validParams();

  return params;
}

OneDPIC::OneDPIC(const InputParameters & parameters)
  : GeneralUserObject(parameters)
{
  auto dist_params = _factory.getValidParams("Uniform");
  dist_params.setParameters<Real>("lower_bound", 0);
  dist_params.setParameters<Real>("upper_bound", 1);
  _fe_problem.addDistribution("Uniform", "test", dist_params);

  std::vector<DistributionName> names = {"test"};

  auto sampler_params = _factory.getValidParams("LatinHypercube");
  sampler_params.setParameters<dof_id_type>("num_rows", 10);
  sampler_params.setParameters<std::vector<DistributionName>>("distributions", names);
  _fe_problem.addSampler("LatinHypercube", "sampler", sampler_params);
  // _sampler = getSamplerByName("sampler");
}

void
OneDPIC::execute()
{

  Sampler &sampler = getSamplerByName("sampler");
  // auto sampler =
  int elem_count = 0;

  for (auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    elem_count++;
    std::cout << "Element: " << elem_count << std::endl;

    for (dof_id_type i = sampler.getLocalRowBegin(); i < sampler.getLocalRowEnd(); ++i)
    {
      std::vector<Real> data = sampler.getNextLocalRow();

      for (auto v : data)
      {
        std::cout << v << std::endl;
      }
    }

    for (int i = 0; i < 10; ++i)
    {
      auto values = sampler.getNextLocalRow();


    }


    // auto sampler =
    auto nodes = elem->get_nodes();

    auto left_x = (*nodes[0])(0);
    auto right_x = (*nodes[1])(0);

    // std::cout << "Left: " << left_x;
    // std::cout << "  Right: " << right_x << std::endl;
    // std::cout << std::endl << std::endl;
  }
}
