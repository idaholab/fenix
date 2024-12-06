import openmc
import math

mat1 = openmc.Material(name="mat1")
mat1.set_density('g/cc', 19.30)
mat1.add_element('W', 1.0)

eurofer = openmc.Material(name="eurofer")
eurofer.add_element('Fe', 0.011   , 'wo')
eurofer.add_element('Al', 0.002   , 'wo')
eurofer.add_element('As', 0.0002  , 'wo')
eurofer.add_element('B', 0.0012  , 'wo')
eurofer.add_element('C', 0.0005  , 'wo')
eurofer.add_element('Co', 0.0005  , 'wo')
eurofer.add_element('Cr', 0.0005  , 'wo')
eurofer.add_element('Cu', 0.00005 , 'wo')
eurofer.add_element('Mn', 0.00005 , 'wo')
eurofer.add_element('Mo', 0.0001  , 'wo')
eurofer.add_element('N', 0.0001  , 'wo')
eurofer.add_element('Nb', 0.00005 , 'wo')
eurofer.add_element('Ni', 0.0003  , 'wo')
eurofer.add_element('O', 0.00005 , 'wo')
eurofer.add_element('P', 0.004   , 'wo')
eurofer.add_element('S', 0.0001  , 'wo')
eurofer.add_element('Sb', 0.09    , 'wo')
eurofer.add_element('Sn', 0.0001  , 'wo')
eurofer.add_element('Si', 0.0011  , 'wo')
eurofer.add_element('Ta', 0.00002 , 'wo')
eurofer.add_element('Ti', 0.0005  , 'wo')
eurofer.add_element('V', 0       , 'wo')
eurofer.add_element('W', 0.0001  , 'wo')
eurofer.add_element('Zr', 0.88698 , 'wo')
eurofer.set_density("g/cm3", 7.798)

Helium = openmc.Material(name="Helium")
Helium.add_element('He', 1.0)
Helium.set_density("kg/m3", 0.166)

mat2 = openmc.Material.mix_materials([eurofer, Helium], [0.65, 0.35], 'ao', name="mat2")

beryllium = openmc.Material(name="beryllium")
beryllium.add_element('Be', 1.0)
beryllium.set_density("g/cm3", 1.85)

Li4SiO4 = openmc.Material(name="Li4SiO4")
Li4SiO4.add_element('Li', 4.0)
Li4SiO4.add_element('Si', 1.0)
Li4SiO4.add_element('O', 4.0)
Li4SiO4.set_density("g/cm3", 2.39)

mat3 = openmc.Material.mix_materials([eurofer, beryllium, Li4SiO4, Helium], [0.1, 0.37, 0.15, 0.38], 'ao',name="mat3")

mats = openmc.Materials([mat1, eurofer, Helium,  mat2, beryllium, Li4SiO4,  mat3])
mats.export_to_xml()

pz = openmc.Plot()
pz.basis = 'yz'
pz.origin = (0.0, 0.0, 0.0)
pz.width = (200.0, 200.0)
pz.pixels = (500, 500)
pz.color_by = 'material'

px = openmc.Plot()
px.basis = 'xy'
px.origin = (0.0, 0.0, 0.0)
px.width = (200, 200)
px.pixels = (500, 500)
px.color_by = 'material'

plots = openmc.Plots([pz,px])
plots.export_to_xml()

settings = openmc.Settings()
settings.dagmc = True
settings.batches = 20
settings.particles = 10000000
settings.run_mode = "fixed source"

settings.temperature = {'default': 800.0,
                        'method': 'interpolation',
                        'range': (294.0, 3000.0),
                        'tolerance': 1000.0}

source = openmc.Source()

r = openmc.stats.PowerLaw(55, 65, 1.0)
phi = openmc.stats.Uniform(0.0, 2*math.pi)
z = openmc.stats.Discrete([0,], [1.0,])
spatial_dist = openmc.stats.CylindricalIndependent(r, phi, z)

source.angle = openmc.stats.Isotropic()
source.energy = openmc.stats.Discrete([14.08e6], [1.0])
source.space=spatial_dist
settings.source = source
settings.photon_transport = True
settings.export_to_xml()

dagmc_univ = openmc.DAGMCUniverse(filename='tmesh_1.h5m')

geometry = openmc.Geometry(root=dagmc_univ)
geometry.export_to_xml()
