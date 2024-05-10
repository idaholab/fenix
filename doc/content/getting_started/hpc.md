# Using FENIX on INL HPC Systems

!alert! note
This guide will discuss how to use FENIX in an [!ac](INL) [!ac](HPC) environment. To
install FENIX on your own [!ac](HPC) system, you should start by reviewing the
[MOOSE HPC Cluster instructions](https://mooseframework.inl.gov/getting_started/installation/hpc_install_moose.html)
for information on how to install MOOSE (and MOOSE-based applications like FENIX) and working with
your system administrator to get the appropriate software dependencies installed. Once this is done,
proceed with the manual HPC installation instructions below in Step Three.
!alert-end!

## Step One: Get an INL HPC Account

!style halign=left
To use FENIX on [!ac](INL) [!ac](HPC), you first need an [!ac](HPC) account. Information on direct
access to [!ac](HPC), or through the web-based HPC OnDemand service, can be found on the
MOOSE website [INL HPC Services](https://mooseframework.inl.gov/help/inl/index.html) page.

## Step Two: Set Up the Right Environment

!style halign=left
An optimized, pre-packaged environment is available for use on the Sawtooth and Lemhi supercomputers.
When logged in, run:

```bash
module load use.moose moose-dev
```

A set of appropriate MOOSE dependency modules will be loaded for use. Note that these modules
+require activation+ each time you log in.

## Step Three: Clone FENIX

<!-- Re-use the clone instructions from the main getting started instructions here -->

!style halign=left
!include getting_started/installation.md start=FENIX is hosted on end=!alert-end! include-end=True

## Step Four: Build and Test FENIX

!style halign=left
To compile FENIX, first make sure that the [!ac](HPC) environment is activated from Step Two (+and be sure to do this any time that a new HPC Terminal instance is opened+):

```bash
module load use.moose moose-dev
```

<!-- Re-use the bulk of the build and test section from the main getting started instructions here -->

!include getting_started/installation.md start=Then navigate to the FENIX end=FENIX can be compiled and tested include-end=True

```bash
make -j8
```

Once FENIX has successfully been compiled, the following command runs the active FENIX tests:

```bash
./run_tests -j8
```

The `-j8` flag in the above commands signifies the number of processor cores used to build the code
and run the tests. The number in that flag can be changed to the desired number of physical and
virtual cores on the [!ac](HPC) system being used to build FENIX. If FENIX is working correctly, all
active tests will pass. This indicates that FENIX is ready to be used and further developed.

!alert! warning title=Be courteous to fellow HPC users
The number of cores for building and testing on a Sawtooth or Lemhi "head node" / "sign-in node"
should be kept as low as possible to not take up excessive system resources. Please be courteous to
fellow users!
!alert-end!

## Step Five: Keep FENIX Updated

<!-- Re-use the bulk of the update section from the main getting started instructions here -->

!style halign=left
!include getting_started/installation.md start=FENIX (and the underlying MOOSE Framework) end=updated up to several times a week. include-end=True

!alert! note
While the FENIX source code should be manually updated, the HPC modules (unlike the
[MOOSE conda packages](https://mooseframework.inl.gov/getting_started/installation/conda.html)) do
not need to be kept up-to-date by the end user/developer. These are updated automatically for you by
the [!ac](HPC) system administrators.
!alert-end!

!include getting_started/installation.md start=To update your FENIX repository as a FENIX user end=Finally, FENIX can be re-compiled and re-tested. include-end=True

```bash
make -j8
./run_tests -j8
```

## Viewing Results Remotely

!style halign=left
You can use HPC OnDemand to view a FENIX Exodus results file remotely. First, access the
[HPC OnDemand Dashboard](https://hpcondemand.inl.gov/pun/sys/dashboard), select `Interactive Apps`,
and then select `Linux Desktop with Visualization`. Next, select your cluster (such as Sawtooth),
the amount of time you believe you need, and then click `Launch`.

It may take some time before your 'Visualization Job' becomes available. When it does, simply click
on it, and you will be presented a [!ac](GUI) desktop within your web browser. From here, you can
open visualization applications (such as [ParaView](https://www.paraview.org/)), and open your
results file.

To use ParaView, open a terminal by clicking `Applications` at the top left, then click
`Terminal Emulator`. A terminal window will open. Enter the following commands:

```bash
module load paraview
paraview
```

ParaView should open. From here, you can select `File`, `Open`, and navigate to the directory
containing your Exodus results file, and open it.

<!-- Include troubleshooting section from main getting started  -->

!include getting_started/installation.md start=Troubleshooting end=non-FENIX issues and questions. include-end=True

