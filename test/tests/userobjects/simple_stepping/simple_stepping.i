[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 2
  nx = 5
  ny = 5
  xmax = 10
  ymax = 10
[]

[UserObjects]
  [stepper]
    type = VelocityUpdaterBase
  []

  [study]
    type = TestPICStudyUserDefinedRays
    start_points = '1.0 1.0 0.0
                    9.0 8.0 0.0
                    4.5 3.3 0.0'

    start_velocities = '0.5 0.1 4
                       -1.34 -1.63  7
                        0.5 0.9 10'

    execute_on = 'TIMESTEP_BEGIN'
    always_cache_traces = true
    data_on_cache_traces = true
    velocity_updater = stepper
  []
[]

[RayBCs]
  [reflect]
    type = ReflectRayBC
    boundary = 'top bottom right left'
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
  output_data = true
  execute_on = TIMESTEP_BEGIN
[]
