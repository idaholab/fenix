number_density = 1e16 # m^-3
# domain length
l = 0.1 # [m]
# premitity of free space
eps_0 = 8.85e-12 # [F/m]
# particle mass
m = 6.64e-26 # [kg]
# particle charge
q = 1.602e-19 # [C]
# number of elements being used
num_elem = 100
# number of points to sample the potential at
num_samples = ${fparse num_elem + 1}

[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = ${num_elem}
  xmax = ${l}
[]

[Variables]
  [phi]
  []

  [n]
  []
[]

[Kernels]
  [poissons]
    type = ADMatDiffusion
    diffusivity = ${eps_0}
    variable = phi
  []

  [projection]
    type = ProjectionKernel
    variable = n
  []
[]

[BCs/zero]
  type = DirichletBC
  variable = phi
  boundary = 'left right'
  value = 0
[]

[Functions]
  [potential_ic_func]
    type = ParsedFunction
    symbol_names = 'l q n_i eps_0'
    symbol_values = '${l} ${q} ${number_density} ${eps_0}'
    expression = '0.5 * q * n_i / eps_0 * ((l / 2)^2 - (x-l/2)^2)'
  []
[]

[ICs]
  [potential_ic]
    type = FunctionIC
    variable = phi
    function = 'potential_ic_func'
  []
[]

[AuxVariables]
  [Ex]
    order = CONSTANT
    family = MONOMIAL
  []
  [Ey]
  []
  [Ez]
  []
[]

[AuxKernels]
  [grad_potential]
    type = NegativeVariableGradientComponent
    variable = Ex
    gradient_variable = phi
    component = 0
    execute_on = 'INITIAL TIMESTEP_END'
  []
[]


[Distributions]
  [zero]
    type = Constant
    value = 0
  []
[]
[UserObjects]
  [stepper]
    type = LeapFrogStepper
    field_components = 'Ex Ey Ez'
  []

  [initializer]
    type = UniformGridParticleInitializer
    mass = ${m}
    charge = ${q}
    total_particles = 100
    number_density = ${number_density}
    velocity_distributions = 'zero zero zero'
  []

  [study]
    type = TestInitializedPICStudy
    stepper = stepper
    initializer = initializer
    always_cache_traces = true
    data_on_cache_traces = true
    # use_custom_rayids = false
    particles_per_element = 1
    execute_on = 'TIMESTEP_BEGIN'
  []

  [potential_accumulator]
    type = ChargeDensityAccumulator
    study = study
    variable = phi
  []

  [density_accumulator]
    type = NumberDensityAccumulator
    study = study
    variable = n
  []
[]

[RayBCs]
  [reflect]
    type = KillRayBC
    boundary = 'left right'
    study = study
  []
[]

[RayKernels]
  [null]
    type = NullRayKernel
  []
[]

[Postprocessors]
  [ray_count]
    type = RayTracingStudyResult
    study = study
    result = 'total_rays_started'
    execute_on = 'TIMESTEP_END'
  []
[]


[VectorPostprocessors]
  [ray_data]
    type = TestParticleMotionData
    study = study
    execute_on = TIMESTEP_END
  []

  [potential]
    type = LineValueSampler
    variable = phi
    start_point = '0 0 0'
    end_point = '${l} 0 0'
    num_points = ${num_samples}
    sort_by = 'x'
    execute_on = TIMESTEP_END
  []
[]


[Executioner]
  type = Transient
  solve_type = NEWTON
  petsc_options_iname = '-pc_type -pc_factor_shift_type -pc_factor_shift_amount -pc_factor_mat_solver'
  petsc_options_value = 'lu NONZERO 1.e-9 superlu_dists'
  line_search = 'none'
  nl_max_its = 15
  l_max_its = 300
  scheme = 'bdf2'
  automatic_scaling = true
  compute_scaling_once = false
  dt = 1e-10
  end_time = 2.6e-8
  # num_steps = 1
[]
[Outputs]
  exodus = true
  [csv]
    type = CSV
    execute_on = TIMESTEP_END
    file_base = 'lieberman'
    start_step = 1
    time_step_interval = 259
  []
[]
