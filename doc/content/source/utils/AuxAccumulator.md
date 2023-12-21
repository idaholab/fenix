# AuxAccumulator

The `AuxAccumulator` is a utility that enables the accumulation of pointwise data
into an auxiliary field. The accumulation is done similarly to how Dirac points
would contribute to a field.

It is designed to be constructed and destructed on use. That is - you should
instantiate an `AuxAccumulator` when you want to contribute to the field, and then
call `AuxAccumulator::finalize()` and destruct it once you are done contributing
to the field. The finalization will take care of properly closing the aux variable.

Upon instantiation, use `AuxAccumulator::add()` to accumulate values into the field.
