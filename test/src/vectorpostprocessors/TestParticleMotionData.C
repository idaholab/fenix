//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "TestParticleMotionData.h"
#include "PICStudyBase.h"

registerMooseObject("FenixTestApp", TestParticleMotionData);

InputParameters
TestParticleMotionData::validParams()
{
  InputParameters params = GeneralVectorPostprocessor::validParams();
  params.addClassDescription("Collect that value of ray data on every time step");
  params.addRequiredParam<UserObjectName>("study", "The RayTracingStudy that owns the Ray");
  return params;
}

TestParticleMotionData::TestParticleMotionData(const InputParameters & parameters)
  : GeneralVectorPostprocessor(parameters),
    _study(getUserObject<PICStudyBase>("study")),
    _v_x_index(_study.getRayDataIndex("v_x")),
    _v_y_index(_study.getRayDataIndex("v_y")),
    _v_z_index(_study.getRayDataIndex("v_z")),
    _t_pos(declareVector("t_pos")),
    _x(declareVector("x")),
    _y(declareVector("y")),
    _z(declareVector("z")),
    _t_vel(declareVector("t_vel")),
    _v_x(declareVector("v_x")),
    _v_y(declareVector("v_y")),
    _v_z(declareVector("v_z")),
    _id(declareVector("id"))
{
}

void
TestParticleMotionData::initialize()
{
}

void
TestParticleMotionData::execute()
{
  _t_pos.clear();
  _x.clear();
  _y.clear();
  _z.clear();
  _t_vel.clear();
  _v_x.clear();
  _v_y.clear();
  _v_z.clear();
  _id.clear();

  const auto rays = _study.getBankedRays();
  for (auto it = rays.begin(); rays.size() != 0 && it != rays.end(); ++it)
  {
    auto ray = *it;
    _t_pos.push_back(_t);
    _x.push_back(ray->currentPoint()(0));
    _y.push_back(ray->currentPoint()(1));
    _z.push_back(ray->currentPoint()(2));
    _t_vel.push_back(_t - _dt / 2);
    _v_x.push_back(ray->data(_v_x_index));
    _v_y.push_back(ray->data(_v_y_index));
    _v_z.push_back(ray->data(_v_z_index));
    _id.push_back(ray->id());
  }
}

void
TestParticleMotionData::finalize()
{
  comm().gather(0, _t_pos);
  comm().gather(0, _x);
  comm().gather(0, _y);
  comm().gather(0, _z);
  comm().gather(0, _t_vel);
  comm().gather(0, _v_x);
  comm().gather(0, _v_y);
  comm().gather(0, _v_z);
  comm().gather(0, _id);

  std::vector<size_t> indicies;
  indicies.resize(_id.size());
  std::iota(indicies.begin(), indicies.end(), 0);
  std::sort(indicies.begin(),
            indicies.end(),
            [&](size_t a, size_t b) -> bool
            {
              return _id[a] < _id[b];
            });

  Moose::applyIndices(_t_pos, indicies);
  Moose::applyIndices(_x, indicies);
  Moose::applyIndices(_y, indicies);
  Moose::applyIndices(_z, indicies);
  Moose::applyIndices(_t_vel, indicies);
  Moose::applyIndices(_v_x, indicies);
  Moose::applyIndices(_v_y, indicies);
  Moose::applyIndices(_v_z, indicies);
  Moose::applyIndices(_id, indicies);
}
