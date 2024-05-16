[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 10
    ny = 10
    xmax = 1
    ymax = 1
    elem_type = TRI3
  []
  uniform_refine = 0
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
  [Solution]
    type = FunctionDirichletBC
    variable = phi
    function = potential
    boundary = 'left right top bottom'
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
  [uniform_x]
    type = Uniform
    lower_bound = 0
    upper_bound = 1
  []

  [uniform_y]
    type = Uniform
    lower_bound = 0
    upper_bound = 1
  []
[]

[Samplers]
  [sampler]
    # type = LatinHypercube
    type = MonteCarlo
    distributions = 'uniform_x uniform_y'
    # this needs to be _max_num_local_elems * _num_processors * _particles_per_elem
    # 270 for 3 processes with 10 particles per element
    # num_rows = 540000
    num_rows = 50000000
    seed = 10
  []
[]


[UserObjects]
  [stepper]
    type = TestSimpleStepper
  []

  [study]
    type = TwoDTriPIC
    sampler = sampler
    always_cache_traces = true
    data_on_cache_traces = true
    mass = 1
    charge = 1
    charge_density = 4
    particles_per_element = 100
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


[Functions]
  [potential]
    type = ParsedFunction
    expression = 'x * (1 - x) + y * (1 - y)'
  []

  [density]
    type = ParsedFunction
    expression = 4
  []
[]


[Problem]
  kernel_coverage_check = false
  # solve = false
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

  [csv]
    type = CSV
    execute_on = 'TIMESTEP_END'
  []

  # [rays]
  #   type = RayTracingExodus
  #   study = study
  #   output_data_names = 'v_x v_y v_z weight'
  #   execute_on = TIMESTEP_END
  # []
[]
