# Installation

## Step One: Install Conda MOOSE Environment

!style halign=left
In order to install FENIX, the MOOSE developer environment must be installed. The
installation procedure depends on your operating system, so click on the MOOSE
website link below that corresponds to your operating system/platform and follow
the instructions until you are done with the step named "Install MOOSE" (note that
you do not need to follow the steps from the section titled "Cloning MOOSE" and below). Then,
return to this page and continue with Step Two.

- [Linux and MacOS](https://mooseframework.inl.gov/getting_started/installation/conda.html)
- [Windows (experimental)](https://mooseframework.inl.gov/getting_started/installation/windows.html)

Advanced manual installation instructions for this environment are available
[via the MOOSE website](https://mooseframework.inl.gov/getting_started/installation/index.html).

If an error or other issue is experienced while using the conda environment,
please see [the MOOSE troubleshooting guide for Conda](https://mooseframework.inl.gov/help/troubleshooting.html#condaissues)

## Step Two: Clone FENIX

!style halign=left
FENIX is hosted on [GitHub](https://github.com/idaholab/FENIX), and should be
cloned directly from there using [git](https://git-scm.com/). As in the MOOSE
directions, it is recommended that users create a directory named "projects" to
put all of your MOOSE-related work.

To clone FENIX, run the following commands in Terminal:

```bash
mkdir ~/projects
cd ~/projects
git clone https://github.com/idaholab/FENIX.git
cd FENIX
git checkout main
```

!alert! note title=FENIX branches
This sequence of commands downloads FENIX from the GitHub server and checks
out the "main" code branch. There are two code branches available:

- "main", which is the current most-tested version of FENIX for general usage, and
- "devel", which is intended for code development (and may be more regularly broken
  as changes occur in FENIX and MOOSE).

Developers wishing to add new features should create a new branch for submission
off of the current "devel" branch.
!alert-end!

## Step Three: Build and Test FENIX

!style halign=left
To compile FENIX, first make sure that the conda MOOSE environment is activated
(*and be sure to do this any time that a new Terminal window is opened*):

```bash
conda activate moose
```

Then navigate to the FENIX clone directory and download the MOOSE, TMAP8, and Cardinal submodules:

```bash
cd ~/projects/FENIX
git submodule update --init moose
git submodule update --init tmap8
git submodule update --init cardinal
```


Then, to download OpenMC cross sections needed for OpenMC-based Cardinal runs, run:

```bash
cd ~/projects/FENIX/cardinal
scripts/download-openmc-cross-sections.sh
```

and subsequently set the location of those cross sections in your environment:

```bash
export OPENMC_CROSS_SECTIONS=~/projects/FENIX/cross_sections/cross_sections.xml
```

!alert! warning
This variable +must+ be set in your environment anytime you wish to run FENIX input files that
utilize the OpenMC functionality! This can be done either using `export` on the command line,
or placing this command within a shell config file (`.bashrc`, `.zshrc`, etc.).
!alert-end!

!alert! note
The copies of MOOSE, TMAP8, and Cardinal provided with FENIX have been fully tested against the current
FENIX version, and is guaranteed to work with all current FENIX tests.
!alert-end!

Once all dependencies have been downloaded, FENIX can be compiled and tested:

```bash
cd ~/projects/FENIX
make -j8
./run_tests -j8
```

!alert! note
The `-j8` flag in the above commands signifies the number of processor cores used to
build the code and run the tests. The number in that flag can be changed to the
number of physical and virtual cores on the workstation being used to build FENIX.
!alert-end!

If FENIX is working correctly, all active tests will pass. This indicates that
FENIX is ready to be used and further developed.

## Update FENIX

FENIX (and its underlying dependencies MOOSE, TMAP8, and Cardinal) is under heavy development and is
updated on a continuous basis. Therefore, it is important that the local copy of FENIX be periodically
updated to obtain new capabilities, improvements, and bugfixes. Weekly updates are recommended as,
at minimum, the MOOSE submodule within FENIX is updated up to several times a week.

!alert note title=Always update FENIX and the Conda packages together.
There is a tight dependency between the libraries and applications provided by Conda, and the submodules that FENIX depends on. Therefore, when you update one, you should always update the other.

To update your conda environment, perform the following commands (assuming that your MOOSE-based development environment is named `moose`):

!package! code
conda activate base
conda env remove -n moose
conda create -n moose moose-dev=__MOOSE_DEV__
conda activate moose
!package-end!

To update your FENIX repository as a FENIX user, use the following commands, which provide to general users the content of the most stable branch (`upstream/main`):

```bash
cd ~/projects/FENIX
git checkout main
git fetch upstream
git rebase upstream/main
git submodule update moose
```

To update your FENIX repository as a FENIX developer who regularly makes modifications to the code, use the following commands,
which provide developers with the `devel` branch:

```bash
cd ~/projects/FENIX
git checkout devel
git fetch upstream
git rebase upstream/devel
git submodule update moose
```

Both sets of instructions assume that your copy of FENIX is stored in `~/projects` and that the [idaholab/FENIX](https://github.com/idaholab/FENIX)
git remote is labeled `upstream`. Use `git remote -v` in the FENIX repository location to check for
this and change the commands as necessary. Finally, FENIX can be re-compiled and re-tested.

```bash
cd ~/projects/FENIX
make -j8
./run_tests -j8
```

## Troubleshooting

!style halign=left
If issues are experienced in installation and testing, several resources
are available:

- [FENIX Issues Page](https://github.com/idaholab/FENIX/issues) for FENIX bugs or feature requests.
- [FENIX Discussion Forum](https://github.com/idaholab/FENIX/discussions) for FENIX issues and questions.
- [MOOSE FAQ page](https://mooseframework.inl.gov/help/faq/index.html) for common MOOSE issues.
- [MOOSE Discussion Forum](https://github.com/idaholab/moose/discussions) for non-FENIX issues and questions.

## What next?

!style halign=left
With installation and testing complete, proceed to [using_fenix.md].
