# Lieberman Ion Wall Losses

This benchmark case is based on the example presented in [!cite](lieberman1994principles), chapter 1, pages 26-27

## Problem Description

Consider a line $x\in[0, l]$ of length $l = 0.1$ \[m\] between two grounded points, $\phi(0) = \phi(l) = 0$, where $\phi$ is the electrostatic potential. Between these two points there exists a uniform singly charged positive argon ion number density, $n = 10^{16}$ \[m$^{-3}$\] where all of the ions are initially stationary, and particles do not interact with eachother. In this scenario the resulting electrostatic field dictates the particle motion. To solve for the electrostatic potential Poisson's equation is used.

\begin{equation}
  \nabla^2 \phi = \frac{\rho}{\varepsilon_0}
\end{equation}

In 1D this can be written as

\begin{equation}
  \frac{\partial^2  \phi}{\partial x^2} = \frac{\rho}{\varepsilon_0}
\end{equation}

Accounting for the charge density present yields

\begin{equation}
  \frac{\partial^2  \phi}{\partial x^2} = \frac{en}{\varepsilon_0}
\end{equation}

where $e$ is the elemental charge, Specifying the boundary conditions leads to the following boundary value problem

\begin{equation}
  \phi(0) = \phi(1) = 0
\end{equation}

\begin{equation}
  \frac{\partial^2  \phi}{\partial x^2} = \frac{en}{\varepsilon_0}
\end{equation}

The solution to which is

\begin{equation}
  \phi(x) =
  \frac{1}{2}
  \frac{en}{\varepsilon_0}
  \left[
    \left(\frac{l}{2}\right)^2
    - x^2
  \right]
\end{equation}

The initial maximum electrostatic potential is located at the point $x=l/2$ and has a value of $2.3\times10^5$ \[V\].

## Particle Representation

100 computational particles are used to represent the initial uniform argon ion density. These particles are evenly spaced across the domain and the $i^\text{th}$ computational particle weight, $\omega_i$ is calculated by

\begin{equation}
  \omega_i =
  \frac{
  n  V
  }{
    \text{PPE}
  }
\end{equation}

where $V$ is the volume of the element in which the particle exists, and $PPE$ is the number of particles which exist in the element. All particles are stationary initially $v=0$ \[m/s\] and particle velocities are updated with the [LeapFrogStepper.md]. Additionally, when particles hit the boundaries of the domain they are considered to have left the domain and we no longer track them after this point, this is why [KillRayBC.md] are selected for the points $x=0,l$.

## Transient Conditions

In figure 2.2 of [!cite](lieberman1994principles) there is inconsistent information about the time step selected and the total simulation time. We were able to replicated the results presented in this figure using a total simulation time of $2.5\times10^{-9}$ \[s\] with a constant time step of $\Delta t = 10^{-10}$ \[s\]. Additionally, the example input file uses 100 elements, 1 for each particle, this was selected arbitrarily.

## Input file

### Various Constants

!listing test/tests/simple-benchmark/lieberman.i end=Mesh

The first section of the input file declares some constants and problem parameters that will be used in various different blocks throughout the input file. These quantities do not belong to any block and can be called from any block within the input file.

### Mesh

!listing test/tests/simple-benchmark/lieberman.i block=Mesh

The mesh block creates a one dimensional domain $x\in[0, l]$ split up evenly into 100 different elements.

### Variables

!listing test/tests/simple-benchmark/lieberman.i block=Variables

In this simulation there are two field variables. The first variable, `phi`, is the variable that represents the electrostatic potential. The second variable, `n`, represents the projection of the discrete particle density onto the finite element mesh.

!alert note title=Particle Quantity Visualization
Since computational particles in FENIX are assumed to be point particles, the particle shape function is a dirac delta function, we must project the discrete quantities onto the finite element mesh.

### Kernels

!listing test/tests/simple-benchmark/lieberman.i block=Kernels

Each variable only needs a single kernel. The electrostatic potential, `phi`, requires this diffusion kernel which enables the system to solve Poisson's equation.

!alert note title=
Since FENIX does not apply the factor $\varepsilon_0^{-1}$ directly when evalulating the inner product of the particle charge density and the test function the quantity $\varepsilon_0$ is shifted to the laplacian operator.

The variable `n` utilizes the [ProjectionKernel.md] to enable visualization of the particle number density as a field variable. See [ProjectionKernel.md] for more detail.

### Boundary Conditions

!listing test/tests/simple-benchmark/lieberman.i block=BCs

The only variable that needs a boundary condition is the electrostatic potential. Since our simulation assumes that end points of the domain are grounded.

### Field Initial Condition

!listing test/tests/simple-benchmark/lieberman.i block=Functions ICs

The only variable which needs an initial condition is the electrostatic potential, here we prescribe the initial condition of the electrostatic potential to be the analytic solution for the electrostatic potential given the prescribed uniform charge density.

### AuxVariables

!listing test/tests/simple-benchmark/lieberman.i block=AuxVariables

Here one auxilary variable is created for each field component. We require three here since our particles always have 3 velocity components stored in data.

### AuxKernels

!listing test/tests/simple-benchmark/lieberman.i block=AuxKernels

For this simulation only the x component of the electric field is being used in this block the electric field is computed from the electrostatic potential. The other AuxVariables do not have any kernels associated with them and as a result they will have a value of 0 throughout the simulation.

### Particle Initialization and Updating

!listing test/tests/simple-benchmark/lieberman.i block=Distributions UserObjects
                                                 remove=UserObjects/charge_accumulator UserObjects/density_accumulator

This block is where particles are created and the rules for how the particle velocity is update by the fields is defined. The `stepper` is a [LeapFrogStepper.md] which updates the particles velocity based on the value of the electric fields at the location the particle exists. The `initializer` defines the rules for how particles are placed in the mesh. The [UniformGridParticleInitializer.md]

## Results
