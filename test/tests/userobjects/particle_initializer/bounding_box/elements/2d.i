!include initializer_base.i

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 10
    ny = 10
    xmax = 10
    ymax = 10
    elem_type = TRI3
  []
  allow_renumbering = false
[]
