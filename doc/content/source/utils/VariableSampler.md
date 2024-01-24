# VariableSampler

The `VariableSampler` is a utility that enables users to sample the value of a MooseVariable at an abitrary point in space

The sampler is designed to be used with rays. The point parameter exists within the provided element parameter. After instantiation, use `VariableSampler::sampleVariable()` to sample the value of a variable at a point in space.
