[Mesh]
  [file]
    type = FileMeshGenerator
    file = tmesh_1.e
  []
[]

[AuxVariables]
  [cell_temperature]
    family = MONOMIAL
    order = CONSTANT
  []
[]

[AuxKernels]
  [cell_temperature]
    type = CellTemperatureAux
    variable = cell_temperature
  []
[]

[Problem]
  type = OpenMCCellAverageProblem
  tally_type = cell
  tally_name = 'heat_source H3'
  lowest_cell_level = 0
  temperature_blocks = '1 2 3'
  check_tally_sum = false
  source_strength = 1e18
  volume_calculation = vol
  tally_score = 'heating_local H3_production'
  tally_trigger = 'rel_err none'
  tally_trigger_threshold = '0.1 0.1'
  verbose = true
  max_batches = 10
  batch_interval = 5
  particles = 5000
  output = unrelaxed_tally_std_dev
  skinner = moab
[]

[UserObjects]
  [vol]
    type = OpenMCVolumeCalculation
    n_samples = 5000
  []
  [moab]
    type = MoabSkinner
    temperature_min = 800
    temperature_max = 1100
    n_temperature_bins = 100
    temperature = temp
    build_graveyard = true
  []
[]

[Postprocessors]
  [heat_source]
    type = ElementIntegralVariablePostprocessor
    variable = heat_source
  []
  [tritium_production]
    type = ElementIntegralVariablePostprocessor
    variable = H3
  []
  [tritium_RelativeError]
    type = TallyRelativeError
    tally_score = h3_production
  []
  [heat_source_RelativeError]
    type = TallyRelativeError
    tally_score = heating_local
  []
[]

[Executioner]
  type = Transient
[]

[Outputs]
  exodus = true
  csv = true
[]
