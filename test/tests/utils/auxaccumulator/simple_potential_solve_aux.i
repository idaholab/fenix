[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 4
  xmax = 1
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
  [charge_source]
    type = ADCoupledForce
    variable = phi
    v = rho
  []
[]

[BCs]
  [zero]
    type = ADDirichletBC
    variable = phi
    value = 0
    boundary = 'left right'
    preset = false
  []
[]

[AuxVariables]
  [rho]
  []
[]

[UserObjects]
  [stepper]
    type = TestSimpleStepper
  []

  [initializer]
    type = TestPlacedParticleInitializer
    start_points = '0 0 0
                    0 0 0
                    0.25 0 0
                    0.25 0 0
                    0.5 0 0
                    0.5 0 0
                    0.75 0 0
                    0.75 0 0
                    1 0 0
                    1 0 0'

    start_velocities = '0 0 0
                        0 0 0
                        0 0 0
                        0 0 0
                        0 0 0
                        0 0 0
                        0 0 0
                        0 0 0
                        0 0 0
                        0 0 0'
  []

  [study]
    type = TestChargeAccumulator
    variable = rho
    stepper = stepper
    initializer = initializer
    use_custom_rayids = false
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
  type = Steady
[]

[Outputs]
  exodus = true
[]
