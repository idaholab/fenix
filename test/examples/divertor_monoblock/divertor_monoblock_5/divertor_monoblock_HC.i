!include divertor_monoblock_neutronics_Base.i

[AuxVariables]
    [heat_source]
        order = CONSTANT
        family = MONOMIAL
        block = '2 3 4'
        initial_condition = 1e7
    []
    [time_dep_heat_source]
        order = CONSTANT
        family = MONOMIAL
        block = '2 3 4'
        initial_condition = 0.0
    []
[]

[Kernels]
    [heat_source_W]
        type = CoupledForce
        variable = temperature
        v = time_dep_heat_source
        block = 4
    []
    [heat_source_Cu]
        type = CoupledForce
        variable = temperature
        v = time_dep_heat_source
        block = 3
    []
    [heat_source_CuCrZr]
        type = CoupledForce
        variable = temperature
        v = time_dep_heat_source
        block = 2
    []
[]

[AuxKernels]
    [heat_source_t]
        type = ParsedAux
        variable = time_dep_heat_source
        coupled_variables = heat_source
        expression = 'heat_source * pulse_func'
        functor_names = 'pulse_func'
    []
[]

# remove [temp_top] from BC
    # [temp_top]
    #    type = FunctionNeumannBC
    #    variable = temperature
    #    boundary = 'top'
    #    function = temp_flux_bc_func
    # []

[Functions]
    ### pulse func (to be multiplied by heat_source)
    [pulse_func]
        type = ParsedFunction
        expression =   'if((t % 1600) < 100.0, 0.0   + 1.0*(t % 1600)/100,
                        if((t % 1600) < 500.0, 1.0,
                        if((t % 1600) < 600.0, 1.0 - 1.0*((t % 1600)-500)/100, 300)))'
    []
[]

### remove [temp_flux_bc_func] function
#    [temp_flux_bc_func]
#        type = ParsedFunction
#        expression =   'if((t % 1600) < 100.0, 0.0   + 1.0e7*(t % 1600)/100,
#                        if((t % 1600) < 500.0, 1.0e7,
#                        if((t % 1600) < 600.0, 1.0e7 - 1.0e7*((t % 1600)-500)/100, 300)))'
#    []

[MultiApps]
   [neutronics]
    type = TransientMultiApp
    input_files = 'neutronics.i'
    execute_on = timestep_begin
  []
[]

[Transfers]
  [heat_source_to_divertor]
    type = MultiAppGeneralFieldShapeEvaluationTransfer
    from_multi_app = neutronics
    variable = heat_source
    source_variable = heating_local
    execute_on = timestep_begin
  []
  [temperature_to_openmc]
    type = MultiAppGeneralFieldShapeEvaluationTransfer
    to_multi_app = neutronics
    variable = temp
    source_variable = temperature
    execute_on = timestep_begin
  []
[]