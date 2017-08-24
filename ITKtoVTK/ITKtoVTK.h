#pragma once

#include "Wrapper.h"
#include <itkImage.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkImageData.h>

namespace reg {
/// \struct ITKtoVTK
/// \brief converts itk::Image<double, 3> to vtk::ImageData
/// \warning requires itk to be built with ITKVTKglue
struct ITKtoVTK : Wrapper<itk::Image<double, 3>>,
                  Wrapper<vtkImageData>,
                  Wrapper<itk::ImageToVTKImageFilter<itk::Image<double, 3>>> {
  ITKtoVTK() = default;
  ITKtoVTK(itk::Image<double, 3> *);
  void Execute(itk::Image<double, 3> *);
  void Initialize(itk::Image<double, 3> *);
};
}
