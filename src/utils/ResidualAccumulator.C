//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "ResidualAccumulator.h"

#include "FEProblemBase.h"
#include "MooseVariableFE.h"

namespace FENIX
{
ResidualAccumulator::ResidualAccumulator(FEProblemBase & problem,
                                         const MooseObject * const object,
                                         const NonlinearVariableName & variable,
                                         const unsigned int nl_sys_num)
  : AccumulatorBase(problem),
    TaggingInterface(object),
    _var(dynamic_cast<MooseVariableField<Real> &>(_problem.getVariable(
        _tid, variable, Moose::VarKindType::VAR_SOLVER, Moose::VarFieldType::VAR_FIELD_STANDARD))),
    _assembly(_problem.assembly(_tid, nl_sys_num)),
    _test(_var.phi())
{
  if (!_problem.currentlyComputingResidual())
    mooseError("ResidualAccumulator: The residual is not currently being computed. You should only "
               "construct this object when it is.");
}

void
ResidualAccumulator::add(const Elem & elem, const Point & point, const Real & value)
{
  prepare(elem);

  _problem.reinitElemPhys(&elem, {point}, _tid);
  _assembly.modifyArbitraryWeights({1});

  for (const auto i : index_range(_test))
    _local_re(i) -= _test[i][0] * value;
}

void
ResidualAccumulator::addCachedValues()
{
  accumulateTaggedLocalResidual();

  _problem.cacheResidual(_tid);
  _problem.addCachedResidual(_tid);
}

void
ResidualAccumulator::initCachedValues()
{
  _problem.prepare(&currentElem(), _tid);
  prepareVectorTag(_assembly, _var.number());
}

}
