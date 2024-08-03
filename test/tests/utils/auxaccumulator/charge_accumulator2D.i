!include charge_accumulator_base.i

[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 2
  nx = 3
  ny = 3
  xmax = 1
  ymax = 1
[]



[UserObjects/initializer]
  start_points = '0.5 0.5 0'
[]
