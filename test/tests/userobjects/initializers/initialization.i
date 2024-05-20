[GlobalParams]
  particles_per_element = 10
[]

[Mesh]
  [gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 10
  xmax = 10
  []
  allow_renumbering = false
[]

[Problem]
  extra_tag_vectors = dump_value
[]

[Variables]
  [phi]
  []

  [n]
  []
[]

[Kernels]
  [poissons]
    type = ADMatDiffusion
    diffusivity = 1
    variable = phi
  []

  [projection]
    type = ProjectionKernel
    variable = n
  []
[]

[BCs]
  [zero]
  type = DirichletBC
  variable = phi
  value = 0
  boundary = 'left right'
  preset = false
  []
[]

[AuxVariables]
  [dump_value]
  []
  [density_test]
  []
[]

[AuxKernels/dump_value]
  type = TagVectorAux
  variable = dump_value
  vector_tag = dump_value
  v = phi
[]

[Distributions]
  [small_const]
    type = Constant
    value = 1e-5
  []

  [const]
    type = Constant
    value = 0.0
  []
[]

[UserObjects]
  [testing]
    type = ParticlesPerElementInitializer
    mass = 1
    charge = 1
    charge_density = 2
    velocity_distributions = 'small_const const const'
  []

  [stepper]
    type = TestSimpleStepper
  []

  [study]
    type = InitializedPICStudy
    initializer = testing
    always_cache_traces = true
    data_on_cache_traces = true
    velocity_updater = stepper
    execute_on=TIMESTEP_BEGIN
  []

  [accumulator]
    type = ChargeAccumulator
    study = study
    variable = phi
    extra_vector_tags = dump_value
  []

  [density_accumulator]
    type = ChargeAccumulator
    study = study
    variable = n
  []
[]

[RayKernels]
  [null]
    type = NullRayKernel
  []
[]

[RayBCs]
  [reflect]
    type = ReflectRayBC
    boundary = 'left right'
    study = study
  []
[]


[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_shift_type -pc_factor_shift_amount -pc_factor_mat_solver'
  petsc_options_value = 'lu NONZERO 1.e-9 superlu_dists'
  line_search = 'none'
  nl_max_its = 15
  l_max_its = 300
  scheme = 'bdf2'
  automatic_scaling = true
  compute_scaling_once = false
  dt=1
  num_steps = 1
[]



[Problem]
  kernel_coverage_check = false
[]


[Functions]
  [potential]
    type = ParsedFunction
    expression = 'x * (1 - x)'
  []

  [density]
    type = ParsedFunction
    expression = 2
  []
[]

[Postprocessors]
  [potential_l2]
    type = ElementL2Error
    variable = phi
    function = potential
  []

  [density_l2]
    type = ElementL2Error
    variable = n
    function = density
  []

  [h]
    type = AverageElementSize
  []
[]

[Outputs]
  exodus = true
  # csv = true
  [csv]
    type = CSV
    execute_on = 'FINAL'
    file_base = 'test'
  []
  [rays]
    type = RayTracingExodus
    study = study
    output_data_names = 'v_x v_y v_z weight'
    execute_on = TIMESTEP_END
  []
[]
