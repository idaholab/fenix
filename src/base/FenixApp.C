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

#include "FenixApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "ModulesApp.h"
#include "MooseSyntax.h"

InputParameters
FenixApp::validParams()
{
  InputParameters params = MooseApp::validParams();
  params.set<bool>("use_legacy_material_output") = false;
  params.set<bool>("use_legacy_initial_residual_evaluation_behavior") = false;
  return params;
}

FenixApp::FenixApp(InputParameters parameters) : MooseApp(parameters)
{
  FenixApp::registerAll(_factory, _action_factory, _syntax);
}

FenixApp::~FenixApp() {}

void
FenixApp::registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  ModulesApp::registerAllObjects<FenixApp>(f, af, s);
  Registry::registerObjectsTo(f, {"FenixApp"});
  Registry::registerActionsTo(af, {"FenixApp"});

  /* register custom execute flags, action syntax, etc. here */
}

void
FenixApp::registerApps()
{
  registerApp(FenixApp);
  ModulesApp::registerApps();
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
extern "C" void
FenixApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  FenixApp::registerAll(f, af, s);
}
extern "C" void
FenixApp__registerApps()
{
  FenixApp::registerApps();
}
