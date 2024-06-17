# the number of computational particles to be put in each element
particles_per_element = 20
# the analytic solution for the charge density
# set by cli args for each case since it is needed by all
# tests but changes based on the dimension of the problem
charge_density = 2
[Mesh]
  [gmg]
    type = GeneratedMeshGenerator
  []
  allow_renumbering = false
[]

[GlobalParams]
  particles_per_element = ${particles_per_element}
[]

[Problem]
 solve = false
 kernel_coverage_check = false
[]

[Distributions]
  [zero]
    type = Constant
    value = 0.0
  []
[]

[UserObjects]
  [initializer]
    type = BoundingBoxInitializer
    mass = 1
    charge = 1
    number_density = ${charge_density}
    bottom_left = '0 0 0'
    top_right = '1 0 0'
    velocity_distributions = 'zero zero zero'
  []

  [velocity_updater]
    type = TestSimpleStepper
  []

  [study]
    type = TestInitializedPICStudy
    initializer = initializer
    velocity_updater = velocity_updater
    always_cache_traces = true
    data_on_cache_traces = true
    execute_on=TIMESTEP_BEGIN
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

[Outputs]
  [rays]
    type = RayTracingExodus
    study = study
    output_data_names = 'charge weight mass'
    execute_on = TIMESTEP_END
  []
[]
