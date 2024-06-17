!include initializer_base.i

[Mesh]
  [gmg]
  type = GeneratedMeshGenerator
  dim = 3
  nx = 2
  ny = 2
  nz = 2
  xmax = 1
  ymax=1
  zmax=1
  []
  allow_renumbering = false
[]

[BCs]
  [potential_solution]
    type = FunctionDirichletBC
    variable = phi
    function = potential
    boundary = 'left right top bottom back front'
  []
[]

[Functions]
  [potential]
    type = ParsedFunction
    expression = 'x * (1 - x) + y * (1 - y) + z * (1 - z)'
  []
[]
