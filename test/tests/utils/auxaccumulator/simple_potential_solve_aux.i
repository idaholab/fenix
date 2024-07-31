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

  [study]
    type = TestChargeAccumulator
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

    execute_on = 'TIMESTEP_BEGIN'
    always_cache_traces = true
    data_on_cache_traces = true
    stepper = stepper
    variable = rho
  []
[]

[RayKernels]
  [null]
    type = NullRayKernel
  []
[]

[RayBCs]
  [reflect]
    type = ReflectRayBC
    boundary = 'left right'
    study = study
  []
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]
