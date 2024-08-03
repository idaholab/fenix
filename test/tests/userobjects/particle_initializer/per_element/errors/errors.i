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
    type = PerElementParticleInitializer
    mass = 1
    charge = 1
    number_density = 2
    velocity_distributions = 'zero zero zero'
  []

  [stepper]
    type = TestSimpleStepper
  []

  [study]
    type = TestInitializedPICStudy
    initializer = initializer
    stepper = stepper
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
  type = Steady
[]
