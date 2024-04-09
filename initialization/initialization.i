[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 25
  xmax = 22
[]


[UserObjects]
  [study]
    type = OneDPIC
  []
[]


[Executioner]
  type = Steady
  # type = Transient
  # dt = 1e-2
  # num_steps = 10
[]

[Problem]
  solve = false
  kernel_coverage_check = false
[]

# [Outputs/rays]
#   type = RayTracingExodus
#   study = study
#   execute_on = TIMESTEP_BEGIN
# []
