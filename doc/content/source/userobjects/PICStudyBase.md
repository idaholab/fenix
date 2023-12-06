# PICStudyBase

A base class for the PIC simulations utilizing the RayTracing module. The base study only registers basic ray data needed for PIC and provides a basic implementation of `postExecuteStudy()` which moves `bankedRay()` to a public member variable `_banked_rays`. `RayDataIndex`'s are registered for each velocity component. Each is named registered with the name `"v_n"` and the index has the corrisponding member variable `_v_n_index` where  `n` is one of the following x, y or z. An additional piece of data that is registered which is the particle weight. This is registered with the name `"weight"` and the index has the corrisponding member variable `_weight_index`.



!table caption=Registered RayData
| Description | Registered Name | Index Member Variable |
| - | - | - |
| X Velocity Component | `"v_x"` | `_v_x_index` |
| Y Velocity Component | `"v_y"` | `_v_y_index` |
| Z Velocity Component | `"v_z"` | `_v_z_index` |
| Particle Weight | `"weight"` | `_weight_index`

## Building From The Base

The `PICBaseStudy` provides the basic functionality for resetting rays but does not provide the implementation for generating them. When creating a new study which inherits from `PICStudyBase` your implmentation of the `generateRays()` method should looks something like the following

```c++
void
PICStudyBase::generateRays()
{
  if (!_has_generated)
  {
    // add your custom logic for initial generation here
  }

  PICStudyBase::generateRays();
}
```

The base implementation of `generateRays()` will do nothing but flip the `_has_generated` flag on the first time it is called and provides the logic needed for a basic resetting of rays. Rays will be reset with their velocity updated with the current velocity stored in `RayData` as the vector argument and _dt being passed as the time step into to `_velocity_updater.updateVelocity`.
