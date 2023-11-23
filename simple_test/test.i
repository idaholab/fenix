[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 2
  nx = 5
  ny = 5
  xmax = 10
  ymax = 10
[]

[UserObjects]
  [study]
    type = PICStudyBase
    start_points = '1.0 1.0 0.0
                    9.0 8.0 0.0
                    4.5 3.3 0.0'

    start_velocities = '0.5 0.1 0
                       -1.34 -1.63  0
                        0.5 0.9 0'

    execute_on = 'TIMESTEP_BEGIN'
    always_cache_traces = true
  []
[]

[RayBCs]
  [reflect]
    type = ReflectRayBC
    boundary = 'top bottom right left'
    study = study
  []
[]

[Executioner]
  type = Transient
  num_steps = 10
[]

[Problem]
  solve = false
[]

[RayKernels/kernel]
  type = PICKernelBase
[]


[Outputs/rays]
  type = RayTracingExodus
  study = study
  execute_on = TIMESTEP_BEGIN
[]

