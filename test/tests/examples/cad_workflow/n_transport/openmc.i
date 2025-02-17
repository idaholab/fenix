[Mesh]
  [file]
    type = FileMeshGenerator
    file = tmesh_1.e
  []
  allow_renumbering = false
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
  cell_level = 0
  temperature_blocks = '1 2 3'
  temperature_variables = 'temperature'

  source_strength = 1e18 # Particles/s.
  volume_calculation = volume

  verbose = true
  # this is a low number of particles; you will want to increase in order to obtain
  # high-quality results
  particles = 10
  batches = 2
  relaxation = robbins_monro

  skinner = moab
  [Tallies]
    [heating_tritium]
      type = MeshTally
      score = 'heating_local H3_production'
      name = 'heating_local tritium_production'
      output = unrelaxed_tally_std_dev
    []
  []
[]

[UserObjects]
  [volume]
    type = OpenMCVolumeCalculation
    n_samples = 5000
  []
  [moab]
    type = MoabSkinner
    temperature_min = 800
    temperature_max = 3000
    n_temperature_bins = 10
    temperature = temperature
    build_graveyard = true
  []
[]

[Postprocessors]
  [heat_source]
    type = ElementIntegralVariablePostprocessor
    variable = heating_local
  []
  [tritium_production]
    type = ElementIntegralVariablePostprocessor
    variable = tritium_production
  []
  [tritium_RelativeError]
    type = TallyRelativeError
    tally_score = H3_production
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
