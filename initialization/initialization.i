[StochasticTools]
[]

[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 25
  xmax = 22
[]

[Distributions]
  [uniform]
    type = Uniform
    lower_bound = 0
    upper_bound = 1
    execute_on = 'initial'
  []
[]

[Samplers]
  [sampler]
    type = LatinHypercube
    num_rows = 10
    distributions = 'uniform'
    execute_on = 'initial'
  []
[]

[UserObjects]
  [study]
    type = OneDPIC
    sampler = sampler
    execute_on = 'TIMESTEP_BEGIN'
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
