!template load file=sqa/app_sdd.md.template app=FENIX category=fenix

!template! item key=introduction
Many of the phenomena related to fusion energy systems depend on the solutions of multiple
physics models, which can be described by partial differential equations that provide spatially- and
temporally-varying values of solution variables. These models for individual physics often depend on
each other. [!ac]({{app}}) relies on the MOOSE framework to solve these physics models, accounting
for the couplings that may occur between them. This document describes the system design of [!ac]({{app}}).
!template-end!

!template! item key=system-scope
!include fenix_srs.md start=system-scope-begin end=system-scope-finish
!template-end!

!template! item key=dependencies-and-limitations
{{app}} inherits the [software dependencies of the MOOSE framework](framework_sdd.md#dependencies-and-limitations),
with no additional dependencies.
!template-end!

!template! item key=design-stakeholders
Stakeholders for [!ac]({{app}}) include several of the funding sources including [!ac](DOE) and [!ac](INL).
However, since [!ac]({{app}}) is an open-source project, several universities, companies, and foreign
governments have an interest in the development and maintenance of the [!ac]({{app}}) project.
!template-end!

!template! item key=system-design
[!ac]({{app}}) relies on MOOSE to solve the coupled physics models underlying fusion energy systems,
accounting for the couplings that may occur between various components and sub-systems of this class
of devices. The design of MOOSE is based on the concept of modular code objects that define all of
the aspects of the physics model. [!ac]({{app}}) follows this design, providing code objects that
define specific aspects of the solutions for its physics that derive from the base classes defined
by the MOOSE framework and the modules that it depends on. [!ac]({{app}}) provides specialized
[UserObject](syntax/UserObjects/index.md) classes that compute data for velocity updating, charge
accumulation, and [Ray](source/raytracing/Ray.md) data necessary for simple [!ac](PIC) simulations.
It also provides specialized utilities for accumulating field data on rays. Much of the remaining
functionality is provided by MOOSE modules (electromagnetics, ray tracing), the
[Cardinal](https://cardinal.cels.anl.gov/) application ([!ac](CFD) and Monte Carlo radiation transport),
and the [Tritium Migration Analysis Program Version 8 (TMAP8)](https://mooseframework.inl.gov/TMAP8)
application (tritium migration) that [!ac]({{app}}) builds on.
!template-end!

!template! item key=system-structure
[!ac]({{app}}) relies on the MOOSE framework to provide the core functionality of solving multiphysics problems
using the finite element method. It also relies on the MOOSE modules for much of its core functionality.
A summary listing of the current modules required for complete [!ac]({{app}}) operation are shown below:

- [Electromagnetics](electromagnetics/index.md)
- [Ray Tracing](ray_tracing/index.md)

The structure of [!ac]({{app}}) is based on defining C++ classes that derive from classes in the MOOSE
framework or modules that provide functionality that is specifically tailored to fusion device
modeling and simulation. By using the interfaces defined in MOOSE base classes for these classes,
[!ac]({{app}}) is able to rely on MOOSE to execute these models at the appropriate times during the
simulation and use their results in the desired ways.
!template-end!

!syntax complete subsystems=False actions=False objects=False
