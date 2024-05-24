#include "ChargeDensityAccumulator.h"
#include "ResidualAccumulator.h"
#include "PICStudyBase.h"
#include "MooseMesh.h"

registerMooseObject("FenixApp", ChargeDensityAccumulator);

InputParameters
ChargeDensityAccumulator::validParams()
{
  auto params = GeneralUserObject::validParams();
  params.addClassDescription(
      "Accumulator used to evaluate the inner product of the charge density and the test function "
      "required for solving electromagnetic equations");
  params.addRequiredParam<UserObjectName>("study", "The PICStudy that owns the charged particles");
  // These parameters are necessary when using ResidualAccumulator
  params += TaggingInterface::validParams();
  // This exec flag is necessary for the ChargeDensityAccumulator to contribute to residuals
  ExecFlagEnum & exec_enum = params.set<ExecFlagEnum>("execute_on", true);
  exec_enum.addAvailableFlags(EXEC_PRE_KERNELS);
  params.set<ExecFlagEnum>("execute_on") = EXEC_PRE_KERNELS;
  // making this input parameter private so the user cannot use the object incorrectly
  params.suppressParameter<ExecFlagEnum>("execute_on");
  params.addRequiredParam<NonlinearVariableName>("variable",
                                                 "The variable to contribute to the residual of");
  return params;
}

ChargeDensityAccumulator::ChargeDensityAccumulator(const InputParameters & params)
  : GeneralUserObject(params),
    _var_name(getParam<NonlinearVariableName>("variable")),
    _study(getUserObject<PICStudyBase>("study")),
    _charge_index(_study.getRayDataIndex("charge")),
    _weight_index(_study.getRayDataIndex("weight"))
{
}

void
ChargeDensityAccumulator::execute()
{
  if (_fe_problem.currentlyComputingResidual())
  {
    std::unique_ptr<FENIX::AccumulatorBase> accumulator =
        std::make_unique<FENIX::ResidualAccumulator>(_fe_problem, this, _var_name, 0);

    auto particles = _study.getBankedRays();

    for (auto & p : particles)
    {
      accumulator->add(
          *p->currentElem(), p->currentPoint(), p->data(_charge_index) * p->data(_weight_index));
    }

    accumulator->finalize();
  }
}
