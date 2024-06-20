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

#pragma once

#include "GeneralUserObject.h"

#include "VariableSampler.h"

class Ray;

class ParticleStepperBase : public GeneralUserObject
{
public:
  ParticleStepperBase(const InputParameters & parameters);

  static InputParameters validParams();
  /**
   * Unused methods
   */
  ///@{
  virtual void initialize() override final {}
  virtual void finalize() override final {}
  virtual void execute() override final {}
  ///@}

  /**
   * Method available for override to define the rules by which particles' velocity
   * is updated. The particles' velocity data should be updated in this function.
   * @param ray the ray whose velocity we want to set
   * @param v the current velocity of the ray and where the updated ray velocity will be stored
   * @param distance the distance the ray traveled before using this method
   */
  virtual void
  setupStep(Ray & ray, Point & v, const Real q_m_ratio, const Real distance = 0) const = 0;

protected:
  /**
   * Method for a simple dimension-dependent update of the rays max distance and
   * direction based on the dimension of the problem
   * for a 1D problem only the x value of the velocity stored ray data will be used
   * for a 2D problem only the x and y values of the velocity store in ray data will be used
   * for a 3D problem all components of the velocity will be used
   * @param ray the ray whose velocity is being set
   * @param v the new ray velocity
   * @param dt the time step used to set the maximum distance
   */
  virtual void setMaxDistanceAndDirection(Ray & ray, const Point & v, const Real dt) const final;

  /**
   * Used for sampling each component of a finite element field variable
   * The field will be sampled at the point of the ray passed to the method
   * @param field_samplers the sampler utilities set up to sample each component of the force field
   * @param ray the current ray
   * @return the value of the force field at the location of the ray
   */
  Point sampleField(const std::vector<FENIX::VariableSampler> & field_samplers,
                    const Ray & ray) const;

  /**
   * Calculates the updated velocity of a particle subject to force F that applies acceleration
   * to the particle in a linear fashion i.e. v_new = v_current + q/m * F * dt
   * @param v the current velocity of the particle
   * @param F the value of the field at the point the particle exists
   * @param q_m_ratio the charge to mass ratio of the current particle
   * @param dt the time step
   */
  Point linearImpulse(const Point & v, const Point & F, const Real q_m_ratio, const Real dt) const;
  /// the dimension of the mesh being using used for dimension-dependent velocity updates
  const Real _mesh_dimension;
};
