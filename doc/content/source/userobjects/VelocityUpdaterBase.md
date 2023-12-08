# VelocityUpdaterBase

VelocityUpdaterBase provides the basic method to update a particles direction and maximum distance based on the veloctiy stored in ray data, the current time step and dimension of the mesh. The maximum distance will be set by

\begin{equation}
  d_\text{max} = \vec{v} \cdot \vec{v} \: dt
\end{equation}

However, only the components of the velocity data corrisponding to the dimension of the mesh will be used in the calculation. If the mesh is 2D then only the first 2 components of velocity will be used for this calculation. This calculation is peformed with `VelocityUpdaterBase::updateVelocity`. This function takes an argument of the ray that needs its velocity updated, the new velocity of the ray and a time step to be used for the maximum distance calculation to allow flexibility for particle pushing methods.