[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 3
  nx = 4
  ny = 5
  nz = 6
[]

[AuxVariables/value]
[]

[UserObjects/auxaccumulator]
  type = TestAuxAccumulator
  variable = value
  execute_on = TIMESTEP_END
[]

[Problem]
  solve = false
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]
