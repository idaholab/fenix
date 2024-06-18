# LeapFrogStepper

!syntax description /UserObjects/LeapFrogStepper

When charged particles are only subjected to an electric field, a standard particle stepping scheme called the leapfrog scheme can be used to accurately calculate the motion of these particles through the field. This scheme can reproduce particle paths exactly, to machine precision, in the case of a constant uniform electric field. More information about this particle stepping scheme can be found in [!cite](Birdsall_Langdon_1991).

On the first step that a particle takes the particle velocity is updated with a 1/2 time step as

\begin{equation}
  \vec{v}_{1/2} = \vec{v}_0 + \frac{q}{m} \vec{F}_0 \frac{dt}{2}
\end{equation}

where $\vec{v}_{0}$ is the initial particle velocity, $q$, and $m$ are the particles charge and mass, $\vec{F}_0$ is the initial force field the particle is subject to, and $\vec{v}_{1/2}$ is the particle velocity 1/2 of a time step after the simulation start time.

The particle position is then updated with a full time step as

\begin{equation}
  \vec{r}_{1} = \vec{r}_0 + \vec{v}_{1/2} dt
\end{equation}

where $\vec{r}_{1}$ is the particles' position after the first time step has been taken and $\vec{r}_0$ is the initial position of the particle. On the $n^\text{th}$ step after the initial step both the particles' velocity and postition are updated with a full time step

\begin{equation}
  \vec{v}_{n + 1/2} = \vec{v}_{n - 1/2} + \frac{q}{m} \vec{F} dt
\end{equation}

where $\vec{v}_{n + 1/2}$ is the particles' velocity at a time which is 1/2 of a time step after the time at time step $n$, and $\vec{v}_{n - 1/2}$ is the particles' velocity at a time which is 1/2 of a time step before the time at time step $n$. Then the new particle position is updated using the velocity from 1/2 step in the future.

\begin{equation}
  \vec{r}_{n} = \vec{r}_{n-1} + \vec{v}_{n - 1/2} dt
\end{equation}

where $\vec{r}_{n}$ is the particles position at the time corresponding to the $n^\text{th}$ time step and $\vec{r}_{n-1}$ is the particles position on the previous time step.

The implementation of the leap frog scheme was verified using single particle motion tests: parallel acceleration in an electric field ([/parallel_acceleration.i]), and projectile motion ([/projectile_motion.i]).

# Example Input Syntax

!listing test/tests/userobjects/particle_stepper/leapfrog_stepper/projectile_motion.i block=UserObjects

!syntax parameters /UserObjects/LeapFrogStepper

!syntax inputs /UserObjects/LeapFrogStepper

!syntax children /UserObjects/LeapFrogStepper

!bibtex bibliography
