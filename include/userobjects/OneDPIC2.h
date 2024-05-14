//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "GeneralUserObject.h"



class OneDPIC2 : public GeneralUserObject
{
public:
  OneDPIC2(const InputParameters & parameters);

  static InputParameters validParams();
  virtual void initialSetup() override;
  /**
   * Unused methods
   */
  ///@{
  virtual void initialize() override final {}
  virtual void finalize() override final {}
  virtual void execute() override;
  ///@}

protected:
  const ConstElemRange & _range;
  const int _num_local_elements;
  int _max_num_local_elements;
  Sampler * _sampler;
};
