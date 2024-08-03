# Lieberman Ion Wall Losses

This benchmark case is based on the example presented in [!cite](lieberman1994principles), chapter 1, pages 26-27. This case presents some basic particle-in-cell (PIC) simulation results from an unnamed finite-difference-based PIC code.

## Problem Description

Consider a line $x\in[0, l]$ of length $l = 0.1$ \[m\] between two grounded points, where $\phi$ is the electrostatic potential. Between these two points, there exists a uniform singly-charged positive argon ion population of density, $n = 10^{16}$ \[m$^{-3}$\] where all of the ions are initially stationary, and particles do not interact with each other. In this scenario the resulting electrostatic electric field dictates the particle motion. To solve for the electrostatic potential, Poisson's equation is used:

\begin{equation}
  \nabla^2 \phi = \frac{\rho}{\varepsilon_0}
\end{equation}

where $rho$ is the charge density and $\varepsilon_0$ is the permittivity of free space. In 1D, this can be written as

\begin{equation}
  \frac{\partial^2  \phi}{\partial x^2} = \frac{\rho}{\varepsilon_0}.
\end{equation}

Accounting for the charge density present yields

\begin{equation}
  \frac{\partial^2  \phi}{\partial x^2} = \frac{en}{\varepsilon_0}
\end{equation}

where $e$ is the elemental charge. Specifying the boundary conditions leads to the following boundary value problem:

\begin{equation}
  \frac{\partial^2  \phi}{\partial x^2} = \frac{en}{\varepsilon_0}
\end{equation}

with

\begin{equation}
  \phi(0) = \phi(l) = 0.
\end{equation}


The solution to which is

\begin{equation}
  \phi(x) =
  \frac{1}{2}
  \frac{en}{\varepsilon_0}
  \left[
    \left(\frac{l}{2}\right)^2
    -
    \left(x - \frac{l}{2}\right)^2
  \right].
\end{equation}

The initial maximum electrostatic potential is located at the point $x=l/2$ and has a value of $2.3\times10^5$ \[V\].

## Particle Representation

100 computational particles are used to represent the initial uniform argon ion density. These particles are evenly spaced across the domain and the $i^\text{th}$ computational particle weight, $\omega_i$, is calculated by

\begin{equation}
  \omega_i =
  \frac{
  n  V
  }{
    \text{PPE}
  }
\end{equation}

where $V$ is the volume of the element in which the particle exists, and $PPE$ is the number of particles which exist in the element. All particles are stationary initially ($v=0$ \[m/s\]) and particle velocities are updated with the [LeapFrogStepper.md]. Additionally, when particles hit the boundaries of the domain, they are considered to have left the domain, and we no longer track them after this point. This is why a [KillRayBC.md] is selected for the boundary points of the domain $x=0,l$.

## Transient Conditions

In Figure 2.2 of [!cite](lieberman1994principles), there is inconsistent information about the time step selected and the total simulation time. We were able to replicate the results presented in this figure using a total simulation time of $2.5\times10^{-9}$ \[s\] with a constant time step of $\Delta t = 10^{-10}$ \[s\]. Additionally, the example input file uses 100 elements, 1 for each particle. This was selected arbitrarily.

## Input file

### Various Constants

The first section of the input file declares some constants and problem parameters that will be used in various different blocks throughout the file. These quantities do not belong to any block and can be called from any block within the input file.

!listing test/tests/benchmarking/lieberman.i end=Mesh

### Mesh

The mesh block creates a one dimensional domain $x\in[0, l]$ split up evenly into 100 different elements.

!listing test/tests/benchmarking/lieberman.i block=Mesh

### Variables

In this simulation, there are two field variables. The first variable, `phi`, is the variable that represents the electrostatic potential. The second variable, `n`, represents the projection of the discrete particle density onto the finite element mesh. The variable `n` is not required for actually performing the simulation, but it provides a convenient way to visualize the argon ion density.

!alert note title=Particle Quantity Visualization
Since computational particles in FENIX are assumed to be point particles, the particle shape function is a Dirac delta function. Thus, we must project the discrete quantities onto the finite element mesh.

!listing test/tests/benchmarking/lieberman.i block=Variables

### Kernels

Each variable only needs a single kernel. For the electrostatic potential, `phi`, the [ADMatDiffusion.md] kernel can be used to provide the Laplacian operator, which enables the system to solve Poisson's equation.

!alert note title=
Since FENIX does not apply the factor $\varepsilon_0^{-1}$ directly when evaluating the inner product of the particle charge density and the test function, the quantity $\varepsilon_0$ is passed to the Laplacian operator via the `diffusivity` parameter.

The variable `n` utilizes the [ProjectionKernel.md] to enable visualization of the particle number density as a field variable. See [ProjectionKernel.md] for more detail.

!listing test/tests/benchmarking/lieberman.i block=Kernels

### Boundary Conditions

The only variable that needs a boundary condition is the electrostatic potential. Since our simulation assumes that end points of the domain are grounded, we can use a simple [DirichletBC.md] on both boundaries set to zero.

!listing test/tests/benchmarking/lieberman.i block=BCs

### Field Initial Condition

The only variable which needs an initial condition is the electrostatic potential. Here, we prescribe the initial condition of the electrostatic potential to be the analytic solution for the electrostatic potential given the prescribed uniform charge density.

!listing test/tests/benchmarking/lieberman.i block=Functions ICs

### AuxVariables

One auxiliary variable is created for each field component. Since our particles always have three velocity components stored in their data to represent their motion regardless of the MOOSE Problem dimensionality, we also require three field components here. However, only one component is being used to represent the 1D electric field.

!listing test/tests/benchmarking/lieberman.i block=AuxVariables

### AuxKernels

For this simulation, only the x component of the electric field is being used. In this block, the electric field is computed from the electrostatic potential. The other AuxVariables do not have any kernels associated with them, and, as a result, they will have a value of 0 throughout the simulation.

!listing test/tests/benchmarking/lieberman.i block=AuxKernels

### Particle Initialization and Updating

The `UserObjects` block is where particles are created and the rules for how the particle velocity is updated by the fields are defined. The `stepper` is a [LeapFrogStepper.md] which updates a particles' velocity based on the value of the electric fields at the location at which the particle exists. The `initializer` defines the rules for how particles are placed in the mesh and the how their velocities are initialized. The [UniformGridParticleInitializer.md] places particles evenly throughout the mesh. In this case, 100 particles are placed on the mesh with uniform spacing between them, and they are weighted so that this particle distribution will approximate the specified argon ion number density, `n`. The last parameter, `velocity_distributions`, tells the system which distributions that should be sampled from when initializing particle velocity data. The final object is the `TestInitializedPICStudy`. This object and any other objects which inherit from [PICStudyBase.md] are responsible for managing the particles themselves.

!listing test/tests/benchmarking/lieberman.i block=Distributions UserObjects
                                                 remove=UserObjects/charge_accumulator UserObjects/density_accumulator
                                                 UserObjects/study/particles_per_element

### Residual Contributions

In order to contribute to the residual of variables based on particle quantities, you must use an accumulator. Accumulators evaluate the inner product of particle quantities and the test function. In this block, there are two accumulators: the [ChargeDensityAccumulator.md] and the [NumberDensityAccumulator.md]. These objects do the same thing, but do so for different quantities. The [ChargeDensityAccumulator.md] evaluates the inner product between the computational particle charge density, which is defined as

\begin{equation}
  \rho = \sum_{i=1}^N \omega_i q_i,
\end{equation}

and the finite element test function, while the [NumberDensityAccumulator.md] evaluates the inner product between the computational particle number density, which is defined as

\begin{equation}
  n = \sum_{i=1}^N \omega_i,
\end{equation}

and the finite element test function. These objects contribute to the residual of the electrostatic potential, `phi` and the projection of the particle density onto the finite element mesh, `n`, respectively.

!listing test/tests/benchmarking/lieberman.i block=UserObjects
                                                 remove=UserObjects/stepper UserObjects/initializer UserObjects/study

## Results

Results of the FENIX simulation are directly compared to those reported in [!cite](lieberman1994principles). The comparison of these results is accomplished by directly reproducing Figure 2.2 of [!cite](lieberman1994principles) and plotting the FENIX results directly on top of the published results, ensuring that the bounds of the axes presented in [!cite](lieberman1994principles) are maintained. As seen in these figures, FENIX is able to reproduce the results nearly exactly.

!row! style=display:inline-flex;

!media figures/lieberman_vdf_comparison.png style=width:90%;display:block;margin-left:auto;margin-right:auto;

!media figures/lieberman_population_comparison.png style=width:88%;display:block;margin-left:auto;margin-right:auto;

!row-end!

!media figures/lieberman_potential_comparison.png style=width:50%;display:block;margin-left:auto;margin-right:auto;


## Running the Case

To run this input, use the following commands in your Terminal. The `--allow-test-objects` flag allows the use of custom FENIX testing objects that are not used for "production" physics capability, but are useful for representing simple cases such as in this scenario.

```bash
  cd ~/projects/fenix/test/tests/benchmarking
  ../../../fenix-opt -i lieberman.i --allow-test-objects
  python plot_lieberman_results.py
```
