#pragma once

#include <exception>
#include <iostream>
#include <string>

namespace reg {
template <typename T>
struct CHECK {
 protected:
  void NULLPTR_ERROR(T* t, std::string&& msg = "NULLPTR_ERROR") const {
    if (t == nullptr)
      std::cerr << "Error: " << std::forward<std::string>(msg) << std::endl;
  }
  void NULLPTR_NOTE(T* t, std::string&& msg = "NULLPTR_NOTE") const noexcept {
    if (t == nullptr)
      std::cout << "Note: " << std::forward<std::string>(msg) << std::endl;
  }
  void NULLPTR_WARNING(T* t, std::string&& msg = "NULLPTR_WARNING") const
      noexcept {
    if (t == nullptr)
      std::cerr << "Warning: " << std::forward<std::string>(msg) << std::endl;
  }
};
}
