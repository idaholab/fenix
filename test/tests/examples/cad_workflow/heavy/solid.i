# Conditions
initial_temperature = ${units 800.0 K}

# Material properties
thermal_conductivity_armor = ${units 1.64 W/m/K}
thermal_conductivity_FW = ${units 0.45 W/m/K}
thermal_conductivity_BM = ${units 0.65 W/m/K}

[Mesh]
  [file]
    type = FileMeshGenerator
    file = tmesh_1.e
  []
[]

[Variables]
  [temperature]
    initial_condition = ${initial_temperature}
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
    variable = temperature
  []
  [heat]
    type = CoupledForce
    variable = temperature
    v = heat_source
  []
[]

[BCs]
  [surface]
    type = DirichletBC
    variable = temperature
    boundary = 1
    value = ${initial_temperature}
  []
[]

[Materials]
  [k_1]
    type = GenericConstantMaterial
    prop_values = ${thermal_conductivity_armor}
    prop_names = 'thermal_conductivity'
    block = 'Armour'
  []
  [k_2]
    type = GenericConstantMaterial
    prop_values = ${thermal_conductivity_FW}
    prop_names = 'thermal_conductivity'
    block = 'FW'
  []
  [k_3]
    type = GenericConstantMaterial
    prop_values = ${thermal_conductivity_BM}
    prop_names = 'thermal_conductivity'
    block = 'BM'
  []
[]

[Executioner]
  type = Transient
  nl_abs_tol = 1e-8
  solve_type = 'NEWTON'
  num_steps = 2
[]

[Outputs]
  exodus = true
  print_linear_residuals = false
  perf_graph = true
  csv = true
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
    source_variable = heating_local
    from_postprocessors_to_be_preserved = heat_source
    to_postprocessors_to_be_preserved = source_integral
  []
  [temperature_to_openmc]
    type = MultiAppGeneralFieldShapeEvaluationTransfer
    to_multi_app = openmc
    variable = temperature
    source_variable = temperature
  []
[]

[Postprocessors]
  [source_integral]
    type = ElementIntegralVariablePostprocessor
    variable = heat_source
    execute_on = transfer
  []
  [max_temperature]
    type = NodalExtremeValue
    variable = temperature
  []
[]
