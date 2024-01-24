#pragma once

#include "VariableSampler.h"
#include "TestPICStudyUserDefinedRays.h"

class TestVariableSampler : public TestPICStudyUserDefinedRays
{
public:
  TestVariableSampler(const InputParameters & parameters);

  static InputParameters validParams();

protected:
  virtual void postExecuteStudy() override;

private:
  /// The utility used to sample a moose variable as a ray is traveling
  const FENIX::VariableSampler _sampler;
  /// The ray data index where the value of sampled field will be stored
  const RayDataIndex _field_idx;
};
