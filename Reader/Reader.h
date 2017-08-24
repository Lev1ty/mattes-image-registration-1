#pragma once

#include "Wrapper.h"
#include <itkImage.h>
#include <itkImageFileReader.h>

namespace reg {
/// \struct Reader
/// \brief read an image into itk::Image<double, 3>
/// \note \var Wrapper<std::string> requires a fully qualified file path and extension
struct Reader : Wrapper<itk::Image<double, 3>>,
                Wrapper<itk::ImageFileReader<itk::Image<double, 3>>>,
                Wrapper<std::string> {
  Reader() = default;
  Reader(std::string &&);
  void Execute(std::string &&);
  void Initialize(std::string &&);
};
}
