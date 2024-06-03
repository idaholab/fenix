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

[UserObjects]
  [initializer]
    x1 = 1.1
    x2 = 4.25
  []
[]
