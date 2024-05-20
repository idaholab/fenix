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

#include "PICStudyBase.h"

class InitializerBase;
/**
 * Test study for generating rays for a basic particle-in-cell capability,
 * where Rays have propagate a bit each time step
 */
class InitializedPICStudy : public PICStudyBase
{
public:
  InitializedPICStudy(const InputParameters & parameters);

  static InputParameters validParams();

protected:

  virtual RayID generateUniqueRayID(const THREAD_ID tid) override;

  virtual void initializeParticles() override final;

  const InitializerBase & _initializer;
  const unsigned int _particles_per_element;

  dof_id_type _curr_elem_id;
  unsigned int _curr_elem_ray_count;

};
