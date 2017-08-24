#pragma once

#include "Wrapper.h"
#include <itkImage.h>
#include <itkImageFileWriter.h>

namespace reg {
/// \struct Writer
/// \brief writes itk::Image<double, 3> to given filename
/// \note \var Wrapper<std::string> requires a fully qualified file path and extension
struct Writer : Wrapper<itk::Image<double, 3>>,
                Wrapper<itk::ImageFileWriter<itk::Image<double, 3>>>,
                Wrapper<std::string> {
  Writer() = default;
  Writer(std::string &&, itk::Image<double, 3> *);
  void Execute(std::string &&, itk::Image<double, 3> *);
  void Initialize(std::string &&, itk::Image<double, 3> *);
};
}
