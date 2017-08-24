#pragma once

#include "ITKtoVTK.h"
#include "Visualizer.h"
#include "Wrapper.h"

#include <itkCenteredTransformInitializer.h>
#include <itkCommand.h>
#include <itkGradientDescentOptimizerv4.h>
#include <itkImage.h>
#include <itkImageRegistrationMethodv4.h>
#include <itkLinearInterpolateImageFunction.h>
#include <itkMattesMutualInformationImageToImageMetricv4.h>
#include <itkResampleImageFilter.h>
#include <itkVersorRigid3DTransform.h>

/// \see https://itk.org/Doxygen45/html/Registration_2ImageRegistration8_8cxx-example.html
/// \note uses itk::GradientDescentOptimizerv4Template<double> instead
/// \warning no considerations for extensibility
class CommandIterationUpdate : public itk::Command {
public:
  typedef CommandIterationUpdate Self;
  typedef itk::Command Superclass;
  typedef itk::SmartPointer<Self> Pointer;
  itkNewMacro(Self);

protected:
  CommandIterationUpdate(){};

public:
  typedef itk::GradientDescentOptimizerv4Template<double> OptimizerType;
  typedef const OptimizerType *OptimizerPointer;
  void Execute(itk::Object *caller,
               const itk::EventObject &event) ITK_OVERRIDE {
    Execute((const itk::Object *)caller, event);
  }
  void Execute(const itk::Object *object,
               const itk::EventObject &event) ITK_OVERRIDE {
    OptimizerPointer optimizer = static_cast<OptimizerPointer>(object);
    if (!itk::IterationEvent().CheckEvent(&event)) {
      return;
    }
    std::cout << optimizer->GetCurrentIteration() << "   ";
    std::cout << optimizer->GetValue() << "   ";
    std::cout << optimizer->GetCurrentPosition() << std::endl;
  }
};
