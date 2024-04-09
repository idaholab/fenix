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



class OneDPIC : public GeneralUserObject
{
public:
  OneDPIC(const InputParameters & parameters);

  static InputParameters validParams();
  /**
   * Unused methods
   */
  ///@{
  virtual void initialize() override final {}
  virtual void finalize() override final {}
  virtual void execute() override;
  ///@}

};
