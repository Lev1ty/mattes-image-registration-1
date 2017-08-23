#pragma once

#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkImageData.h>
#include "Wrapper.h"

namespace reg {
struct ITKtoVTK : Wrapper<itk::Image<double, 3>>,
                  Wrapper<vtkImageData>,
                  Wrapper<itk::ImageToVTKImageFilter<itk::Image<double, 3>>> {
  ITKtoVTK() = default;
  ITKtoVTK(itk::Image<double, 3> *);
  void Execute(itk::Image<double, 3> *);
  void Initialize(itk::Image<double, 3> *);
};
}