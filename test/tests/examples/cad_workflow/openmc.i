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
  cell_level = 0
  temperature_blocks = '1 2 3'

  source_strength = 1e18 # Particles/s.
  volume_calculation = volume

  verbose = true
  # this is a low number of particles; you will want to increase in order to obtain
  # high-quality results
  first_iteration_particles = 1000
  relaxation = dufek_gudowski

  skinner = moab
  [Tallies]
    [tally1]
      type = MeshTally
      mesh_template = tmesh_1.e
      score = 'heating_local H3_production'
      output = unrelaxed_tally_std_dev
    []
  []
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
    n_temperature_bins = 10
    temperature = temp
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
    variable = H3_production
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
