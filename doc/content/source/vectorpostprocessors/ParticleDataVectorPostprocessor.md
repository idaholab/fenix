# ParticleDataVectorPostprocessor

!syntax description /VectorPostprocessors/ParticleDataVectorPostprocessor

If no additional ray data is requested as output then the following data will be output

| Label | Description |
| - | - |
| `t_pos` | The time at which the particle position is calculated |
| `x` | The x component of the particle position |
| `y` | The x component of the particle position |
| `z` | The x component of the particle position |
| `t_vel` | The time at which the particle velocity is calculated |
| `v_x` | The x component of the velocity vector |
| `v_y` | The x component of the velocity vector |
| `v_z` | The x component of the velocity vector |

## Example Input Syntax

!listing test/tests/benchmarking/lieberman.i block=VectorPostprocessors/ray_data

!syntax parameters /VectorPostprocessors/ParticleDataVectorPostprocessor

!syntax inputs /VectorPostprocessors/ParticleDataVectorPostprocessor

!syntax children /VectorPostprocessors/ParticleDataVectorPostprocessor
