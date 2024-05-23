[Problem]
  solve = false
[]

[GlobalParams]
  particles_per_element = 10
[]

[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
    dim = 1
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
    charge_density = 2
    velocity_distributions = 'zero zero zero'
  []

  [velocity_updater]
    type = TestSimpleStepper
  []

  [study]
    type = InitializedPICStudy
    initializer = initializer
    velocity_updater = velocity_updater
    always_cache_traces = true
    data_on_cache_traces = true
    execute_on=TIMESTEP_BEGIN
  []

  [potential_accumulator]
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

[Executioner]
  type = Steady
[]
