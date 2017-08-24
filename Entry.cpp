#include "ITKtoVTK.h"
#include "Reader.h"
#include "Registrator.h"
#include "Visualizer.h"
#include "Writer.h"

/// \brief entry point for program
int main() {
  auto fixed = std::make_unique<reg::Reader>("/media/adam/OS/CT.mhd");
  auto moving = std::make_unique<reg::Reader>("/media/adam/OS/MR.mhd");
  auto registrator = std::make_unique<reg::Registrator>(
      fixed->reg::Reader::Wrapper<itk::Image<double, 3>>::Get(),
      moving->reg::Reader::Wrapper<itk::Image<double, 3>>::Get());
  auto visualizer = std::make_unique<reg::Visualizer>(
      reg::ITKtoVTK(registrator->Combined::Get())
          .reg::ITKtoVTK::Wrapper<vtkImageData>::Get());
  auto writer = std::make_unique<reg::Writer>(
      "/media/adam/OS/Users/Adam Yu/Desktop/OUT.mhd",
      registrator->Combined::Get());
}
