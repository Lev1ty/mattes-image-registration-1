#pragma once

#include "Observer.h"
#include "Wrapper.h"
#include <itkCenteredTransformInitializer.h>
#include <itkGradientDescentOptimizerv4.h>
#include <itkImage.h>
#include <itkImageRegistrationMethodv4.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkMattesMutualInformationImageToImageMetricv4.h>
#include <itkResampleImageFilter.h>
#include <itkVersorRigid3DTransform.h>

namespace reg {
using PIXEL = double;
constexpr auto DIMENSIONS = 3u;

struct Combined : Wrapper<itk::Image<PIXEL, DIMENSIONS>> {};

struct Fixed : Wrapper<itk::Image<PIXEL, DIMENSIONS>> {};

struct Moving : Wrapper<itk::Image<PIXEL, DIMENSIONS>> {};

struct Transform
    : Wrapper<itk::VersorRigid3DTransform<PIXEL>> ///< \warning require
/// manipulation when changing
/// DIMENSIONS
{};

struct Interpolator
    : Wrapper<itk::LinearInterpolateImageFunction<Moving::self_type>> {};

struct Method
    : Wrapper<itk::ImageRegistrationMethodv4<
          Fixed::self_type, Moving::self_type, Transform::self_type>> {};

struct Metric : Wrapper<itk::MattesMutualInformationImageToImageMetricv4<
                    Fixed::self_type, Moving::self_type>> {};

struct Optimizer : Wrapper<itk::GradientDescentOptimizerv4Template<PIXEL>> {};

struct Resampler
    : Wrapper<itk::ResampleImageFilter<Moving::self_type,
                                       Fixed::self_type>> ///< \note optional
                                                          /// template
/// parameters not
/// explicitly
/// declared
{};

struct TransformInitializer
    : Wrapper<itk::CenteredTransformInitializer<
          Transform::self_type, Fixed::self_type, Moving::self_type>> {};

struct Monitor : Wrapper<CommandIterationUpdate> {};

struct Registrator : Combined,
                     Fixed,
                     Interpolator,
                     Method,
                     Metric,
                     Moving,
                     Optimizer,
                     Resampler,
                     Transform,
                     TransformInitializer,
                     Monitor {
  Registrator() = default;
  Registrator(Fixed::self_type *, Moving::self_type *);
  void Initialize(Fixed::self_type *, Moving::self_type *);
  void Execute(Fixed::self_type *, Moving::self_type *);
};
}
