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

#include "FenixTestApp.h"
#include "FenixApp.h"
#include "FenixHeader.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

InputParameters
FenixTestApp::validParams()
{
  InputParameters params = FenixApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

FenixTestApp::FenixTestApp(InputParameters parameters) : MooseApp(parameters)
{
  FenixTestApp::registerAll(
      _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

FenixTestApp::~FenixTestApp() {}

std::string
FenixTestApp::header() const
{
  return FenixHeader::header();
}

void
FenixTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  FenixApp::registerAll(f, af, s);
  if (use_test_objs)
  {
    Registry::registerObjectsTo(f, {"FenixTestApp"});
    Registry::registerActionsTo(af, {"FenixTestApp"});
  }
}

void
FenixTestApp::registerApps()
{
  registerApp(FenixTestApp);
  FenixApp::registerApps();
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
FenixTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  FenixTestApp::registerAll(f, af, s);
}
extern "C" void
FenixTestApp__registerApps()
{
  FenixTestApp::registerApps();
}
