# PICStudyBase

A base class for the PIC simulations utilizing the RayTracing module. The base study only registers basic ray data needed for PIC and provides a basic implementation of `postExecuteStudy()` which moves `bankedRay()` to a public member variable `_banked_rays`. `RayDataIndex`'s are registered for each velocity component. Each is named registered with the name `"v_n"` and the index has the corrisponding member variable `_v_n_index` where  `n` is one of the following x, y or z. An additional piece of data that is registered which is the particle weight. This is registered with the name `"weight"` and the index has the corrisponding member variable `_weight_index`.



!table caption=Registered RayData
| Description | Registered Name | Index Member Variable |
| - | - | - |
| X Velocity Component | `"v_x"` | `_v_x_index` |
| Y Velocity Component | `"v_y"` | `_v_y_index` |
| Z Velocity Component | `"v_z"` | `_v_z_index` |
| Particle Weight | `"weight"` | `_weight_index`
