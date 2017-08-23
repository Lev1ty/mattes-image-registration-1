#pragma once

#include <itkImage.h>
#include <itkImageFileReader.h>
#include "Wrapper.h"

namespace reg {
struct Reader : Wrapper<itk::Image<double, 3>>,
                Wrapper<itk::ImageFileReader<itk::Image<double, 3>>>,
                Wrapper<std::string> {
  Reader() = default;
  Reader(std::string &&);
  void Execute(std::string &&);
  void Initialize(std::string &&);
};
}