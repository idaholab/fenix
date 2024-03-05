[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 2
  nx = 10
  ny = 10
[]

[Variables/value]
[]

[Kernels/diffusion]
  type = Diffusion
  variable = value
[]

[BCs/zero]
  type = DirichletBC
  variable = value
  value = 0
  boundary = 'left'
[]

[AuxVariables/dump_value]
[]

[AuxKernels/dump_value]
  type = TagVectorAux
  variable = dump_value
  vector_tag = dump_value
  v = value
[]

[UserObjects/residualaccumulator]
  type = TestResidualAccumulator
  variable = value
  execute_on = PRE_KERNELS
  extra_vector_tags = dump_value
[]

[Problem]
  extra_tag_vectors = dump_value
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]
