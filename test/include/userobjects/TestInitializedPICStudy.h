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
 * Test study of initializing rays with an initializer that will
 * give all of the data needed for rays and ensure that results are parallel consistent
 * assuming that element ids do not change as a function of the number processes being
 * used to solve this
 */
class TestInitializedPICStudy : public PICStudyBase
{
public:
  TestInitializedPICStudy(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual RayID generateUniqueRayID(const THREAD_ID tid) override;

  virtual void initializeParticles() override final;
  /// the object that will supply initial data needed for rays
  const InitializerBase & _initializer;
  /// the number of particles that will be placed in each element
  const unsigned int _particles_per_element;
  /// the element id of the element that we are putting rays in
  dof_id_type _curr_elem_id;
  /// the number of rays that we have put into the current element
  unsigned int _curr_elem_ray_count;
};
