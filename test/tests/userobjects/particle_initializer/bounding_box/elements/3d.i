!include initializer_base.i

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 3
    nx = 5
    ny = 5
    nz = 5
    xmax = 10
    ymax = 10
    zmax = 10
  []
  allow_renumbering = false
[]
