# CAD-based Geometry Workflow for Multiphysics Fusion Problems Using OpenMC and MOOSE

This demonstration describes a workflow for modeling fusion problems in OpenMC and MOOSE using a computer aided design (CAD)-based geometry workflow. It is based on the work published in [!cite](Eltawila2024PBNC). 

Fusion system designs are complex and require intricate and accurate models that can be best represented using CAD-based geometry. Fusion neutronics models frequently leverage on CAD based geometries. An example is the Paramak python package [!cite](shimwell2021paramak) which allows production of 3D CAD models of fusion reactors. The model shown in [paramak] is based on the same generic workflow described in this tutorial.

!media figures/cad_workflow_paramakmodel.png 
  id=paramak
  caption=Tokamak model based on CAD generated with Paramak package
  style=width:30%;display:block;margin-left:auto;margin-right:auto;

The results of the coupled neutronics and heat transfer model are shown in [tok_temps] and [tok_h3production] for the temperature distribution and tritium production rate density.

!row! style=display:inline-flex;
!col! small=12 medium=4 large=3
!media figures/cad_workflow_tokamak_temps.png 
  id=tok_temps
  caption=Example Paramak tokamak model temperature distribution result 
  style=width:130%;display:block;margin-left:auto;margin-right:auto;

!col-end!

!col! small=12 medium=4 large=3
!media figures/cad_workflow_tokamak_tritium_production.png 
  id=tok_h3production
  caption=Example Paramak tokamak model tritium production rate density result
  style=width:130%;display:block;margin-left:auto;margin-right:auto;

!col-end!
!row-end!

In this example, you'll learn how to:
- Generate meshes for a coupled multiphysics FENIX model.
- Couple OpenMC [!cite](openmc) and MOOSE [!cite](giudicelli2024moose) using Cardinal for fixed source Monte Carlo calculations.
- Use Cardinal [!cite](novak2022_cardinal) to tally values of interest such as tritium production and heating which would be used in MOOSE to solve for the temperature distribution

We describe this workflow for a generic CAD model which is extremely simplified for the purpose of presenting the modeling workflow. This model was also used in [!cite](Eltawila2024PBNC) to perform mesh refinement studies to investigate how the different model meshes refinement affect multiphysics results. The meshed geometry was prepared using direct accelerated geometry Monte Carlo (DAGMC) for particle transport, and a volumetric mesh was also prepared to be used in MOOSE’s finite element solver and to tally OpenMC results for heat source distribution and tritium production. Cardinal was used to run OpenMC Monte Carlo particle transport within MOOSE framework. The data transfer system transferred heat source and temperature distribution between OpenMC and MOOSE as shown in [transfers], with coupling between neutron transport and heat conduction achieved via Picard iteration.

!media figures/cad_workflow_transfers.png 
  id=transfers
  caption=OpenMC and MOOSE Coupling
  style=width:80%;display:block;margin-left:auto;margin-right:auto;

## Generating the meshes

The CAD model was first developed in FUSION360 and was imported into Cubit to assign blocks, materials, and side sets and generate the mesh ([volumetric_mesh]). A corresponding DAGMC surface mesh ([dagmc]) was exported directly from the meshed geometry in Cubit (by loading the volumetric meshed geometry in Cubit and exporting a DAGMC surface mesh).

In this example, `tmesh_1.e` ([volumetric_mesh]) is the finite element mesh used in MOOSE on which the heat conduction physics is solved. `tmesh_1.h5m` ([dagmc]) is the DAGMC surface mesh used for particle transport in OpenMC (which bounds the surfaces between different materials). Cardinal also allows for mesh tallying for tallying OpenMC results directly on the mesh overlayed on the OpenMC geometry which `tmesh_1.e` ([volumetric_mesh]) could be used for as well as an unstructured volume mesh. This could be used by changing the tally type and adding a mesh template (`tally_type = mesh`, `mesh_template = tmesh_1.e`) in Cardinal input under Problem.

&nbsp;

!row! style=display:inline-flex;
!col! small=12 medium=4 large=3

!media figures/cad_workflow_mesh_1.png 
  id=volumetric_mesh
  caption=Model volumetric mesh
  style=width:130%;display:block;margin-left:auto;margin-right:auto;

!col-end!

!col! small=12 medium=4 large=3

!media figures/cad_workflow_d1.png 
  id=dagmc
  caption=Model DAGMC mesh
  style=width:130%;display:block;margin-left:auto;margin-right:auto;

!col-end!
!row-end!

## OpenMC

The OpenMC input files is as follows:

!listing /test/tests/examples/cad_workflow/model.py language=python

## Cardinal

The Cardinal input files is shown below. The ([MoabSkinner](https://cardinal.cels.anl.gov/source/userobjects/MoabSkinner.html)) was chosen in this model to update the DAGMC geometry from the MOOSE mesh directly ensuring meshes matching which is furhter discussed in[!cite](Eltawila2024PBNC) as well as the use of cell tallies.

!listing /test/tests/examples/cad_workflow/openmc.i

## FENIX (MOOSE) Heat transfer

The FENIX input files is as follows:

!listing /test/tests/examples/cad_workflow/solid.i

## Execution

To generate OpenMC xml files, run:

```
python model.py
```

Then to run the coupled calculation:

```
mpiexec -np 2 fenix-opt -i solid.i --n-threads=2
```

This will run both MOOSE and OpenMC (with Cardinal) with 2 MPI processes and 2 OpenMP threads per rank. To run the simulation faster, you can increase the parallel processes/threads, or simply decrease the number of particles used in OpenMC. When the simulation has completed, you will have created a number of different output files:

- `solid_out.e`, an Exodus output with the solid mesh and solution from the MOOSE/FENIX input file.
- `solid_out_openmc0.e`, an Exodus output with the OpenMC solution and the data that was ultimately transferred in/out of OpenMC.

## Results
  
&nbsp;

The results of this simulation are shown in [temps], [h3production], and [results]. More information - including a mesh sensitivity study and results from a finer mesh - is available in [!cite](Eltawila2024PBNC).

&nbsp;

!row! style=display:inline-flex;
!col! small=12 medium=4 large=3

!media figures/cad_workflow_Temps.png 
  id=temps
  caption=Temperature distribution result from [!cite](Eltawila2024PBNC)
  style=width:130%;display:block;margin-left:auto;margin-right:auto;

!col-end!

!col! small=12 medium=4 large=3

!media figures/cad_workflow_tritium_production.png 
  id=h3production
  caption=Tritium production rate density result from [!cite](Eltawila2024PBNC)
  style=width:130%;display:block;margin-left:auto;margin-right:auto;

!col-end!
!row-end!

&nbsp;

!table id=results caption=Results summary
| Parameter (Units) | Value |
| :- | :- |
| Armor Max. Temp. (K)         | 1062.4                   |
| First Wall Max. Temp. (K)    | 1057.6                   |
| Breeder Max. Temp. (K)       | 987.4                    |
| Heat Source (W)              | $2.44 \times 10^5^ \pm 3 \times 10^3^$   |
| Tritium Production (atoms/s) | $4.70 \times 10^13^ \pm 8 \times 10^11^$ |

&nbsp;