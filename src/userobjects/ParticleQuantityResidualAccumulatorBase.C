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

#include "ParticleQuantityResidualAccumulatorBase.h"
#include "PICStudyBase.h"

InputParameters
ParticleQuantityResidualAccumulatorBase::validParams()
{
  auto params = GeneralUserObject::validParams();
  params.addClassDescription(
      "Accumulator used to evaluate the inner product of the particle number density and the test function.");
  params.addRequiredParam<UserObjectName>("study", "The PICStudy that owns the charged particles");
  // These parameters are necessary when using ResidualAccumulator
  params += TaggingInterface::validParams();
  // This exec flag is necessary for the ParticleQuantityResidualAccumulatorBase to contribute to residuals
  ExecFlagEnum & exec_enum = params.set<ExecFlagEnum>("execute_on", true);
  exec_enum.addAvailableFlags(EXEC_PRE_KERNELS);
  params.set<ExecFlagEnum>("execute_on") = EXEC_PRE_KERNELS;
  // making this input parameter private so the user cannot use the object incorrectly
  params.suppressParameter<ExecFlagEnum>("execute_on");
  params.addRequiredParam<NonlinearVariableName>("variable",
                                                 "The variable to contribute to the residual of");
  return params;
}

ParticleQuantityResidualAccumulatorBase::ParticleQuantityResidualAccumulatorBase(const InputParameters & params)
  : GeneralUserObject(params),
    _var_name(getParam<NonlinearVariableName>("variable")),
    _study(getUserObject<PICStudyBase>("study")),
    _v_x_index(_study.getRayDataIndex("v_x")),
    _v_y_index(_study.getRayDataIndex("v_y")),
    _v_z_index(_study.getRayDataIndex("v_z")),
    _weight_index(_study.getRayDataIndex("weight")),
    _charge_index(_study.getRayDataIndex("charge")),
    _mass_index(_study.getRayDataIndex("mass")),
    _species_index(_study.getRayDataIndex("species"))
{
}
