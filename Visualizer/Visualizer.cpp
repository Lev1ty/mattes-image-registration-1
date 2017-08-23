#include "Visualizer.h"

reg::Visualizer::Visualizer(vtkImageData* image) {
  Initialize(image);
  Execute(image);
}

void reg::Visualizer::Execute(vtkImageData* image) {
  Wrapper<vtkRenderWindow>::Get()->AddRenderer(Wrapper<vtkRenderer>::Get());
  Wrapper<vtkRenderWindowInteractor>::Get()->SetRenderWindow(
      Wrapper<vtkRenderWindow>::Get());
  Wrapper<vtkRenderWindow>::Get()->Render();
  Wrapper<vtkImageShiftScale>::Get()->SetInputData(
      Wrapper<vtkImageData>::Get());
  Wrapper<vtkImageShiftScale>::Get()->SetScale(5);
  Wrapper<vtkImageShiftScale>::Get()->Update();
  Wrapper<vtkImageThreshold>::Get()->SetInputData(
      Wrapper<vtkImageShiftScale>::Get()->GetOutput());
  Wrapper<vtkImageThreshold>::Get()->ThresholdBetween(
      110, std::numeric_limits<double>::max());
  Wrapper<vtkImageThreshold>::Get()->ReplaceOutOn();
  Wrapper<vtkImageThreshold>::Get()->SetOutValue(0);
  Wrapper<vtkImageThreshold>::Get()->Update();
  Wrapper<vtkSmartVolumeMapper>::Get()->SetBlendModeToComposite();
  // Wrapper<vtkSmartVolumeMapper>::Get()->SetInputData(Wrapper<vtkImageThreshold>::Get()->GetOutput());
  Wrapper<vtkSmartVolumeMapper>::Get()->SetInputData(
      Wrapper<vtkImageData>::Get());
  Wrapper<vtkVolumeProperty>::Get()->SetScalarOpacity(CompositeOpacity::Get());
  std::cout << *Wrapper<vtkVolumeProperty>::Get()->GetScalarOpacity()
            << std::endl;
  Wrapper<vtkVolumeProperty>::Get()->ShadeOff();
  Wrapper<vtkVolumeProperty>::Get()->SetInterpolationType(
      VTK_LINEAR_INTERPOLATION);
  Wrapper<vtkVolume>::Get()->SetMapper(Wrapper<vtkSmartVolumeMapper>::Get());
  Wrapper<vtkVolume>::Get()->SetProperty(Wrapper<vtkVolumeProperty>::Get());
  Wrapper<vtkRenderer>::Get()->AddViewProp(Wrapper<vtkVolume>::Get());
  Wrapper<vtkRenderer>::Get()->ResetCamera();
  Wrapper<vtkRenderWindow>::Get()->Render();
  Wrapper<vtkRenderWindowInteractor>::Get()->Start();
}

void reg::Visualizer::Initialize(vtkImageData* image) {
  Wrapper<vtkImageData>::Set(image);
  Wrapper<vtkRenderer>::Allocate();
  Wrapper<vtkRenderWindow>::Allocate();
  Wrapper<vtkRenderWindowInteractor>::Allocate();
  Wrapper<vtkSmartVolumeMapper>::Allocate();
  Wrapper<vtkVolume>::Allocate();
  Wrapper<vtkVolumeProperty>::Allocate();
  CompositeOpacity::Allocate();
  Wrapper<vtkImageShiftScale>::Allocate();
  Wrapper<vtkImageThreshold>::Allocate();
}