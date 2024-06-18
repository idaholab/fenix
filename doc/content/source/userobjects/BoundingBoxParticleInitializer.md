# BoundingBoxParticleInitializer

!syntax description /UserObjects/BoundingBoxParticleInitializer

## Overview

In this intiailizer first all of the elements that are at least partially contained within the requested bounding box are collected. Then the requested number (`particles_per_element`) of spatial locations are sampled within each element. Finally, all potential particle locations which are within the element but are not within the bounding box are discarded.

!alert note
This method does not give explict control over the number of macro particles per element that will be created. If an entire element is contained within the bounding box, then that element will contain the requested particles per element. On average elements which are partilaly contained within the bounding box will contain a number of macro particles approximately proportional to the fraction of the elements' volume that is contained within the bounding box.

## Example Input Syntax

!listing test/tests/userobjects/particle_initializer/bounding_box/elements/1d.i block=UserObjects/initializer

!syntax parameters /UserObjects/BoundingBoxParticleInitializer

!syntax inputs /UserObjects/BoundingBoxParticleInitializer

!syntax children /UserObjects/BoundingBoxParticleInitializer

!bibtex bibliography
