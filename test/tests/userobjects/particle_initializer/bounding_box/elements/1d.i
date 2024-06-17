!include initializer_base.i

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 1
    nx = 10
    xmax = 10
  []
  allow_renumbering = false
[]

