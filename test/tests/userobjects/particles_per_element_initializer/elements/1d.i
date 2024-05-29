!include initializer_base.i

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 1
    nx = 10
    xmax = 1
    elem_type = EDGE2
  []
  allow_renumbering = false
[]

[BCs]
  [potential_solution]
    type = DirichletBC
    variable = phi
    value = 0
    boundary = 'left right'
    preset = false
  []
[]

[Functions]
  [potential]
    type = ParsedFunction
    expression = 'x * (1 - x)'
  []
[]
