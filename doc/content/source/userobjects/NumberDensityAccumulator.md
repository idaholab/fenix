# NumberDensityAccumulator

!syntax description /UserObjects/NumberDensityAccumulator

## Overview

In FENIX the charge density $\rho$ is defined as

\begin{equation} \label{eq:charge_density}
  \rho
  \left(
    \vec{r}\,
  \right)
   =
  \sum_{i=1}^N
  q_i
  w_i \delta
  \left(
    \vec{r} - \vec{r}_i
  \right),
\end{equation}

where $N$ is the number of computational particles in the simulation, $q_i$ is the charge of the $i^\text{th}$ particle, $w_i$ is the weight of the $i^\text{th}$ particle, and $\vec{r}_i$ is the position at which the $i^\text{th}$ computational particle exists.

## Example Input Syntax

!listing test/tests/userobjects/particle_quantity_accumulation/simple_potential_solve.i block=UserObjects

!syntax parameters /UserObjects/NumberDensityAccumulator

!syntax inputs /UserObjects/NumberDensityAccumulator

!syntax children /UserObjects/NumberDensityAccumulator
