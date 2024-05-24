# ParticlesPerElementInitializer

!syntax description /UserObjects/ParticlesPerElementInitializer

Because the charge density in FENIX is defined by delta functions and each computational particle represents some number of physical particles computational particles are assigned a weight $\omega_i$. In cartesian geometry this weight reprsents the number of physical particles per \[m$^{3-d}$\] where $d$ is the dimension of the problem.

!alert warning Using Non-Cartesian Geometries
The weight calculation for particles has only been implemented for Cartesian domains and may not be valid for Non-Cartesian domains.

In this object the the particle weight is given by

\begin{equation}
  \omega_j \equiv
  \frac{
    N \; V
  }{
    \text{ppe}
  }
\end{equation}

where $N$ is the requested number density, $V_j$ is the dimension dependent "volume" (length in 1D, area in 2D and volume in 3D) of the element the particle is being placed in and ppe is the requested number of particles per element.

# Example Input Syntax

!listing test/tests/userobjects/particles_per_element_initializer/elements/1d.i block=UserObjects

!syntax parameters /UserObjects/ParticlesPerElementInitializer

!syntax inputs /UserObjects/ParticlesPerElementInitializer

!syntax children /UserObjects/ParticlesPerElementInitializer

!bibtex bibliography
