  //* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "OneDPIC2.h"

#include "Sampler.h"

registerMooseObject("FenixApp", OneDPIC2);

InputParameters
OneDPIC2::validParams()
{
  auto params = GeneralUserObject::validParams();
  params.addRequiredParam<SamplerName>("sampler",
                                       "The sample from which to extract distribution data.");
  return params;
}

OneDPIC2::OneDPIC2(const InputParameters & parameters)
  : GeneralUserObject(parameters),
  _range(*_fe_problem.mesh().getActiveLocalElementRange()),
  _num_local_elements(std::distance(_range.begin(), _range.end())),
  _sampler(nullptr)
{
  _max_num_local_elements = _num_local_elements;

  comm().max(_max_num_local_elements);
}

void
OneDPIC2::initialSetup()
{
  _sampler = &getSampler(getParam<SamplerName>("sampler"));
}

void
OneDPIC2::execute()
{

  // if getNextLocalRow is not called the same number of times
  // on every processor the program hangs
  // this makes sure we call it the samen number of times everywhere
  std::vector<Real> data;
  int elem_count = 0;
  for (auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  {
    elem_count++;
    for (int i = 0; i < 10; ++i)
    {
      data = _sampler->getNextLocalRow();
      // if (i >= _num_local_elements)
      //   continue;

      // for (auto v : data)
      // {
      //   std::cout << v << std::endl;
      // }
    }
  }

  for (int i = 0; i < (elem_count - _max_num_local_elements); ++i)
  {
    for (int i = 0; i < 10; ++i)
    {
      data = _sampler->getNextLocalRow();
    }
  }


  //

  // for (auto elem : *_fe_problem.mesh().getActiveLocalElementRange())
  // {
  //   elem_count++;
  //   std::cout << "Element: " << elem_count << std::endl;


  //   for (int i = 0; i < 10; ++i)
  //   {
  //     std::vector<Real> data = _sampler->getNextLocalRow();

  //     for (auto v : data)
  //     {
  //       std::cout << v << std::endl;
  //     }
  //   }

  //   // auto nodes = elem->get_nodes();

  //   // auto left_x = (*nodes[0])(0);
  //   // auto right_x = (*nodes[1])(0);

  //   // std::cout << "Left: " << left_x;
  //   // std::cout << "  Right: " << right_x << std::endl;
  //   // std::cout << std::endl << std::endl;
  // }
}
