[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 10
  xmax = 1
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
  [uniform]
    type = Uniform
    lower_bound = -1
    upper_bound = 1
  []
[]

[Samplers]
  [sampler]
    # type = LatinHypercube
    type = MonteCarlo
    distributions = 'uniform'
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
    type = OneDPIC
    sampler = sampler
    # always_cache_traces = true
    # data_on_cache_traces = true
    mass = 1
    charge = 1
    charge_density = 2
    particles_per_element = 10000
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
  num_steps = 1
  dt=1
[]

[Problem]
  kernel_coverage_check = false
[]


[VectorPostprocessors]
  [potential]
    type = LineValueSampler
    variable = phi
    start_point = '0.0 0.0 0.0'
    end_point = '1.0 0.0 0.0'
    sort_by = x
    num_points = 1000
  []

  [density]
    type = LineValueSampler
    variable = n
    start_point = '0.0 0.0 0.0'
    end_point = '1.0 0.0 0.0'
    sort_by = x
    num_points = 1000
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
