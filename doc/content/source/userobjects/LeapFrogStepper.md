# LeapFrogStepper

!syntax description /UserObjects/LeapFrogStepper

On the first step that a particle takes the particle velocity is updated with a 1/2 time step as

\begin{equation}
  \vec{v}_{1/2} = \vec{v}_0 + \frac{q}{m} \vec{F} \frac{dt}{2}
\end{equation}

The particle position is then updated with a full time step as

\begin{equation}
  \vec{r}_{1} = \vec{r}_0 + \vec{v}_{1/2} dt
\end{equation}

For every other time step, $n$, after the initial both are updated with a full time step and

\begin{equation}
  \vec{v}_{n + 1/2} = \vec{v}_{n - 1/2} + \frac{q}{m} \vec{F} dt
\end{equation}

\begin{equation}
  \vec{r}_{n} = \vec{r}_{n-1} + \vec{v}_{n + 1/2} dt
\end{equation}

# Example Input Syntax

!listing test/tests/userobjects/leapfrog_stepper/projectile_motion.i block=UserObjects

!syntax parameters /UserObjects/LeapFrogStepper

!syntax inputs /UserObjects/LeapFrogStepper

!syntax children /UserObjects/LeapFrogStepper
