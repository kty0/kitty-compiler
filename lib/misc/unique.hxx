/**
 ** \file misc/unique.hxx
 ** \brief Inline implementation of misc::unique.
 */

#pragma once

#include <misc/contract.hh>
#include <misc/unique.hh>

namespace misc
{
  template <typename T, class C> unique<T, C>::unique(const data_type& s)
  // FIXED: Some code was deleted here (Initializations).
  /** \brief Following the Flyweight design pattern, set the attribute to a
       unique reference of value s. You might want to check out std::set methods
       on cppreference.com. */
  /*
   * This constructor adds the given object to the global set and initializes
   * the obj_ attribute with the address of the corresponding set element.
   */
  {
    auto& instance = object_set_instance();
    instance.insert(s);

    obj_ = &(*instance.find(s));
  }

  template <typename T, class C>
  typename unique<T, C>::object_set_type& unique<T, C>::object_set_instance()
  // FIXED: Some code was deleted here (Classical Singleton pattern, a la Scott Meyers').
  /** \brief Create a persistent instance of a set which would hold each value. */
  /*
   * Recall the singleton exercise from the C++ workshop: the instance is held
   * as a static member variable.
   */
  {
    static object_set_type instance;
    return instance;
  }

  template <typename T, class C>
  typename unique<T, C>::object_size_type unique<T, C>::object_map_size()
  // FIXED: Some code was deleted here.
  /*
   * This functions simply returns the size of the global set.
   */
  {
    auto& instance = object_set_instance();
    return instance.size();
  }

  template <typename T, class C>
  inline const typename unique<T, C>::data_type& unique<T, C>::get() const
  // FIXED: Some code was deleted here.
  /*
   * This function simply returns the stored object.
   */
  {
    return *obj_;
  }

  template <typename T, class C>
  inline unique<T, C>::operator const data_type&() const
  // FIXED: Some code was deleted here (Check Typecasts overload).
  /*
   * This function is an overload of a cast: it returns our object of type
   * unique casted to a const reference to the data_type.
   */
  {
    return *obj_;
  }

  template <typename T, class C>
  inline typename unique<T, C>::value_type&
  unique<T, C>::operator=(const value_type& rhs)
  {
    if (this != &rhs)
      obj_ = rhs.obj_;
    return *this;
  }

  template <typename T, class C>
  inline bool unique<T, C>::operator==(const value_type& rhs) const
  {
    return obj_ == rhs.obj_;
  }

  template <typename T, class C>
  inline bool unique<T, C>::operator!=(const value_type& rhs) const
  {
    return !operator==(rhs);
  }

  template <typename T, class C>
  inline bool unique<T, C>::operator<(const value_type& rhs) const
  {
    C cmp;
    assertion(obj_);
    assertion(rhs.obj_);
    return cmp(*obj_, *rhs.obj_);
  }

  template <typename T, class C>
  inline std::ostream& operator<<(std::ostream& ostr, const unique<T, C>& the)
  {
    return ostr << the.get();
  }

} // namespace misc
