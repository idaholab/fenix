!template load file=sqa/app_srs.md.template app=FENIX category=fenix

!template! item key=system-scope
!! system-scope-begin
FENIX is an application for performing system-level, engineering scale (i.e., at the scale of
centimeters and meters), and microstructure-scale (i.e., at the scale of microns) multiphysics
calculations related to fusion energy systems. These models often include highly coupled systems of
equations related to plasma physics, electromagnetics, heat conduction, scalar transport, thermal
hydraulics, [!ac](CFD), and thermomechanics, amongst others. Interfaces to other MOOSE-based codes,
including tritium transport (TMAP8) and neutronics (Cardinal) are also included to support FENIX
simulations. FENIX will enable high-fidelity modeling of irradiation levels and plasma exposure
conditions of plasma facing components and their impact on heat and tritium distributions, as well
as the resulting mechanical constraints experienced by the plasma facing components. The [syntax/MultiApps/index.md]
is leveraged to allow for the multiscale, multiphysics coupling. Further, other MOOSE capabilities
(such as the stochastic tools module) will eventually enable engineering studies, allowing for extended
uncertainty quantification and risk analysis studies for particular system designs. Interfaces for
[!ac](CAD) meshing workflows to model complex geometries are also included. FENIX therefore supports
design, safety, engineering, and research projects.
!! system-scope-finish
!template-end!

!template! item key=system-purpose
!! system-purpose-begin
The purpose of FENIX is to perform fully integrated, high-fidelity, multiphysics simulations of fusion
energy systems and devices at different length scales with a variety of materials, system configurations,
and component designs in order to better understand component degradation and operational impacts on
system performance. FENIX's main goal is to bring together the combined multiphysics capabilities of
the [!ac](MOOSE) ecosystem to provide an open platform for future research, safety assessment,
engineering, and design studies of fusion energy systems.
!! system-purpose-finish
!template-end!

!template! item key=assumptions-and-dependencies
[!ac]({{app}}) has no constraints on hardware and software beyond those of the MOOSE framework and
modules listed in their respective [!ac](SRS) documents, which are accessible through the links at
the beginning of this document.

[!ac]({{app}}) provides access to a number of code objects that perform computations, such as particle
transport, material behavior, and boundary conditions. These objects each make their own physics-based
assumptions, such as the units of the inputs and outputs. Those assumptions are described in the
documentation for those individual objects.
!template-end!

!template! item key=user-characteristics
[!ac]({{app}}) has three main classes of users:

- +[!ac]({{app}}) Developers+: These are the core developers of [!ac]({{app}}). They are responsible
  for designing, implementing, and maintaining the software, while following and enforcing its software
  development standards.
- +Developers+: These are scientists or engineers that modify or add capabilities to [!ac]({{app}})
  for their own purposes, which may include research or extending its capabilities. They will typically
  have a background in fusion energy sciences, plasma physics, tritium migration, radiation transport,
  heat conduction, and/or material science as well as in modeling and simulation techniques, but may have
  more limited background in code development using the C++ language. In many cases, these developers
  will be encouraged to contribute code back to [!ac]({{app}}).
- +Analysts+: These are users that run [!ac]({{app}}) to run simulations, but do not develop code.
  The primary interface of these users with [!ac]({{app}}) is the input files that define their
  simulations. These users may interact with developers of the system requesting new features and
  reporting bugs found.
!template-end!

!template! item key=information-management
[!ac]({{app}}) as well as the core MOOSE framework in its entirety will be made publicly available
on an appropriate repository hosting site. Day-to-day backups and security services will be provided
by the hosting service. More information about backups of the public repository on [!ac](INL)-hosted
services can be found on the following page: [sqa/github_backup.md]
!template-end!

!template! item key=policies-and-regulations
!include framework_srs.md start=policies-and-regulations-begin end=policies-and-regulations-finish
!template-end!

!template! item key=packaging
No special requirements are needed for packaging or shipping any media containing the [!ac](MOOSE)
and [!ac]({{app}}) source code. However, some [!ac](MOOSE)-based applications that use the [!ac]({{app}})
code may be export-controlled, in which case all export control restrictions must be adhered to when
packaging and shipping media.
!template-end!

!template! item key=reliability
The regression test suite will cover at least 87% of all lines of code at all times. Known
regressions will be recorded and tracked (see [#maintainability]) to an independent and
satisfactory resolution.
!template-end!
