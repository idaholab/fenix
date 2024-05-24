# InitializerBase

`InitializerBase` is the abstract class form which all initializers should be derived. The method `getParticleData` should be called by a PIC study and will return a list of `InitialParticleData` structs which contain all of the information needed for a PIC study to create and place particles in the finite element mesh.
