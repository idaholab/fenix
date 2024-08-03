[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 2
  nx = 120
  ny = 20
  xmax = 120
  ymax = 20
[]

[Variables]
  [Ex]
  []
  [Ey]
  []
  [Ez]
  []
[]

[Functions]
  [E_x_ic]
    type = ParsedFunction
    expression = '5e-7'
  []
  [E_y_ic]
    type = ParsedFunction
    expression = '0'
  []
  [E_z_ic]
    type = ParsedFunction
    expression = '0'
  []
[]

[ICs]
  [Ex_ic]
    type = FunctionIC
    variable = Ex
    function = E_x_ic
  []
  [Ey_ic]
    type = FunctionIC
    variable = Ey
    function = E_y_ic
  []
  [Ez_ic]
    type = FunctionIC
    variable = Ez
    function = E_z_ic
  []
[]

[UserObjects]
  [stepper]
    type = LeapFrogStepper
    field_components = 'Ex Ey Ez'
  []

  [initializer]
    type = TestPlacedParticleInitializer
    start_points = '0 10.5 0'
    start_velocities = '0 0 0'
    mass = 9.1093837015e-31
    charge = 1.602176634e-19
  []

  [study]
    type = TestInitializedPICStudy
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
  dt = 1e-4
  num_steps = 10
[]

[Problem]
  solve = false
  kernel_coverage_check=false
[]

[Outputs/rays]
  type = RayTracingExodus
  study = study
  output_data_names='v_x v_y v_z charge mass'
  execute_on = TIMESTEP_BEGIN
[]
