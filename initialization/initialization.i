[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 20
  xmax = 1
[]

[Problem]
  extra_tag_vectors = dump_value
[]

[Variables]
  [phi]
  []
[]

[Kernels]
  [poissons]
    type = ADMatDiffusion
    diffusivity = 1
    variable = phi
  []
[]

[BCs/zero]
  type = DirichletBC
  variable = phi
  value = 0
  boundary = 'left right'
  preset = false
[]

[AuxVariables/dump_value]
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
    type = LatinHypercube
    distributions = 'uniform'
    # this needs to be _max_num_local_elems * _num_processors * _particles_per_elem
    # 270 for 3 processes with 10 particles per element
    # num_rows = 540000
    num_rows = 5000
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
    always_cache_traces = true
    data_on_cache_traces = true
    mass = 1
    charge = 1
    charge_density = 2
    particles_per_element = 200
    velocity_updater = stepper
    execute_on=TIMESTEP_BEGIN
  []

  [accumulator]
    type = ChargeAccumulator
    study = study
    variable = phi
    extra_vector_tags = dump_value
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

[Outputs]
  exodus = true
  [rays]
    type = RayTracingExodus
    study = study
    output_data_names = 'v_x v_y v_z weight'
    execute_on = TIMESTEP_END
  []
[]

[Postprocessors/ray_distance]
  type = RayTracingStudyResult
  study = study
  result = total_rays_started
  execute_on = 'TIMESTEP_END'
[]
