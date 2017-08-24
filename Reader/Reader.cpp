#include "Reader.h"

reg::Reader::Reader(std::string&& filename) {
  Initialize(std::forward<std::string>(filename));
  Execute(std::forward<std::string>(filename));
}

void reg::Reader::Execute(std::string&& filename) {
  Wrapper<itk::ImageFileReader<itk::Image<double, 3>>>::Get()->SetFileName(
      Wrapper<std::string>::Get()->c_str());
  Wrapper<itk::ImageFileReader<itk::Image<double, 3>>>::Get()->Update();
  Wrapper<itk::Image<double, 3>>::Set(
      Wrapper<itk::ImageFileReader<itk::Image<double, 3>>>::Get()->GetOutput());
}

void reg::Reader::Initialize(std::string&& filename) {
  Wrapper<std::string>::Set(
      new std::string{std::forward<std::string>(filename)});
  Wrapper<itk::Image<double, 3>>::Allocate();
  Wrapper<itk::ImageFileReader<itk::Image<double, 3>>>::Allocate();
}
