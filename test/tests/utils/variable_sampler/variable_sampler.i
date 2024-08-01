[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 3
  nx = 5
  ny = 5
  nz = 5
  xmax = 10
  ymax = 10
  zmax = 10
[]

[AuxVariables]
  [F]
  []
[]

[AuxKernels]
  [F_val]
    type = FunctionAux
    variable = F
    function = function
  []
[]

[Functions]
  [function]
    type = ParsedFunction
    expression = 'sqrt(x^2 + y^2 + z^2)'
  []
[]

[UserObjects]
  [stepper]
    type = TestSimpleStepper
  []

  [initializer]
    type = TestPlacedParticleInitializer
    start_points = '0 0 0'
    start_velocities = '1 1 1'
  []

  [study]
    type = TestVariableSampler
    field = F
    stepper = stepper
    initializer = initializer
    use_custom_rayids = false
    always_cache_traces = true
    data_on_cache_traces = true
    execute_on = 'TIMESTEP_BEGIN'
  []
[]

[RayKernels]
  [null]
    type = NullRayKernel
  []
[]

[Executioner]
  type = Transient
  num_steps = 10
[]

[Problem]
  solve = false
[]

[Outputs]
  [rays]
    type = RayTracingExodus
    study = study
    output_data_names='v_x v_y v_z weight field_value'
    execute_on = TIMESTEP_BEGIN
  []
  exodus = true
[]
