//* This file is part of FENIX: Fusion ENergy Integrated multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
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

#include "VariableSampler.h"

namespace FENIX
{
VariableSampler::VariableSampler(FEProblemBase & problem,
                                 const VariableName & name,
                                 const THREAD_ID tid)
  : _system(problem.getSystem(name)),
    _var_num(
        problem
            .getVariable(
                tid, name, Moose::VarKindType::VAR_ANY, Moose::VarFieldType::VAR_FIELD_STANDARD)
            .number())
{
}

Real
VariableSampler::sampleVariable(const Point & point, const Elem * element) const
{
  return _system.point_value(_var_num, point, element);
}
}
