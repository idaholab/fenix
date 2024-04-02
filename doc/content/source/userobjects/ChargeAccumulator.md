# ChargeAccumulator

!syntax description /UserObjects/ChargeAccumulator

In FENIX the charge density $\rho$ is defined as

\begin{equation} \label{eq:charge_density}
  \rho =
  \sum_{i=1}^N
  q_i \delta
  \left(
    \vec{r} - \vec{r}_i
  \right),
\end{equation}

where $N$ is the number of computational particles in the simulation, $q_i$ is the charge of the $i^\text{th}$ particle, and $\vec{r}_i$ is the position at which the $i^\text{th}$ computational particle exists.

## Electrostatic PIC

In electrostatic PIC simulations the electric field is indirectly solved for by first solving for the electrostatic potential, $\phi$, using Poisson's equation

\begin{equation} \label{eq:poisson}
  -\nabla^2 \phi
  =
  \frac{\rho}{\varepsilon_0}
\end{equation}

The weak form of this takes the form

\begin{equation} \label{eq:weak_poisson}
  \left<
  -
  \varepsilon_0
  \nabla^2 \phi,
  \psi
  \right>
  -
  \left<
  \rho,
  \psi
  \right>
  =
  0.
\end{equation}

The second term of the weak form of Poisson's equation can then be evaluated as

\begin{equation} \label{eq:weak_poisson_right_term}
  \left<
  \rho,
  \psi_j
  \right>
  =
  \sum_{i=1}^{M}
  q_i
  \psi_j
  \left(
    \vec{r}_i
  \right),
\end{equation}

where $M$ is the number of particles which exist on the subdomain where the $j^\text{th}$ test function $\psi_j$ is non-zero. This objects evaluates this source term directly and accumulates the result directly into the residual of the variable representing $\phi$.

!alert note
This representation of the charge density means that direct knowledge of the charge density is lost and only the inner product can be evaluated.

!alert note
This object should be set to execute on `PRE_KERNELS` so that the residual contribution from the charge density source term is computed correctly.

!listing test/tests/userobjects/charge_accumulation/simple_potential_solve.i block=UserObjects

!syntax parameters /UserObjects/ChargeAccumulator

!syntax inputs /UserObjects/ChargeAccumulator

!syntax children /UserObjects/ChargeAccumulator
