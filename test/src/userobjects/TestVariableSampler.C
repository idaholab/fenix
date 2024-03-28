#include "TestVariableSampler.h"

registerMooseObject("FenixTestApp", TestVariableSampler);

InputParameters
TestVariableSampler::validParams()
{
  InputParameters params = TestPICStudyUserDefinedRays::validParams();
  params.addRequiredParam<VariableName>("field", "The field that you want to sample");
  return params;
}

TestVariableSampler::TestVariableSampler(const InputParameters & params)
  : TestPICStudyUserDefinedRays(params),
    _sampler(FENIX::VariableSampler(_fe_problem, getParam<VariableName>("field"), _tid)),
    _field_idx(registerRayData("field_value"))
{
}

void
TestVariableSampler::postExecuteStudy()
{
  TestPICStudyUserDefinedRays::postExecuteStudy();
  for (auto & ray : _banked_rays)
  {
    ray->data()[_field_idx] = _sampler.sampleVariable(ray->currentPoint(), ray->currentElem());
  }
}
