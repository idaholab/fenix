#Operating Conditions
T_in = 423 # K  "Thermal-hydraulic modelling and analysis of ITER tungsten divertor monoblock"
p_out = 5e6 # Pa "Thermal-hydraulic modelling and analysis of ITER tungsten divertor monoblock"
v_in = 16 #m/s "Thermal-hydraulic modelling and analysis of ITER tungsten divertor monoblock"

#Channel Geometry
radius = ${units 6 mm -> m}
wetted_perimeter = ${fparse 2 * 3.14159 * radius} # perimeter
area = ${fparse 3.14159 * radius * radius} # area
D_h = ${fparse (4*area)/wetted_perimeter}

#Fluid Properties (Water at 5Mpa and 423K https://www.peacesoftware.de/einigewerte/wasser_dampf_e.html)
rho = 919.69
mu =  0.00018379
k = 0.6851
cp = 4.29547e3

#interp methods
velocity_interp_method = 'rc'
advected_interp_method = 'average'

[GlobalParams]
  rhie_chow_user_object = 'rc'
[]

[Mesh]
  [circle]
    type = AnnularMeshGenerator
    nt = 20
    nr = 20
    rmax = 0.006
    rmin = 0
    dmin = 90
    dmax = 270
    growth_r = 0.9
  []
  [extrude]
    type = MeshExtruderGenerator
    input = circle
    extrusion_vector = '0 0 0.006'
    num_layers = 20
    bottom_sideset = 'inlet'
    top_sideset = 'outlet'
  []
  construct_side_list_from_node_list=true
[]

[UserObjects]
  [rc]
    type = INSFVRhieChowInterpolator
    u = vel_x
    v = vel_y
    w = vel_z
    pressure = pressure
  []
[]

[Variables]
  [vel_x]
    type = INSFVVelocityVariable
  []
  [vel_y]
    type = INSFVVelocityVariable
  []
  [vel_z]
    type = INSFVVelocityVariable
    initial_condition = ${v_in}
  []
  [pressure]
    type = INSFVPressureVariable
  []
  [T_fluid]
    type = INSFVEnergyVariable
    initial_condition = ${T_in}
  []
[]

[AuxVariables]
  [power_density]
    type = MooseVariableFVReal
    initial_condition = 1e4
  []
  [Hw]
    type = MooseVariableFVReal
    initial_condition = 100
  []
  [T_wall]
    type = MooseVariableFVReal
    initial_condition = ${T_in}
  []
[]

[AuxKernels]
  [q]
    type = ParsedAux
    variable = power_density
    coupled_variables = 'Hw T_wall T_fluid'
    expression = 'Hw*(T_wall-T_fluid)'
  []
  [Hw_ak]
    type = ADMaterialRealAux
    variable = Hw
    property = 'Hw'
  []
[]

[FunctorMaterials]
  [Enthalpy]
    type = INSFVEnthalpyFunctorMaterial
    rho = ${rho}
    temperature = 'T_fluid'
  []
  [functor_constants]
    type = ADGenericFunctorMaterial
    prop_names = 'cp'
    prop_values = '${cp}'
  []
[]

[Materials]
  [Hw_material]
    type = ADWallHeatTransferCoefficient3EqnDittusBoelterMaterial
    rho = ${rho}
    vel = vel_Z
    D_h = ${D_h}
    k = ${k}
    mu = ${mu}
    cp = ${cp}
    T = T_Fluid
    T_wall = T_Wall
  []
  [mat_vel_z]
    type = ADParsedMaterial
    property_name = vel_Z
    coupled_variables = vel_z
    expression = vel_z
  []
  [mat_T_fluid]
    type = ADParsedMaterial
    property_name = T_Fluid
    coupled_variables = T_fluid
    expression = T_fluid
  []
  [mat_T_wall]
    type = ADParsedMaterial
    property_name = T_Wall
    coupled_variables = T_wall
    expression = T_wall
  []
[]

[UserObjects]
  [T_uo]
    type = LayeredAverage
    direction = z
    variable = T_fluid
    num_layers = 20
  []
  [Hw_uo]
    type = LayeredAverage
    direction = z
    variable = Hw
    num_layers = 20
  []
[]

[Physics]
  [NavierStokes]
    [Flow]
      [flow]
        compressibility = 'incompressible'

        velocity_variable = 'vel_x vel_y vel_z'

        density = 'rho'
        dynamic_viscosity = 'mu'

        initial_velocity = '0 0 ${v_in}'
        initial_pressure = '${p_out}'

        inlet_boundaries = 'inlet'
        momentum_inlet_types = 'fixed-velocity'
        momentum_inlet_functors = '$0 0 ${v_in}'

        wall_boundaries = '1 2 3'
        momentum_wall_types = 'noslip symmetry symmetry'

        outlet_boundaries = 'outlet'
        momentum_outlet_types = 'fixed-pressure'
        pressure_functors = '${p_out}'

        mass_advection_interpolation = 'average'
        momentum_advection_interpolation = 'average'
      []
    []
    [FluidHeatTransfer]
      [energy]
        coupled_flow_physics = flow

        fluid_temperature_variable = 'T_fluid'

        thermal_conductivity = 'k'
        specific_heat = 'cp'

        initial_temperature = '${T_in}'

        energy_inlet_types = 'fixed-temperature'
        energy_inlet_functors = '${T_in}'
        energy_wall_types = 'heatflux'
        energy_wall_functors = 'q'

        #external_heat_source = 'power_density'
        #energy_advection_interpolation = 'average'
      []
    []
    [Turbulence]
      [turbulence]
        coupled_flow_physics = flow
        fluid_heat_transfer_physics = energy
      []
    []
  []
[]

[Executioner]
  type = Transient
  solve_type = 'NEWTON'
  petsc_options = '-snes_converged_reason -ksp_converged_reason'
  petsc_options_iname = '-pc_type'
  petsc_options_value = 'lu'
  line_search = 'none'
  nl_rel_tol = 1e-8
  nl_abs_tol = 1e-10
  nl_max_its = 10
  end_time = 15
  dtmax = 2e-2
  dtmin = 1e-5
  scheme = 'bdf2'
  [TimeStepper]
    type = IterationAdaptiveDT
    dt = 1e-3
    optimal_iterations = 6
    growth_factor = 1.5
  []
[]

[Outputs]
  exodus = true
[]
