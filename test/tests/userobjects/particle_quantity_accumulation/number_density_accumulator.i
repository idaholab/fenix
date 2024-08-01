[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 4
  xmax = 1
[]

[Problem]
  extra_tag_vectors = dump_value
[]

[Variables/n]
[]

[Kernels/null]
  type = NullKernel
  variable = n
[]

[AuxVariables/dump_value]
[]

[AuxKernels/dump_value]
  type = TagVectorAux
  variable = dump_value
  vector_tag = dump_value
  v = n
[]

[UserObjects]
  [stepper]
    type = TestSimpleStepper
  []

  [initializer]
    type = TestPlacedParticleInitializer
    start_points = '0 0 0
                    0.25 0 0
                    0.5 0 0
                    0.75 0 0
                    1 0 0'
    start_velocities = '0 0 0
                        0 0 0
                        0 0 0
                        0 0 0
                        0 0 0'
    charge = 0.5
    weight = 1
  []


  [study]
    type = TestInitializedPICStudy
    stepper = stepper
    initializer = initializer
    always_cache_traces = true
    data_on_cache_traces = true
    use_custom_rayids = false
    execute_on = 'TIMESTEP_BEGIN'
  []

  [accumulator]
    type = NumberDensityAccumulator
    study = study
    variable = n
    vector_tags = dump_value
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
  exodus = true

  [rays]
    type = RayTracingExodus
    study = study
    output_data_names = 'charge weight'
    execute_on = 'TIMESTEP_END'
  []
[]
