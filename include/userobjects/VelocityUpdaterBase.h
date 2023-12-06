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

// MOOSE Includes
#include "Ray.h"
#include "GeneralUserObject.h"
#include "RayTracingStudy.h"

class VelocityUpdaterBase: public GeneralUserObject
{
public:

  VelocityUpdaterBase(const InputParameters & parameters);

  static InputParameters validParams();
  // setting all of these methods to do nothing since this class will only
  // provide a method to be called by a ray tracing study
  virtual void initialSetup() override {}
  virtual void residualSetup() override {}
  virtual void jacobianSetup() override {}
  virtual void meshChanged() override {}
  virtual void timestepSetup() override {}
  virtual void initialize() override {}
  virtual void finalize() override {}
  virtual void execute() override {}

  /**
   * Method for a simple dimenion dependent update of the rays max distance and
   * velocity based on the dimension of the problem
   * for a 1D problem only the x value of the velocity stored ray data will be used
   * for a 2D problem only the x and y values of the velocity store in ray data will be used
   * for a 3D problem all components of the velocity will be used
   * @param ray the ray whose velocity we want to set
   * @param v a vector which is intended to be either the new velocity vector or the current velocity vector for the ray
   * @param dt the time step which you are using to set the maximum distance
  */
  virtual void updateVelocity(Ray & ray, const Point v, const Real dt) const;
};
