#include "ITKtoVTK.h"

reg::ITKtoVTK::ITKtoVTK(itk::Image<double, 3>* itk_image) {
  Initialize(itk_image);
  Execute(itk_image);
}

void reg::ITKtoVTK::Execute(itk::Image<double, 3>* itk_image) {
  Wrapper<itk::ImageToVTKImageFilter<itk::Image<double, 3>>>::Get()->SetInput(
      Wrapper<itk::Image<double, 3>>::Get());
  Wrapper<itk::ImageToVTKImageFilter<itk::Image<double, 3>>>::Get()->Update();
  Wrapper<vtkImageData>::Set(
      Wrapper<itk::ImageToVTKImageFilter<itk::Image<double, 3>>>::Get()
          ->GetOutput());
}

void reg::ITKtoVTK::Initialize(itk::Image<double, 3>* itk_image) {
  Wrapper<itk::Image<double, 3>>::Set(itk_image);
  Wrapper<itk::ImageToVTKImageFilter<itk::Image<double, 3>>>::Allocate();
}