# BorisStepper

!syntax description /UserObjects/BorisStepper

In magnetized (or electromagnetic) PIC simulations, the de facto standard particle stepping algorithm is commonly known as the Boris algorithm [!cite](boris1970relativistic,Birdsall_Langdon_1991, qin2013boris). This algorithm is similar to a [Leap Frog method](userobjects/LeapFrogStepper.md) and has second order accuracy in time when solving the equations of motion for a charged particle, given by

\begin{equation} \label{eq:pos}
  \frac{\partial \vec{r}}{\partial t}
  =
  \vec{v}
\end{equation}

and

\begin{equation} \label{eq:vel}
  \frac{\partial \vec{v}}{\partial t}
  =
  \frac{q}{m}
  \left[
    \vec{E}
    +
    \vec{v}
    \times
    \vec{B}
  \right]
\end{equation}

where $q$ is the particle's charge, $m$ is the particle's mass, and  $\vec{E}$ and $\vec{B}$ are the electric and magnetic fields that the particle is subject to, respectively.

In the Boris algorithm, \cref{eq:pos,eq:vel} are discretized with a central difference scheme and the acceleration due to the electric field and magnetic field are separated. First, half of the impulse due to the electric field is applied to the particle, as

\begin{equation} \label{eq:e_half1}
  \vec{v}^{\,-}
  =
  \vec{v}_{n}
  +
  \frac{q}{m}
  \vec{E}_n
  \frac{\Delta t}{2}
\end{equation}

where $\vec{v}^{\,-}$ is an intermediate particle velocity, $\vec{v}_{n}$ is the particle velocity at step $n$, and $\vec{E}_{n}$ is the electric field at step $n$. The velocity of the particle after rotation due to the magnetic field is derived as

\begin{equation} \label{eq:boris_v_plus}
  \vec{v}^{\,+}
  =
  \vec{v}^{\,-}
  +
  \vec{v}^{\,'}
  \times
  \vec{s},
\end{equation}

with

\begin{equation} \label{eq:boris_v_prime}
  \vec{v}^{\,'}
  =
  \vec{v}^{\,-}
  +
  \vec{v}^{\,-}
  \times
  \vec{l}
\end{equation}

where

\begin{equation} \label{eq:boris_t}
  \vec{l} =
  \frac{q}{m}
  \vec{B}_n
  \Delta t,
\end{equation}

which accounts for the effect of $\vec{B}_n$, the magnetic field at step $n$. $\vec{s}$ is defined as

\begin{equation} \label{eq:boris_s}
  \vec{s} =
  \frac{2 \vec{l}}{
    1 + \vec{l} \cdot \vec{l}
  }.
\end{equation}

Finally, the rotation due to the presence of the magnetic field is then applied with

\begin{equation} \label{eq:mag_step}
  \frac{
    \vec{v}^{\,+}
    -
    \vec{v}^{\,-}
  }{ \Delta t}
  =
  \frac{q}{m}
  \left(
    \vec{v}^{\,+}
    +
    \vec{v}^{\,-}
  \right)
  \times
  \vec{B}_n,
\end{equation}

and the final impulse due to the electric field is then applied to the particle using

\begin{equation} \label{eq:e_half2}
  \vec{v}_{n+1}
  =
  \vec{v}^{\,+}
  +
  \frac{q}{m}
  \vec{E}_n
  \frac{\Delta t}{2}.
\end{equation}

The implementation of the Boris algorithm was verified using several single particle motion tests: constant electric field ([/boris_parallel_acceleration.i], [/boris_projectile_motion.i]), cyclotron motion ([/cyclotron_motion.i]), and $\vec{E} \times \vec{B}$ drift motion ([/e_cross_b.i]).

# Example Input Syntax

!listing test/tests/userobjects/boris_stepper/cyclotron_motion.i block=UserObjects

!syntax parameters /UserObjects/BorisStepper

!syntax inputs /UserObjects/BorisStepper

!syntax children /UserObjects/BorisStepper

!bibtex bibliography
