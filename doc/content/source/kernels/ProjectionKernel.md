# ProjectionKernel

!syntax description /Kernels/ProjectionKernel

## Overview

In general the projection of a function onto finite element basis functions is defined by

\begin{equation}
  \left<
    f_h - f,
    v_h
  \right>
  = 0
\end{equation}

Where $f$ is the function being projected onto the finite element mesh, $f_h$ is the projection of $f$ onto the finite element mesh, and $v_h$ represents the finite element test functions. This equation can be expanded into

\begin{equation}
  \underbrace{
  \left<
    f_h,
    v_h
  \right>}
  -
  \left<
    f,
    v_h
  \right>
    = 0
\end{equation}

This kernel represent the first term in the above equation and in FENIX is generally used for solving for the projection of discrete - on particle - quantities onto the finite element mesh.

## Example Input Syntax

!listing test/tests/userobjects/particle_initializer/per_element/elements/1d.i block=Kernels

!syntax parameters /Kernels/ProjectionKernel

!syntax inputs /Kernels/ProjectionKernel

!syntax children /Kernels/ProjectionKernel
