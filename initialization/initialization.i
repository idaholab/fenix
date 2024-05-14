[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 2
  xmax = 1
[]

[Variables]
  [Ex]
  []
  [Ey]
  []
  [Ez]
  []
[]

[Distributions]
  [uniform]
    type = Uniform
    lower_bound = 0
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
    type = LeapFrogStepper
    field_components = 'Ex Ey Ez'
  []

  [study]
    type = OneDPIC
    sampler = sampler
    execute_on = 'INITIAL'
    always_cache_traces = true
    data_on_cache_traces = true
    mass = 1
    charge = 1
    velocity_updater = stepper
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
  num_steps = 2
  # type = Transient
  # dt = 1e-2
  # num_steps = 10
[]



[Problem]
  solve = false
  kernel_coverage_check = false
[]


[Outputs/rays]
  type = RayTracingExodus
  study = study
  output_data = true
  # output_data_names='v_x v_y v_z charge mass'
  execute_on = TIMESTEP_END
[]
