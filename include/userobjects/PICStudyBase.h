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

#include "Function.h"
#include "ClaimRays.h"
#include "RayTracingStudy.h"
#include "VelocityUpdaterBase.h"
/**
 * Test study for generating rays for a basic particle-in-cell capability,
 * where Rays have propagate a bit each time step
 */
class PICStudyBase : public RayTracingStudy
{
public:
  PICStudyBase(const InputParameters & parameters);

  static InputParameters validParams();

  virtual void generateRays() override;
  // The banked rays to be used on the next timestep (restartable)
  std::vector<std::shared_ptr<Ray>> & _banked_rays;

protected:
  virtual void postExecuteStudy() override;
  /** Ray Data for storing all components of velocity */
  const RayDataIndex _v_x_index;
  const RayDataIndex _v_y_index;
  const RayDataIndex _v_z_index;

  // Whether or not we've generated rays yet (restartable)
  bool & _has_generated;

  const VelocityUpdaterBase & _velocity_updater;

  Point getVelocity(const Ray & ray) const;
};
