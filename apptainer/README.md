Contents in this directory are used for the generation of apptainer images using the `ApptainerGenerator` script found in MOOSE at `scripts/apptainer_generator.py`.

In particular:

- `app_envrionment`: Appends to the environment in the container to add `OPENMC_CROSS_SECTIONS`
- `app_post_post_install`: Copies the cross section download script for use in testing
- `app_post_pre_make`: Sets extra variables for the build and builds the extra dependencies (openmc, dagmc) before the main build so that the `MAKEFLAGS` variable can be set for parallel builds.
