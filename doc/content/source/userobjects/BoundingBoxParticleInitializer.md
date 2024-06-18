# BoundingBoxParticleInitializer

!syntax description /UserObjects/BoundingBoxParticleInitializer

!alert note
This method does not give explict control over the total number of macro particles per element that will be created. If an entire element is contained within the bounding box, however, if a part of an element is contained within a bounding box then that element will contain the requested particles per element. All other elements on average will contain a number of particles proportional to fraction of the its volume contained in the bounding box.

## Example Input Syntax

!listing test/tests/userobjects/particles_per_element_initializer/elements/1d.i block=UserObjects

!syntax parameters /UserObjects/BoundingBoxParticleInitializer

!syntax inputs /UserObjects/BoundingBoxParticleInitializer

!syntax children /UserObjects/BoundingBoxParticleInitializer

!bibtex bibliography
