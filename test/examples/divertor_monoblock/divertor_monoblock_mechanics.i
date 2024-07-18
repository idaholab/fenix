# This input file extends divertor_monoblock_1.i to add mechanics.

# Mechanics properties
# "Youngs's Modulus and Poisson's Ratio Characterization of Tungsten Thin Films via Laser Ultrasound" "Thermal Expansion of Oericlase (MgO) and Tungsten (W) to Melting Temperatures"
youngs_modulus_W = ${units 4.05e11 Pa}
poissons_ratio_W = 2.80E-01
thermal_expansion_coeff_W = 1.41e-05
stress_free_temperature_W = ${units 300 K}
# https://www.mit.edu/~6.777/matprops/copper.htm
youngs_modulus_Cu = ${units 1.30e11 Pa}
poissons_ratio_Cu = 0.34
thermal_expansion_coeff_Cu = 1.70e-05
stress_free_temperature_Cu = ${units 300 K}
# define as identical to Cu for now ########### will need to be updated
youngs_modulus_CuCrZr = ${units 1.30e11 Pa}
poissons_ratio_CuCrZr = 0.34
thermal_expansion_coeff_CuCrZr = 1.70e-05
stress_free_temperature_CuCrZr = ${units 300 K}

# Include base input file with heat conduction and TMAP8
!include divertor_monoblock_HC_TMAP8_Base.i

[GlobalParams]
    displacements = 'disp_x disp_y disp_z'
[]

[Problem]
    group_variables = 'disp_x disp_y disp_z'
    converge_on = 'disp_x disp_y disp_z temperature C_mobile_W C_trapped_W C_mobile_Cu C_trapped_Cu C_mobile_CuCrZr C_trapped_CuCrZr'
[]

## Mesh generated by ConcentricCircleMeshGenerator and saved as "2DMonoblock.e"
[Mesh]
    [pin]
        type = ExtraNodesetGenerator
        input = bdg
        new_boundary = pin
        coord = '${fparse -pitch/2} ${fparse -pitch/2} 0'
      []
    [pin2]
        type = ExtraNodesetGenerator
        input = pin
        new_boundary = pin2
        coord = '${fparse -pitch/2} ${fparse -pitch/2} ${thickness_monoblock}'
    []
    [pin3]
        type = ExtraNodesetGenerator
        input = pin2
        new_boundary = pin3
        coord = '0 ${fparse -pitch/2} ${thickness_monoblock}'
    []
[]

[Physics/SolidMechanics/QuasiStatic]
    [all]
      add_variables = true
      strain = FINITE
    #   incremental = true
      automatic_eigenstrain_names = true
      generate_output = 'vonmises_stress hydrostatic_stress'
      material_output_order = SECOND
      extra_vector_tags = 'ref'
      temperature = temperature
    []
[]

[BCs]
    ############################## BC for mechanics
    [bottom_x]
        type = DirichletBC
        variable = disp_x
        boundary = 'right'
        value = 0
    []
    [bottom_y]
        type = DirichletBC
        variable = disp_y
        boundary = 'pin pin2'
        value = 0
    []
    [bottom_z]
        type = DirichletBC
        variable = disp_z
        boundary = 'pin pin3'
        value = 0
    []
[]

[Materials]
    ############################## Materials for mechanics
    [elasticity_W]
        type = ComputeIsotropicElasticityTensor
        youngs_modulus = ${youngs_modulus_W}
        poissons_ratio = ${poissons_ratio_W}
        block = 4
    []
    [thermal_expansion_W]
        type = ComputeThermalExpansionEigenstrain
        temperature = temperature
        thermal_expansion_coeff = ${thermal_expansion_coeff_W}
        stress_free_temperature = ${stress_free_temperature_W}
        eigenstrain_name = thermal_expansion
        block = 4
    []
    [elasticity_Cu]
        type = ComputeIsotropicElasticityTensor
        youngs_modulus = ${youngs_modulus_Cu}
        poissons_ratio = ${poissons_ratio_Cu}
        block = 3
    []
    [thermal_expansion_Cu]
        type = ComputeThermalExpansionEigenstrain
        temperature = temperature
        thermal_expansion_coeff = ${thermal_expansion_coeff_Cu}
        stress_free_temperature = ${stress_free_temperature_Cu}
        eigenstrain_name = thermal_expansion
        block = 3
    []
    [elasticity_CuCrZr]
        type = ComputeIsotropicElasticityTensor
        youngs_modulus = ${youngs_modulus_CuCrZr}
        poissons_ratio = ${poissons_ratio_CuCrZr}
        block = 2
    []
    [thermal_expansion_CuCrZu]
        type = ComputeThermalExpansionEigenstrain
        temperature = temperature
        thermal_expansion_coeff = ${thermal_expansion_coeff_CuCrZr}
        stress_free_temperature = ${stress_free_temperature_CuCrZr}
        eigenstrain_name = thermal_expansion
        block = 2
    []
    [stress]
        type = ComputeFiniteStrainElasticStress
    []
[]
