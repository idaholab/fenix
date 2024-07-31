[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 2
  nx = 3
  ny = 3
  xmax = 1
  ymax = 1
[]

[Problem]
  solve = false
[]

[AuxVariables]
  [F]
  []
[]

[UserObjects]
  [stepper]
    type = TestSimpleStepper
  []

  [study]
    type = TestChargeAccumulator
    start_points = '0.5 0.5 0'

    start_velocities = '0 0 0'

    execute_on = 'TIMESTEP_BEGIN'
    always_cache_traces = true
    data_on_cache_traces = true
    stepper = stepper
    variable = F
  []
[]

[RayBCs]
  [reflect]
    type = ReflectRayBC
    boundary = 'left right'
    study = study
  []
[]

[RayKernels]
  [null]
    type = NullRayKernel
  []
[]

[Executioner]
  type = Transient
  num_steps = 2
[]

[Outputs]
  exodus = true
[]
