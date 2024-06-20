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

#include "ParticleStepperBase.h"
#include "VariableSampler.h"

class BorisStepper : public ParticleStepperBase
{
public:
  BorisStepper(const InputParameters & parameters);

  static InputParameters validParams();

  /**
   * Electromagnetic particle stepper
   * uses the Boris algorithm
   */
  virtual void setupStep(Ray & ray,
                         Point & v,
                         const Real q_m_ratio = 0,
                         const Real distance = 0) const override final;

private:
  ///@{ Variables containing the variable name for each of the components
  /// of the electric and magnetic fields
  const std::vector<VariableName> _efield_vars;
  const std::vector<VariableName> _bfield_vars;
  ///@}

  ///@{ the samplers used to sample the fields at the point a particle exists
  std::vector<FENIX::VariableSampler> _efield_samplers;
  std::vector<FENIX::VariableSampler> _bfield_samplers;
  ///@}

  /**
   * Rotates the particle throught the magnetic field using the Boris Algorithms method
   * @param v the current particle velocity
   * @param B the magnetic field
   * @param q_m_ratio the charge to mass ratio of the particle
   * @param dt the time step over which the particle is going to be rotated through
   * @returns the particles velocity after rotation through the magnetic field
   */
  Point
  magneticImpulse(const Point & v, const Point & B, const Real q_m_ratio, const Real dt) const;
};
