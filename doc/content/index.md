!config navigation breadcrumbs=False scrollspy=False

&nbsp;

!media figures/FENIX_logo_blue.png dark_src=figures/FENIX_logo_white.png style=display:block;margin-left:auto;margin-right:auto;width:40%;

# Fusion ENergy Integrated multiphysi-X (FENIX) class=center style=font-weight:200;font-size:200%

!style halign=center
FENIX is designed as an open-source, fully integrated, multiphysics, multiscale, Nuclear Quality Assurance Level 1 (NQA-1) compliant framework facilitating 3D, high-fidelity fusion system modeling.

!style halign=center
FENIX is an application based on the
[MOOSE framework](https://mooseframework.inl.gov) performing system-level, engineering scale (i.e., at the scale of
centimeters and meters), and microstructure-scale (i.e., at the scale of microns) multiphysics
calculations related to fusion energy systems.
Interfaces to other MOOSE-based codes, including tritium transport ([TMAP8](https://mooseframework.inl.gov/tmap8))
and neutronics ([Cardinal](https://cardinal.cels.anl.gov/)) are also included to support FENIX simulations.

!row!
!col! small=12 medium=4 large=4 icon=get_app
## [Getting Started](getting_started/installation.md) class=center style=font-weight:200;font-size:150%;

!style halign=center
Quickly learn how to obtain the FENIX source code, compile an executable, and
run simulations with these instructions.
!col-end!

!col! small=12 medium=4 large=4 icon=settings

## [Code Reference](syntax/index.md) class=center style=font-weight:200;font-size:150%;

!style halign=center
FENIX provides capabilities that can be applied to a wide variety of problems.
The Code Reference provides detailed documentation of specific code features.
General user notes on FENIX can also be found [here](getting_started/user_notes.md).
!col-end!

!col! small=12 medium=4 large=4 icon=assessment
## [Verification, Validation, and Example Cases](verification_validation_examples/index.md) class=center style=font-weight:200;font-size:150%;

!style halign=center
Verification, validation, and example cases list cases showcasing FENIX's capabilities
and ensuring its accuracy.
!col-end!
!row-end!

## FENIX is built on MOOSE style=clear:both;

!style halign=left
FENIX is based on [MOOSE], an extremely flexible framework and simulation environment
that permits the solution of coupled physics problems of varying size and dimensionality.
These can be solved using computer hardware appropriate for the model size, ranging from
laptops and workstations to large high performance computers.

!media large_media/framework/inl_blue.png style=float:right;width:20%;margin-left:30px;

Code reliability is a central principle in code development, and this project
employs a well-defined development and testing strategy.  Code changes are only
merged into the repository after both a manual code review and the automated
regression test system have been completed.  The testing process and status of
FENIX is available at [civet.inl.gov](https://civet.inl.gov/repo/530/).

FENIX and MOOSE are developed at Idaho National Laboratory by a team of
computer scientists and engineers and is supported by various funding agencies,
including the [United States Department of Energy](http://energy.gov).  Development
of these codes is ongoing at [INL](https://www.inl.gov) and by collaborators
throughout the world.

## Utilities

- [AuxAccumulator.md] - Accumulates pointwise data akin to a point source into an auxiliary field

- [VariableSampler.md] - Samples a variable at a given point in space
