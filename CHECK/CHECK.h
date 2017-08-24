#pragma once

#include <exception>
#include <iostream>
#include <string>

namespace reg {
/// \struct CHECK
/// \brief error checking class
/// \details avoid exceptions because they cause undefined behavior; rather, use
/// detailed reporting
/// \note every class should directly or indirectly be derived from this class
/// \param T the base type to monitor
template <typename T>
struct CHECK {
protected:
  void NULLPTR_ERROR(T *t, std::string &&msg = "NULLPTR_ERROR") const noexcept {
    if (t == nullptr)
      std::cerr << "Error: " << std::forward<std::string>(msg) << std::endl;
  }
  void NULLPTR_NOTE(T *t, std::string &&msg = "NULLPTR_NOTE") const noexcept {
    if (t == nullptr)
      std::cout << "Note: " << std::forward<std::string>(msg) << std::endl;
  }
  void NULLPTR_WARNING(T *t, std::string &&msg = "NULLPTR_WARNING") const
      noexcept {
    if (t == nullptr)
      std::cerr << "Warning: " << std::forward<std::string>(msg) << std::endl;
  }
};
} // namespace reg
