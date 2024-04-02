#include "ChargeAccumulator.h"

#include "MooseMesh.h"

registerMooseObject("FenixApp", ChargeAccumulator);

InputParameters
ChargeAccumulator::validParams()
{
  auto params = GeneralUserObject::validParams();
  params.addClassDescription(
      "Accumulator used to evaluate the inner product of the charge density and the test function "
      "required for solving electromagnetic equations");
  params.addRequiredParam<UserObjectName>("study", "The PICStudy that owns the charged particles");
  // Make sure that you add these params when using the ResidualAccumulator
  params += TaggingInterface::validParams();
  // You need to add this exec flag so it can contribute to residuals
  ExecFlagEnum & exec_enum = params.set<ExecFlagEnum>("execute_on", true);
  exec_enum.addAvailableFlags(EXEC_PRE_KERNELS);
  params.addRequiredParam<NonlinearVariableName>("variable",
                                                 "The variable to contribute to the residual of");
  return params;
}

ChargeAccumulator::ChargeAccumulator(const InputParameters & params)
  : GeneralUserObject(params),
    _var_name(getParam<NonlinearVariableName>("variable")),
    _study(getUserObject<PICStudyBase>("study")),
    _charge_index(_study.getRayDataIndex("charge"))
{
}

void
ChargeAccumulator::execute()
{
  if (_fe_problem.currentlyComputingResidual())
  {
    std::unique_ptr<FENIX::AccumulatorBase> accumulator =
        std::make_unique<FENIX::ResidualAccumulator>(_fe_problem, this, _var_name, 0);

    auto particles = _study.getBankedRays();

    for (auto & p : particles)
    {
      accumulator->add(*p->currentElem(), p->currentPoint(), p->data(_charge_index));
    }

    accumulator->finalize();
  }
}
