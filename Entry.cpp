#include "ITKtoVTK.h"
#include "Reader.h"
#include "Registrator.h"
#include "Visualizer.h"
#include "Writer.h"

int main() {
  auto fixed = std::make_unique<reg::Reader>("C:/training_001_ct.mhd");
  auto moving = std::make_unique<reg::Reader>("C:/training_001_mr_T1.mhd");
  auto registrator = std::make_unique<reg::Registrator>(
      fixed->reg::Reader::Wrapper<itk::Image<double, 3>>::Get(),
      moving->reg::Reader::Wrapper<itk::Image<double, 3>>::Get());
  auto visualizer = std::make_unique<reg::Visualizer>(
      reg::ITKtoVTK(registrator->Combined::Get()).reg::ITKtoVTK::Wrapper<vtkImageData>::Get());
  auto writer = std::make_unique<reg::Writer>(
      "C:/Users/Adam Yu/Desktop/OUT.mhd", registrator->Combined::Get());
}
