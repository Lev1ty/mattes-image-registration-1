#pragma once

#include "ITKtoVTK.h"
#include <limits>
#include <vtkImageShiftScale.h>
#include <vtkImageThreshold.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>

namespace reg {
/// \struct CompositeOpacity
/// \brief alias for Wrapper<vtkPiecewiseFunction> for disambiguation
struct CompositeOpacity : Wrapper<vtkPiecewiseFunction> {};

/// \struct Visualizer
/// \brief visualize vtkImageData in 3D space
struct Visualizer : Wrapper<vtkImageData>,
                    Wrapper<vtkRenderer>,
                    Wrapper<vtkRenderWindow>,
                    Wrapper<vtkRenderWindowInteractor>,
                    Wrapper<vtkSmartVolumeMapper>, ///< maps vtkImageData to
                                                   /// visualization framework
                    Wrapper<vtkVolume>,
                    Wrapper<vtkVolumeProperty>,
                    CompositeOpacity,
                    Wrapper<vtkImageShiftScale>, ///< adjusts brightness
                    Wrapper<vtkImageThreshold> {
  Visualizer() = default;
  Visualizer(vtkImageData *);
  void Execute(vtkImageData *);
  void Initialize(vtkImageData *);
};
}
