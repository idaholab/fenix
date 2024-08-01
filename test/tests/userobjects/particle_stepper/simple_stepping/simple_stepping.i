[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 3
  nx = 5
  ny = 5
  nz = 5
  xmax = 10
  ymax = 10
  zmax = 10
[]

[UserObjects]
  [stepper]
    type = TestSimpleStepper
  []

  [initializer]
    type = TestPlacedParticleInitializer
    start_points = '1.0 1.0 0.0
                    9.0 8.0 0.0
                    4.5 3.3 0.0'

    start_velocities = '0.5 0.1 4.1
                       -1.34 -1.63  6.3
                        0.5 0.9 10.8'
  []

  [study]
    type = TestInitializedPICStudy
    stepper = stepper
    initializer = initializer
    use_custom_rayids = false
    always_cache_traces = true
    data_on_cache_traces = true
    execute_on = 'TIMESTEP_BEGIN'
  []
[]

[RayBCs]
  [reflect]
    type = ReflectRayBC
    boundary = 'top bottom right left back front'
    study = study
  []
[]

[RayKernels]
  [null]
    type = NullRayKernel
  []
[]

[Executioner]
  type = Transient
  num_steps = 10
[]

[Problem]
  solve = false
[]

[Outputs/rays]
  type = RayTracingExodus
  study = study
  output_data_names='v_x v_y v_z weight'
  execute_on = TIMESTEP_BEGIN
[]
