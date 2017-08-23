#pragma once

#include <itkImage.h>
#include <itkImageFileWriter.h>
#include "Wrapper.h"

namespace reg {
struct Writer : Wrapper<itk::Image<double, 3>>,
                Wrapper<itk::ImageFileWriter<itk::Image<double, 3>>>,
                Wrapper<std::string> {
  Writer() = default;
  Writer(std::string &&, itk::Image<double, 3> *);
  void Execute(std::string &&, itk::Image<double, 3> *);
  void Initialize(std::string &&, itk::Image<double, 3> *);
};
}