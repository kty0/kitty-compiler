/**
 ** \file misc/singleton.hh
 ** \brief Generic singleton
 */

#pragma once

namespace misc
{
  template <typename T> class Singleton
  {
    // FIXED: Some code was deleted here.
  protected:
    Singleton() = default;

  public:
    Singleton(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static const T& instance()
    {
      static T instance;
      return instance;
    }
  };

} // namespace misc
