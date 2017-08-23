#include "Writer.h"

reg::Writer::Writer(std::string&& filename, itk::Image<double, 3>* image) {
  Initialize(std::forward<std::string>(filename), image);
  Execute(std::forward<std::string>(filename), image);
}

void reg::Writer::Execute(std::string&& filename,
                          itk::Image<double, 3>* image) {
  Wrapper<itk::ImageFileWriter<itk::Image<double, 3>>>::Get()->SetFileName(
      Wrapper<std::string>::Get()->c_str());
  Wrapper<itk::ImageFileWriter<itk::Image<double, 3>>>::Get()->SetInput(
      Wrapper<itk::Image<double, 3>>::Get());
  Wrapper<itk::ImageFileWriter<itk::Image<double, 3>>>::Get()->Update();
}

void reg::Writer::Initialize(std::string&& filename,
                             itk::Image<double, 3>* image) {
  Wrapper<std::string>::Set(
      new std::string{std::forward<std::string>(filename)});
  Wrapper<itk::Image<double, 3>>::Set(image);
  Wrapper<itk::ImageFileWriter<itk::Image<double, 3>>>::Allocate();
}