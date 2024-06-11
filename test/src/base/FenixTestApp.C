//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "FenixTestApp.h"
#include "FenixApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"

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
