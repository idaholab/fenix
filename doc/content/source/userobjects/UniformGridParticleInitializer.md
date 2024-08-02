# UniformGridParticleInitializer

!syntax description /UserObjects/UniformGridParticleInitializer

!alert warning
This initializer has only been implemented for a one dimensional domain

The initializer first computes the total length of the domain and how much the domain is owned by each process. The total number of particles requested is then divided between the processors, with the division being proportional to the fraction of the total domain each process is responsible for. Each process then calculates the space that should be between each particle based on the total length of the domain. Each process then places particles evenly on its section of the domain using a spacing calculated based on the total length of the domain and the total number of particles requested.

!alert note title=Initializer Behaviour
Because this initializer divides the work base on the fraction of the total domain each process should own there is a possibility that the total number of particles created will not exactly equal the total number of particles requested. Additionally, because the global spacing is used when place particles there is a potnetial that particles will not be evenly spaced globally depending on how the mesh is partitioned between processes.

## Example Input Syntax

!listing test/tests/benchmarking/lieberman.i block=UserObjects/initializer

!syntax parameters /UserObjects/UniformGridParticleInitializer

!syntax inputs /UserObjects/UniformGridParticleInitializer

!syntax children /UserObjects/UniformGridParticleInitializer
