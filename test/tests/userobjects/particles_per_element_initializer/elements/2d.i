particles_per_element = 10
charge_density = 4
[GlobalParams]
  particles_per_element = ${particles_per_element}
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 2
    nx = 10
    ny = 10
    xmax = 1
    ymax=1
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
  [potential_solution]
    type = FunctionDirichletBC
    variable = phi
    function = potential
    boundary = 'left right top bottom'
  []
[]


[AuxVariables]
  [dump_value]
  []
[]

[AuxKernels]
  [dump_value]
    type = TagVectorAux
    variable = dump_value
    vector_tag = dump_value
    v = phi
  []
[]

[Distributions]
  [zero]
    type = Constant
    value = 0.0
  []
[]

[UserObjects]
  [initializer]
    type = ParticlesPerElementInitializer
    mass = 1
    charge = 1
    number_density = ${charge_density}
    velocity_distributions = 'zero zero zero'
  []

  [updater]
    type = TestSimpleStepper
  []

  [study]
    type = TestInitializedPICStudy
    initializer = initializer
    velocity_updater = updater
    always_cache_traces = true
    data_on_cache_traces = true
    execute_on=TIMESTEP_BEGIN
  []

  [potential_accumulator]
    type = ChargeDensityAccumulator
    study = study
    variable = phi
    extra_vector_tags = dump_value
  []

  [density_accumulator]
    type = ChargeDensityAccumulator
    study = study
    variable = n
  []
[]

[RayKernels]
  [null]
    type = NullRayKernel
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
  dt = 1
  num_steps = 1
[]

[Problem]
  kernel_coverage_check = false
[]


[Functions]
  [potential]
    type = ParsedFunction
    expression = 'x * (1 - x) + y * (1 - y)'
  []

  [density]
    type = ParsedFunction
    expression = ${charge_density}
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

  [particles_per_element]
    type = ConstantPostprocessor
    value = ${particles_per_element}
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
  [rays]
    type = RayTracingExodus
    study = study
    output_data_names = 'charge weight mass'
    execute_on = TIMESTEP_END
  []
[]
