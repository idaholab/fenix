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
  /// Ray data for storing velocity components
  ///@{
  const RayDataIndex _v_x_index;
  const RayDataIndex _v_y_index;
  const RayDataIndex _v_z_index;
  ///@}
  /** Ray data fro storing the number of real particles each ray represents */
  const RayDataIndex _weight_index;

  // Whether or not we've generated rays yet (restartable)
  bool & _has_generated;

  // the velocity updater object which we will hold the rules for how our
  // particles velocities are updated
  const VelocityUpdaterBase & _velocity_updater;

  /**
   * Method for getting a rays velocity as a vector
   * Each component is retrieved from ray data and given
   * back to user as a vector to make calculations easier
   * @param ray the ray
   */
  Point getVelocity(const Ray & ray) const;
};
