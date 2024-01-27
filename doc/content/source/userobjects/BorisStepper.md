# BorisStepper

!syntax description /UserObjects/BorisStepper

The equations of motion for a charged particle in an electric and magnetic field are

\begin{equation}
  \frac{d\vec{r}}{dt}
  =
  \vec{v}
\end{equation}

\begin{equation}
  \frac{d\vec{v}}{dt}
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

In the boris algorithm the acceleration due to the electric and magnetic fields are seperated and approximated with a central finite difference scheme. For time step, $n$, the velocity update scheme is given by

\begin{equation}
  \vec{v}^{\,-}
  =
  \vec{v}_{n}
  +
  \frac{q}{m}
  \vec{E}_n
  \frac{\Delta t}{2}
\end{equation}

\begin{equation}
  \vec{v}^{\,+}
  =
  \vec{v}^{\,-}
  +
  \vec{v}^{\,'}
  \times
  \vec{s}
\end{equation}

\begin{equation}
  \vec{v}^{\,'}
  =
  \vec{v}^{\,-}
  +
  \vec{v}^{\,-}
  \times
  \vec{l}
\end{equation}

\begin{equation}
  \vec{l} =
  \frac{q}{m}
  \vec{B}_n
  \Delta t
\end{equation}

\begin{equation}
  \vec{s} =
  \frac{2 \vec{l}}{
    1 + \vec{l} \cdot \vec{l}
  }
\end{equation}

\begin{equation}
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
  \vec{B}_n
\end{equation}

\begin{equation}
  \vec{v}_{n+1}
  =
  \vec{v}^{\,+}
  +
  \frac{q}{m}
  \vec{E}_n
  \frac{\Delta t}{2}
\end{equation}

More details about the Boris algorithm can be found in

!bibtex list bib_files=boris.bib

# Example Input Syntax

!listing test/tests/userobjects/boris_stepper/cyclotron_motion.i block=UserObjects

!syntax parameters /UserObjects/BorisStepper

!syntax inputs /UserObjects/BorisStepper

!syntax children /UserObjects/BorisStepper
