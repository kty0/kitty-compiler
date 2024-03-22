/**
 ** \file misc/variant.hxx
 ** \brief Implementation of misc::variant.
 **/

#pragma once

#include <variant>

#include <type_traits>
#include <misc/variant.hh>

namespace misc
{
  template <typename T, typename... Ts>
  template <typename U>
    requires ContainsTypeSet<U, T, Ts...>
  variant<T, Ts...>::variant(const U& rhs)
    : super_type(rhs)
  {}

  template <typename T, typename... Ts>
  template <typename U>
    requires ContainsTypeSet<U, T, Ts...>
  variant<T, Ts...>& variant<T, Ts...>::operator=(const U& rhs)
  {
    // Don't assign to oneself.
    if (static_cast<const void*>(&rhs) != static_cast<const void*>(this))
      super_type::operator=(rhs);
    return *this;
  }

  template <typename T, typename... Ts>
  template <typename U>
    requires ContainsTypeGet<U, T, Ts...>
  variant<T, Ts...>::operator U&()
  {
    std::cout << "var3\n";
    auto res = std::get<U>(*this);
    std::cout << "var4\n";
    return res;
  }

  template <typename T, typename... Ts>
  template <typename U>
    requires ContainsTypeGet<U, T, Ts...>
  variant<T, Ts...>::operator const U&() const
  // FIXED: Some code was deleted here.
  {
    std::cout << "var1\n";
    auto res = std::get<U>(*this);
    std::cout << "var2\n";
    return res;
  }

  template <typename T, typename... Ts>
  template <typename V>
    requires Visits<V, T, Ts...>
  auto variant<T, Ts...>::visit(V&& visitor) const
  {
    return std::visit(visitor, static_cast<super_type>(*this));
  }

  template <typename T, typename... Ts>
  template <typename V, class... Variants>
  auto variant<T, Ts...>::visit(V&& visitor, Variants&&... vars)
  {
    return std::visit(visitor, static_cast<super_type>(vars)...);
  }

  template <typename T, typename... Ts>
  std::ostream& operator<<(std::ostream& os, const variant<T, Ts...>& obj)
  {
    PrintVisitor pv(os);
    return std::visit(pv, static_cast<const std::variant<T, Ts...>>(obj));
  }

  template <typename T>
  std::ostream& PrintVisitor::operator()(const T& value) const
  {
    return os_ << value;
  }
} // namespace misc
