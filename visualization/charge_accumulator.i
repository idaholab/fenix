[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 3
  xmax = 1
[]



[Variables]
  [phi]
  []
[]

[Kernels]
  [charge_source]
    type = ADCoupledForce
    variable = phi
    v = F
  []
[]

[AuxVariables]
  [F]
  []
[]

[AuxKernels]
  [value]
    type = ConstantAux
    variable = F
    value = 1
  []
[]

# [UserObjects]
#   [stepper]
#     type = TestSimpleStepper
#   []

#   [study]
#     type = TestChargeAccumulator
#     start_points = '0.5 0 0'

#     start_velocities = '0 0 0'

#     execute_on = 'TIMESTEP_BEGIN'
#     always_cache_traces = true
#     data_on_cache_traces = true
#     velocity_updater = stepper
#     variable = F
#   []
# []

# [RayBCs]
#   [reflect]
#     type = ReflectRayBC
#     boundary = 'left right'
#     study = study
#   []
# []

# [RayKernels]
#   [null]
#     type = NullRayKernel
#   []
# []

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]
