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

[UserObjects]
  [initializer]
    x1 = 1.1
    x2 = 4.25
    y1 = 3.6
    y2 = 5.7
    z1 = 2.6
    z2 = 8.3
  []
[]
