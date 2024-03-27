# ParticleStepperBase

`ParticleStepperBase` is the abstract class form which all steppers should be derrived. `ParticleStepperBase` provides the basic method to update a particles direction and maximum distance based on the veloctiy stored in ray data, the current time step and dimension of the mesh. Addtionally, several other useful methods are provided. In the provided method `setMaxDistanceAndDirection` the particles velocity is updated to the newly calculated velocity, the direction is set based on the dimension of the mesh, and the maximum distance is set with the following rule.

\begin{equation}
  d_\text{max} = \vec{v} \cdot \vec{v} \: dt,
\end{equation}
with ....

However, only the components of the velocity data corresponding to the dimension of the mesh will be used in the calculation. If the mesh is 2D then only the first 2 components of velocity will be used for this calculation. This calculation is performed with `ParticleStepperBase::setupStep`. This function takes an argument of the ray that needs its velocity updated, the new velocity of the ray and a time step to be used for the maximum distance calculation to allow flexibility for particle pushing methods.

!syntax parameters /UserObjects/ParticleStepperBase

!syntax inputs /UserObjects/ParticleStepperBase

!syntax children /UserObjects/ParticleStepperBase
