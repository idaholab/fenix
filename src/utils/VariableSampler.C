#include "VariableSampler.h"

namespace FENIX
{
VariableSampler::VariableSampler(FEProblemBase & problem,
                                 const VariableName & name,
                                 const THREAD_ID tid)
  : _system(problem.getSystem(name)),
    _var_num(
        problem
            .getVariable(
                tid, name, Moose::VarKindType::VAR_ANY, Moose::VarFieldType::VAR_FIELD_STANDARD)
            .number())
{
}

Real
VariableSampler::sampleVariable(const Point & point, const Elem * element) const
{
  return _system.point_value(_var_num, point, element);
}
}
