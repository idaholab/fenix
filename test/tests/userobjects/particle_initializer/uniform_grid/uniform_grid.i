[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 1
    nx = 4
  []
  allow_renumbering = false
[]

[Problem]
  solve = false
  kernel_coverage_check = false
[]

[Distributions]
  [zero]
    type = Constant
    value = 0
  []
[]

[UserObjects]
  [stepper]
    type = TestSimpleStepper
  []

  [initializer]
    type = UniformGridParticleInitializer
    mass = 1
    charge = 1
    total_particles = 4
    number_density = 1
    velocity_distributions = 'zero zero zero'
  []

  [study]
    type = TestInitializedPICStudy
    stepper = stepper
    initializer = initializer
    particles_per_element = 1
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
  num_steps = 1
[]

[Outputs]
  [rays]
    type = RayTracingExodus
    study = study
    output_data_names = 'weight'
    execute_on = 'TIMESTEP_END'
  []
[]
