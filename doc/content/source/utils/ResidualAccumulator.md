# ResidualAccumulator

The `ResidualAccumulator` is a utility that enables the accumulation of pointwise data
into a residual as a point source. The accumulation is done similarly to how Dirac points
would contribute to a field.

It is designed to be constructed and destructed on use. That is - you should
instantiate an `ResidualAccumulator` when you want to contribute to the field, and then
call `ResidualAccumulator::finalize()` and destruct it once you are done contributing
to the field. The finalization will take care of properly closing the aux variable.

Upon instantiation, use `ResidualAccumulator::add()` to accumulate values into the field.

This object requires a `MooseObject` to be constructed, as it relies on the
`TaggingInterface`. Be sure to add the parameters from `TaggingInterface` to said
object.
