# CAD-based Geometry Workflow for Multiphysics Fusion Problems Using OpenMC and MOOSE

This demonstration describes a workflow for modeling fusion problems in OpenMC and MOOSE using a computer aided design (CAD)-based geometry workflow.
It is based on the work published in [!cite](Eltawila2024PBNC). 

In this example, you'll learn how to:

- Couple OpenMC [!cite](openmc) and MOOSE [!cite](giudicelli2024moose) using Cardinal for fixed source Monte Carlo calculations.
- Use Cardinal [!cite](novak2022_cardinal) to tally values of interest such as tritium production and heating which would be used in MOOSE to solve for the temperature distribution

!media figures/transfers.png
  id=transfers
  caption=OpenMC and MOOSE Coupling
  style=width:60%;margin-left:auto;margin-right:auto

An extremely simplified tokamak was modeled in CAD and was considered for this example. The meshed geometry was prepared using direct accelerated geometry Monte Carlo (DAGMC) for particle transport, and a volumetric mesh was also prepared to be used in MOOSE’s finite element solver and to tally OpenMC results for heat source distribution and tritium production. Cardinal was used to run OpenMC Monte Carlo particle transport within MOOSE framework. The data transfer system transfered heat source and temperature distribution between OpenMC and MOOSE as shown in Figure 1, with coupling between neutron transport and heat conduction achieved via Picard iteration.

## Generating the meshes

The CAD model was first developed in FUSION360 and was imported into Cubit to assign blocks, materials, and side sets and generate the mesh (Figure 2). A corresponding DAGMC surface mesh (Figure 3) was exported directly from the meshed geometry in Cubit (by loading the volumetric meshed geometry in Cubit and exporting a DAGMC surface mesh).

In this example, `tmesh_1.e` (Figure 2) is the finite element mesh used in MOOSE on which the heat conduction physics is solved. `tmesh_1.h5m` (Figure 3) is the DAGMC surface mesh used for particle transport in OpenMC (which bounds the surfaces between different materials). Cardinal also allows for mesh tallying for tallying OpenMC results directly on the mesh overlayed on the OpenMC geometry  which `tmesh_1.e` (Figure 2) could be used for as well as an unstructered volume mesh. This could be used by changing the tally type and adding a mesh template (`tally_type = mesh`, `mesh_template = tmesh_1.e`) in Cardinal input under Problem.

&nbsp;

!row! style=display:inline-flex;
!col! small=12 medium=4 large=3

!media figures/mesh_1.png style=width:130%;display:block;
  id=volumetric_mesh caption=Volumetric mesh

!col-end!

!col! small=12 medium=4 large=3

!media figures/d1.png style=width:130%;display:block;
  id=dagmc caption=DAGMC mesh

!col-end!
!row-end!

## OpenMC

!listing /input_files/model.py language=python

## Cardinal

!listing /input_files/openmc.i

## MOOSE Heat transfer

!listing /input_files/solid.i

## Execution

To run the coupled calculation:

```
mpiexec -np 2 cardinal-opt -i solid.i --n-threads=2
```
This will run both MOOSE and OpenMC with 2 MPI processes and 2 OpenMP threads per rank. To run the simulation faster, you can increase the parallel processes/threads, or simply decrease the number of particles used in OpenMC. When the simulation has completed, you will have created a number of different output files:
- `solid_out.e`, an Exodus output with the solid mesh and solution 
- `solid_out_openmc0.e`, an Exodus output with the OpenMC solution and the data that was ultimately transferred in/out of OpenMC


## Results
  
&nbsp;

!row! style=display:inline-flex;
!col! small=12 medium=4 large=3

!media figures/Temps.png style=width:130%;display:block;
  id=temps caption=Temperature distribution

!col-end!

!col! small=12 medium=4 large=3

!media figures/tritium_production.png style=width:130%;display:block;
  id=h3production caption=Tritium production rate density

!col-end!
!row-end!

&nbsp;

!table id=results caption=Results summary
| Parameter | Value |
| :- | :- |
| Armor Max. Temp. (K)         | 1062.4                   |
| First Wall Max. Temp. (K)    | 1057.6                   |
| Breeder Max. Temp. (K)       | 987.4                    |
| Heat Source (W)              | 2.44 × 10^5^ ± 3 × 10^3^   |
| Tritium Production (atoms/s) | 4.70 × 10^13^ ± 8 × 10^11^ |

&nbsp;