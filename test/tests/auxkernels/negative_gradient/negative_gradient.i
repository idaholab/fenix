[Mesh/gmg]
  type = GeneratedMeshGenerator
  dim = 1
  nx = 10
[]

[Variables/u]
[]

[Kernels/null]
  type = NullKernel
  variable = u
[]

[Functions]
  [ic_func]
    type = ParsedFunction
    expression = 'x'
  []
[]

[ICs/ic]
  type = FunctionIC
  variable = u
  function = ic_func
[]

[AuxVariables/negative_grad_u_x]
  order = CONSTANT
  family = MONOMIAL
[]

[AuxKernels/negative_grad]
  type = NegativeVariableGradientComponent
  variable = negative_grad_u_x
  gradient_variable = u
  component = 0
[]

[Executioner]
  type = Steady
[]

[Outputs]
  exodus = true
[]
