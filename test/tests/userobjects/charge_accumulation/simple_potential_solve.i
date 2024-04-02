[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 4
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

[UserObjects]
  [stepper]
    type = TestSimpleStepper
  []

  [study]
    type = TestPICStudyUserDefinedRays
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
    execute_on = 'TIMESTEP_BEGIN'
    always_cache_traces = true
    data_on_cache_traces = true
    velocity_updater = stepper
  []

  [accumulator]
    type = ChargeAccumulator
    study = study
    variable = phi
    execute_on = PRE_KERNELS
    extra_vector_tags = dump_value
  []
[]

[RayBCs]
  [reflect]
    type = ReflectRayBC
    boundary = 'left right'
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
  num_steps = 2
[]

[Outputs]
  exodus = true
[]
