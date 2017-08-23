#pragma once

#include <itkObject.h>
#include <itkSmartPointer.h>
#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <vector>
#include "CHECK.h"

namespace reg {
template <typename T, typename Container>
struct AbstractWrapper : CHECK<T>,
                         std::true_type  ///< check if Wrapper inherits any
                                         /// specialized WrapperBase structs
{
  using self_type = T;

 protected:
  AbstractWrapper() { std::cout << typeid(this).name() << std::endl; }

  virtual T *Get() const noexcept = 0;
  virtual void Set(T *) noexcept = 0;

  virtual void Allocate() noexcept = 0;

  Container container_;
};

template <typename, typename>
struct WrapperBase : std::false_type  ///< check if Wrapper inherits any
                                      /// specialized WrapperBase structs
{};

template <typename T>
struct WrapperBase<T, std::unique_ptr<T>>
    : AbstractWrapper<T, std::unique_ptr<T>> {
  T *Get() const noexcept override;
  void Set(T *) noexcept override;

 protected:
  void Allocate() noexcept override;
};

template <typename T>
struct WrapperBase<T, itk::SmartPointer<T>>
    : AbstractWrapper<T, itk::SmartPointer<T>> {
  T *Get() const noexcept override;
  void Set(T *) noexcept override;

 protected:
  void Allocate() noexcept override;
};

template <typename T>
struct WrapperBase<T, vtkSmartPointer<T>>
    : AbstractWrapper<T, vtkSmartPointer<T>> {
  T *Get() const noexcept override;
  void Set(T *) noexcept override;

 protected:
  void Allocate() noexcept override;
};

template <typename>
struct is_stl : std::false_type {};

template <typename T, typename Alloc>
struct is_stl<std::vector<T, Alloc>> : std::true_type {};

template <>
struct is_stl<std::string> : std::true_type {};

template <typename T>
struct Wrapper
    : std::conditional<is_stl<T>::value, WrapperBase<T, std::unique_ptr<T>>,
                       typename std::conditional<
                           std::is_base_of<itk::Object, T>::value,
                           WrapperBase<T, itk::SmartPointer<T>>,
                           typename std::conditional<
                               std::is_base_of<vtkObject, T>::value,
                               WrapperBase<T, vtkSmartPointer<T>>,
                               WrapperBase<void, void>>::type>::type>::type {};

template <typename T>
inline T *WrapperBase<T, std::unique_ptr<T>>::Get() const noexcept {
  CHECK<T>::NULLPTR_ERROR(
      AbstractWrapper<T, std::unique_ptr<T>>::container_.get(),
      "stl type container_ is nullptr");
  return AbstractWrapper<T, std::unique_ptr<T>>::container_.get();
}

template <typename T>
inline T *WrapperBase<T, itk::SmartPointer<T>>::Get() const noexcept {
  CHECK<T>::NULLPTR_ERROR(
      AbstractWrapper<T, itk::SmartPointer<T>>::container_.GetPointer(),
      "itk type container_ is nullptr");
  return AbstractWrapper<T, itk::SmartPointer<T>>::container_.GetPointer();
}

template <typename T>
inline T *WrapperBase<T, vtkSmartPointer<T>>::Get() const noexcept {
  CHECK<T>::NULLPTR_ERROR(
      AbstractWrapper<T, vtkSmartPointer<T>>::container_.GetPointer(),
      "vtk type container_ is nullptr");
  return AbstractWrapper<T, vtkSmartPointer<T>>::container_.GetPointer();
}

template <typename T>
inline void WrapperBase<T, std::unique_ptr<T>>::Set(T *m) noexcept {
  CHECK<T>::NULLPTR_NOTE(
      AbstractWrapper<T, std::unique_ptr<T>>::container_.get(),
      "stl type container_ before set operation is nullptr");
  AbstractWrapper<T, std::unique_ptr<T>>::container_.reset(m);
  CHECK<T>::NULLPTR_WARNING(
      AbstractWrapper<T, std::unique_ptr<T>>::container_.get(),
      "stl type container_ set to nullptr");
}

template <typename T>
inline void WrapperBase<T, itk::SmartPointer<T>>::Set(T *m) noexcept {
  CHECK<T>::NULLPTR_NOTE(
      AbstractWrapper<T, itk::SmartPointer<T>>::container_.GetPointer(),
      "itk type container_ before set operation is nullptr");
  AbstractWrapper<T, itk::SmartPointer<T>>::container_ = m;
  CHECK<T>::NULLPTR_WARNING(
      AbstractWrapper<T, itk::SmartPointer<T>>::container_.GetPointer(),
      "itk type container_ set to nullptr");
}

template <typename T>
inline void WrapperBase<T, vtkSmartPointer<T>>::Set(T *m) noexcept {
  CHECK<T>::NULLPTR_NOTE(
      AbstractWrapper<T, vtkSmartPointer<T>>::container_.GetPointer(),
      "vtk type container_ before set operation is nullptr");
  AbstractWrapper<T, vtkSmartPointer<T>>::container_ = m;
  CHECK<T>::NULLPTR_WARNING(
      AbstractWrapper<T, vtkSmartPointer<T>>::container_.GetPointer(),
      "vtk type container_ set to nullptr");
}

template <typename T>
inline void WrapperBase<T, std::unique_ptr<T>>::Allocate() noexcept {
  Set(new T);
}

template <typename T>
inline void WrapperBase<T, itk::SmartPointer<T>>::Allocate() noexcept {
  Set(T::New());
}

template <typename T>
inline void WrapperBase<T, vtkSmartPointer<T>>::Allocate() noexcept {
  Set(T::New());
}
}
