!include charge_accumulator_base.i

[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 3
  xmax = 1
[]

[UserObjects/initializer]
  start_points = '0.5 0 0'
[]
