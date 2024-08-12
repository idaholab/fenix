import openmc
import math

##### Materials #######

W = openmc.Material(name="W_monoblock")
W.set_density('g/cc', 19.30)
W.add_element('W', 1.0)

Cu = openmc.Material(name="Cu_interlayer")
Cu.set_density('g/cc', 8.96)
Cu.add_element('Cu', 1.0)

CuCrZr = openmc.Material(name="CuCrZr_tube")
#Ref.: https://aurubis.com/dam/jcr:d6d50d64-69d6-4742-821d-8fb8b48c9044/cucrzr-c18160-pna-372_en.pdf
CuCrZr.set_density('g/cc', 8.90)
CuCrZr.add_element('Cr', 0.012)
CuCrZr.add_element('Zr', 0.0025)
CuCrZr.add_element('Cu', 0.9855)

H2O = openmc.Material(name="Water")
H2O.set_density('g/cc', 1.0)
H2O.add_element('H', 2.0)
H2O.add_element('O', 1.0)
H2O.add_s_alpha_beta('c_H_in_H2O')

mats = openmc.Materials([W, Cu, CuCrZr, H2O])
mats.export_to_xml()

##### Geometry #######

mm_to_cm = 0.1
water_channel_radius = 6 * mm_to_cm
CuCrZr_tube_OR = 7.5 * mm_to_cm
Cu_interlayer_OR = 8.5 * mm_to_cm
monoblock_side_length = 28.0 * mm_to_cm
thickness = 6.0 * mm_to_cm

water_channel = openmc.ZCylinder(r=water_channel_radius)
CuCrZr_OR = openmc.ZCylinder(r=CuCrZr_tube_OR)
Cu_OR = openmc.ZCylinder(r=Cu_interlayer_OR)

x_ngv = openmc.XPlane(-monoblock_side_length/2)
x_psv = openmc.XPlane(0)
y_ngv = openmc.YPlane(-monoblock_side_length/2)
y_psv = openmc.YPlane(monoblock_side_length/2)
z_ngv = openmc.ZPlane(0)
z_psv = openmc.ZPlane(thickness)

out = openmc.Sphere(r=50*mm_to_cm, boundary_type='vacuum')

water_region = -water_channel & -x_psv & +z_ngv & -z_psv
CuCrZr_region = +water_channel & -CuCrZr_OR & -x_psv & +z_ngv & -z_psv
Cu_region = +CuCrZr_OR & - Cu_OR & -x_psv & +z_ngv & -z_psv
W_block_region = +x_ngv & -x_psv & +y_ngv & -y_psv & +Cu_OR & +z_ngv & -z_psv
vaccum_region = ~W_block_region & -out

water_cell = openmc.Cell(name='Water_Cell')
water_cell.fill = H2O
water_cell.region = water_region

CuCrZr_cell = openmc.Cell(name='CuCrZr_Cell')
CuCrZr_cell.fill = CuCrZr
CuCrZr_cell.region = CuCrZr_region

Cu_cell = openmc.Cell(name='Cu_Cell')
Cu_cell.fill = Cu
Cu_cell.region = Cu_region

W_cell = openmc.Cell(name='W_Cell')
W_cell.fill = W
W_cell.region = W_block_region

vacuum_cell = openmc.Cell(name='Vacuum_Cell')
vacuum_cell.region = vaccum_region

root_universe = openmc.Universe(cells=(water_cell, CuCrZr_cell, Cu_cell, W_cell, vacuum_cell))
geometry = openmc.Geometry(root_universe)
geometry.export_to_xml()

##### Plotting #######

px = openmc.Plot()
px.basis = 'xy'
px.origin = (0.0, 0.0, thickness/2)
px.width = (30*mm_to_cm, 30*mm_to_cm)
px.pixels = (1000, 1000)
px.color_by = 'material'

pz = openmc.Plot()
pz.basis = 'xz'
pz.origin = (0.0, 0.0, thickness/2)
pz.width = (30*mm_to_cm, thickness)
pz.pixels = (1000, 1000)
pz.color_by = 'material'

px2 = openmc.Plot()
px2.basis = 'xy'
px2.origin = (0.0, 0.0, thickness/2)
px2.width = (100*mm_to_cm, 100*mm_to_cm)
px2.pixels = (2000, 2000)
px2.color_by = 'cell'

plots = openmc.Plots([px,pz,px2])
plots.export_to_xml()

##### Settings #######

settings = openmc.Settings()
settings.batches = 100
settings.particles = 1000000
settings.run_mode = "fixed source"
settings.photon_transport = True

settings.temperature = {'default': 800.0,
                        'method': 'nearest',
                        'range': (294.0, 2000.0),
                        'tolerance': 1000.0}

x = openmc.stats.Uniform(-monoblock_side_length/2, 0)
y = openmc.stats.Discrete([monoblock_side_length/2+5*mm_to_cm,], [1.0,])
z = openmc.stats.Uniform(0, thickness)
spatial_dist = openmc.stats.CartesianIndependent(x, y, z)

source = openmc.IndependentSource()
source.space = spatial_dist
source.angle = openmc.stats.Monodirectional([0.0,-1.0,0])
source.energy = openmc.stats.Discrete([14.08e6], [1.0])
settings.source = source

settings.export_to_xml()
