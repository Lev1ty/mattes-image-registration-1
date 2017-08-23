#pragma once

#include <vtkImageShiftScale.h>
#include <vtkImageThreshold.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <limits>
#include "ITKtoVTK.h"

namespace reg {
struct CompositeOpacity : Wrapper<vtkPiecewiseFunction> {};

struct Visualizer : Wrapper<vtkImageData>,
                    Wrapper<vtkRenderer>,
                    Wrapper<vtkRenderWindow>,
                    Wrapper<vtkRenderWindowInteractor>,
                    Wrapper<vtkSmartVolumeMapper>,
                    Wrapper<vtkVolume>,
                    Wrapper<vtkVolumeProperty>,
                    CompositeOpacity,
                    Wrapper<vtkImageShiftScale>,
                    Wrapper<vtkImageThreshold> {
  Visualizer() = default;
  Visualizer(vtkImageData *);
  void Execute(vtkImageData *);
  void Initialize(vtkImageData *);
};
}