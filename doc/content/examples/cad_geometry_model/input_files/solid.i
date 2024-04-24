[Mesh]
  [file]
    type = FileMeshGenerator
    file = tmesh_1.e
  []
#  [del1]
#    type = PlaneDeletionGenerator
#    point = '0 0 0'
#    normal = '10 17.32 0'
#    input = file
#    new_boundary = 100
#  []
#  [del2]
#    type = PlaneDeletionGenerator
#    point = '0 0 0'
#    normal = '10 -17.32 0'
#    input = file
#    new_boundary = 101
#  []
[]

[Variables]
  [temp]
    initial_condition = 800.0
  []
[]

[AuxVariables]
  [heat_source]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[Kernels]
  [hc]
    type = HeatConduction
    variable = temp
  []
  [heat]
    type = CoupledForce
    variable = temp
    v = heat_source
  []
[]

[BCs]
  [surface]
    type = DirichletBC
    variable = temp
    boundary = 1
    value = 800.0
  []
[]

[Materials]
  [k_1]
    type = GenericConstantMaterial
    prop_values = '1.64'
    prop_names = 'thermal_conductivity'
    block = 'Armour'
  []
  [k_2]
    type = GenericConstantMaterial
    prop_values = '0.45'
    prop_names = 'thermal_conductivity'
    block = 'FW'
  []
  [k_3]
    type = GenericConstantMaterial
    prop_values = '0.65'
    prop_names = 'thermal_conductivity'
    block = 'BM'
  []
[]

[Executioner]
  type = Transient
  nl_abs_tol = 1e-8
  num_steps = 2
  solve_type = 'NEWTON'
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
  perf_graph = true
[]

[MultiApps]
  [openmc]
    type = TransientMultiApp
    app_type = CardinalApp
    input_files = 'openmc.i'
    execute_on = timestep_end
  []
[]

[Transfers]
  [heat_source_from_openmc]
    type = MultiAppGeneralFieldShapeEvaluationTransfer
    from_multi_app = openmc
    variable = heat_source
    source_variable = heat_source
    from_postprocessors_to_be_preserved = heat_source
    to_postprocessors_to_be_preserved = source_integral
  []
  [temp_to_openmc]
    type = MultiAppGeneralFieldShapeEvaluationTransfer
    to_multi_app = openmc
    variable = temp
    source_variable = temp
  []
[]

[Postprocessors]
  [source_integral]
    type = ElementIntegralVariablePostprocessor
    variable = heat_source
    execute_on = transfer
  []
  [max_T]
    type = NodalExtremeValue
    variable = temp
  []
[]
