#pragma once

#include "CHECK.h"
#include <itkObject.h>
#include <itkSmartPointer.h>
#include <memory>
#include <type_traits>
#include <typeinfo>
#include <vector>
#include <vtkObject.h>
#include <vtkSmartPointer.h>

namespace reg {
/// \struct AbstractWrapper
/// \brief abstract class for all Wrapper classes
/// \details encapsulates data
/// \param T contained type
/// \param Container container type
/// \note RTTI
template <typename T, typename Container>
struct AbstractWrapper : CHECK<T>,
                         std::true_type ///< check if Wrapper inherits any
                                        /// specialized WrapperBase structs
{
  using self_type = T; ///< for introspection

protected:
  AbstractWrapper() { std::cout << typeid(this).name() << std::endl; } ///< RTTI

  virtual T *Get() const noexcept = 0;
  virtual void Set(T *) noexcept = 0;

  virtual void Allocate() noexcept = 0;

  Container container_;
};

/// \struct WrapperBase
/// \brief general template of WrapperBase
/// \note not specialized
/// \warning not meant to be selected as Base
template <typename, typename>
struct WrapperBase : std::false_type ///< check if Wrapper inherits any
                                     /// specialized WrapperBase structs
{};

/// \struct WrapperBase
/// \brief specialized template of WrapperBase
/// \details specialized for types in STL
/// \param T contained type
template <typename T>
struct WrapperBase<T, std::unique_ptr<T>>
    : AbstractWrapper<T, std::unique_ptr<T>> {
  T *Get() const noexcept override;
  void Set(T *) noexcept override;

protected:
  void Allocate() noexcept override;
};

/// \struct WrapperBase
/// \brief specialized template of WrapperBase
/// \details specialized for types in itk
/// \param T contained type
template <typename T>
struct WrapperBase<T, itk::SmartPointer<T>>
    : AbstractWrapper<T, itk::SmartPointer<T>> {
  T *Get() const noexcept override;
  void Set(T *) noexcept override;

protected:
  void Allocate() noexcept override;
};

/// \struct WrapperBase
/// \brief specialized template of WrapperBase
/// \details specialized for types in vtk
/// \param T contained type
template <typename T>
struct WrapperBase<T, vtkSmartPointer<T>>
    : AbstractWrapper<T, vtkSmartPointer<T>> {
  T *Get() const noexcept override;
  void Set(T *) noexcept override;

protected:
  void Allocate() noexcept override;
};

/// \struct is_stl
/// \brief general template of is_stl
/// \details check if type is STL
template <typename> struct is_stl : std::false_type {};

/// \struct is_stl
/// \brief specialized template of is_stl
/// \details detects any containers based on std::vector
/// \param T contained type
/// \param Alloc STL specific allocator
template <typename T, typename Alloc>
struct is_stl<std::vector<T, Alloc>> : std::true_type {};

/// \struct is_stl
/// \brief specialized template of is_stl
/// \details detects std::string
template <> struct is_stl<std::string> : std::true_type {};

/// \struct Wrapper
/// \brief interface Wrapper
/// \details chooses which WrapperBase to inherit depending on \p T
/// \warning not optimized for extensibility
/// \note possible future implementation using SFINAE and void_t
/// \param T contained type
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
