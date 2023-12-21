//* This file is part of FENIX: Fusion Energy Integrated Multiphys-X,
//* A multiphysics application for modeling plasma facing components
//* https://github.com/idaholab/fenix
//*
//* FENIX is powered by the MOOSE Framework
//* https://www.mooseframework.inl.gov
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AuxAccumulator.h"

#include "FEProblemBase.h"
#include "AuxiliarySystem.h"
#include "MooseVariableFE.h"
#include "MooseMesh.h"

namespace FENIX
{
AuxAccumulator::AuxAccumulator(FEProblemBase & problem, const AuxVariableName & variable)
  : _problem(problem),
    _dim(_problem.mesh().dimension()),
    _aux(_problem.getAuxiliarySystem()),
    _var(_aux.getFieldVariable<Real>(0, variable)),
    _fe(FEGenericBase<Real>::build(_dim, _var.feType())),
    _fe_map(FEMap::build(_var.feType())),
    _current_elem(nullptr),
    _finalized(false)
{
  _fe->request_phi();
}

AuxAccumulator::~AuxAccumulator()
{
  if (!_finalized)
    mooseError("AuxAccumulator was not finalized");
}

void
AuxAccumulator::add(const Elem & elem, const Point & point, const Real & value)
{
  mooseAssert(!_finalized, "Already finalized");

  const std::vector<Point> master_points = {_fe_map->inverse_map(_dim, &elem, point)};
  _fe->reinit(&elem, &master_points);
  const auto & phi = _fe->get_phi();

  if (_current_elem != &elem)
  {
    if (_current_elem)
      addCachedValues();
    initCachedValues(elem, phi.size());
  }

  mooseAssert(_current_accumulation.size() == phi.size(), "Not sized properly");
  for (const auto i : index_range(phi))
    _current_accumulation[i] += phi[i][0] * value;
}

void
AuxAccumulator::finalize()
{
  mooseAssert(!_finalized, "Already finalized");

  if (_current_elem)
    addCachedValues();
  _aux.solution().close();

  _finalized = true;
}

void
AuxAccumulator::addCachedValues()
{
  mooseAssert(_current_elem, "Element not set");

  std::vector<libMesh::dof_id_type> di;
  _aux.dofMap().dof_indices(_current_elem, di, _var.number());

  mooseAssert(_current_accumulation.size() == di.size(), "Inconsistent size");
  for (const auto i : index_range(di))
    _aux.solution().add(di[i], _current_accumulation[i]);

  _current_elem = nullptr;
  std::fill(_current_accumulation.begin(), _current_accumulation.end(), 0);
}

void
AuxAccumulator::initCachedValues(const Elem & elem, const std::size_t size)
{
  mooseAssert(!_current_elem, "Element currently set");
  mooseAssert(std::find_if(_current_accumulation.begin(),
                           _current_accumulation.end(),
                           [](const auto & val)
                           { return val != 0; }) == _current_accumulation.end(),
              "Values not zeroed");

  _current_elem = &elem;
  _current_accumulation.resize(size, 0);
}

}
