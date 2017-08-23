#include "Registrator.h"

reg::Registrator::Registrator(Fixed::self_type* fixed,
                              Moving::self_type* moving) {
  Initialize(fixed, moving);
  Execute(fixed, moving);
}

void reg::Registrator::Initialize(Fixed::self_type* fixed,
                                  Moving::self_type* moving) {
  Fixed::Set(fixed);
  Interpolator::Allocate();
  Method::Allocate();
  Metric::Allocate();
  Moving::Set(moving);
  Optimizer::Allocate();
  Resampler::Allocate();
  Transform::Allocate();
  TransformInitializer::Allocate();
}

void reg::Registrator::Execute(Fixed::self_type* fixed,
                               Moving::self_type* moving) {
  Metric::Get()->SetNumberOfHistogramBins(50);

  TransformInitializer::Get()->SetFixedImage(Fixed::Get());
  TransformInitializer::Get()->SetMovingImage(Moving::Get());
  TransformInitializer::Get()->SetTransform(Transform::Get());
  TransformInitializer::Get()->MomentsOn();
  TransformInitializer::Get()->InitializeTransform();

  Optimizer::Get()->SetLearningRate(1);
  Optimizer::Get()->SetNumberOfIterations(1000);
  Optimizer::Get()->SetMinimumConvergenceValue(1e-6);
  Optimizer::Get()->SetConvergenceWindowSize(10);

  Method::Get()->SetMetric(Metric::Get());
  Method::Get()->SetMetricSamplingStrategy(
      Method::self_type::MetricSamplingStrategyType::RANDOM);
  Method::Get()->SetMetricSamplingPercentage(.01);
  Method::Get()->SetOptimizer(Optimizer::Get());

  Method::Get()->SetShrinkFactorsPerLevel([]() {
    auto _ = itk::Array<int>(3);
    _[0] = 4;
    _[1] = 2;
    _[2] = 1;
    return _;
  }());
  Method::Get()->SetSmoothingSigmasPerLevel([]() {
    auto _ = itk::Array<int>(3);
    _[0] = 2;
    _[1] = 1;
    _[2] = 0;
    return _;
  }());
  Method::Get()->SetSmoothingSigmasAreSpecifiedInPhysicalUnits(true);
  Method::Get()->SetInitialTransform(Transform::Get());

  Method::Get()->SetFixedImage(Fixed::Get());
  Method::Get()->SetMovingImage(Moving::Get());
  Method::Get()->Update();

  Transform::Allocate();
  Transform::Get()->SetFixedParameters(
      Method::Get()->GetOutput()->Get()->GetFixedParameters());
  Transform::Get()->SetParameters(
      Method::Get()->GetOutput()->Get()->GetParameters());

  Resampler::Get()->SetInput(Moving::Get());
  Resampler::Get()->SetInterpolator(Interpolator::Get());
  Resampler::Get()->SetOutputSpacing(Fixed::Get()->GetSpacing());
  Resampler::Get()->SetOutputOrigin(Fixed::Get()->GetOrigin());
  Resampler::Get()->SetOutputDirection(Fixed::Get()->GetDirection());
  Resampler::Get()->SetSize(Fixed::Get()->GetLargestPossibleRegion().GetSize());
  Resampler::Get()->SetTransform(Transform::Get());

  Combined::Set(Resampler::Get()->GetOutput());
}