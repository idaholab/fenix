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

## Results



## Input file

!listing test/tests/simple-benchmark/lieberman.i
