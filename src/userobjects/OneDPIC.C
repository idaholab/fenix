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

#include "Sampler.h"

registerMooseObject("FenixApp", OneDPIC);

InputParameters
OneDPIC::validParams()
{
  auto params = GeneralUserObject::validParams();
  params.addRequiredParam<SamplerName>("sampler",
                                       "The sample from which to extract distribution data.");
  return params;
}

OneDPIC::OneDPIC(const InputParameters & parameters)
  : GeneralUserObject(parameters),
  _sampler(getSampler("sampler"))
{
}

void
OneDPIC::execute()
{
  int elem_count = 0;

  for (auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    elem_count++;
    std::cout << "Element: " << elem_count << std::endl;

    for (dof_id_type i = _sampler.getLocalRowBegin(); i < _sampler.getLocalRowEnd(); ++i)
    {
      std::vector<Real> data = _sampler.getNextLocalRow();

      for (auto v : data)
      {
        std::cout << v << std::endl;
      }
    }

    for (int i = 0; i < 10; ++i)
    {
      auto values = _sampler.getNextLocalRow();


    }
    auto nodes = elem->get_nodes();

    auto left_x = (*nodes[0])(0);
    auto right_x = (*nodes[1])(0);

    // std::cout << "Left: " << left_x;
    // std::cout << "  Right: " << right_x << std::endl;
    // std::cout << std::endl << std::endl;
  }
}
