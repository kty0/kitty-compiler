/** \file misc/scoped-map.hxx
 ** \brief Implementation of misc::scoped_map.
 */

#pragma once

#include <sstream>
#include <stdexcept>
#include <type_traits>

#include <ranges>
#include <stack>
#include <misc/algorithm.hh>
#include <misc/contract.hh>
#include <misc/indent.hh>
#include <misc/scoped-map.hh>

namespace misc
{
  // FIXED: Some code was deleted here.

  template <typename Key, typename Data>
  inline scoped_map<Key, Data>::scoped_map()
  {
    std::map<Key, Data> first_scope{};
    stack.push(first_scope);
  }

  template <typename Key, typename Data>
  template <Is_pointer T>
  inline T scoped_map<Key, Data>::get(Key key)

  {
    if (stack.size() == 0)
      {
        throw std::invalid_argument("no scope begin");
      }
    auto map = stack.top();
    stack.pop();
    auto it = map.find(key);
    if (it == map.end())
      {
        return nullptr;
      }
    stack.push(map);
    return *it;
  }

  template <typename Key, typename Data>
  inline Data scoped_map<Key, Data>::get(Key key)
  {
    if (stack.size() == 0)
      {
        throw std::invalid_argument("no scope begin");
      }
    auto map = stack.top();
    stack.pop();
    auto it = map.find(key);
    if (it == map.end())
      {
        throw std::invalid_argument("invalid key");
      }
    stack.push(map);
    return (*it).second;
  }

  template <typename Key, typename Data>
  inline void scoped_map<Key, Data>::scope_begin()
  {
    if (stack.size() == 0)
      {
        std::map<Key, Data> map{};
        stack.push(map);
        return;
      }
    std::map<Key, Data> map_ = stack.top();
    stack.pop();
    std::map<Key, Data> map(map_);
    stack.push(map_);
    stack.push(map);
  }

  template <typename Key, typename Data>
  inline void scoped_map<Key, Data>::scope_end()
  {
    if (stack.size() == 0)
      {
        return;
      }
    stack.pop();
  }

  template <typename Key, typename Data>
  inline void scoped_map<Key, Data>::put(Key val1, Data val2)
  {
    if (stack.size() == 0)
      {
        return;
      }
    auto map = stack.top();
    stack.pop();
    map[val1] = val2;
    stack.push(map);
  }

  template <typename Key, typename Data>
  inline std::ostream& operator<<(std::ostream& ostr,
                                  const scoped_map<Key, Data>& tbl)
  {
    return tbl.dump(ostr);
  }

} // namespace misc
