/**
 ** \file misc/scoped-map.hh
 ** \brief Declaration of misc::scoped_map.
 **
 ** This implements a stack of dictionnaries.  Each time a scope is
 ** opened, a new dictionnary is added on the top of the stack; the
 ** dictionary is removed when the scope is closed.  Lookup of keys
 ** is done in the last added dictionnary first (LIFO).
 **
 ** In particular this class is used to implement symbol tables.
 **/

#pragma once

#include <map>
#include <stack>
#include <vector>

namespace misc
{
  template <typename Data>
  concept Is_pointer = requires(Data a)
  {
    std::is_pointer<Data>(a) == true;
  };

  template <typename Key, typename Data> class scoped_map
  {
    // FIXED: Some code was deleted here.

  public:
    std::stack<std::map<Key, Data>> stack;
    std::map<Key, Data> current_scope{};

    template <Is_pointer T> T get(Key key)
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

    Data get(Key key)
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

    void scope_begin()
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
      stack.push(map_);
    }

    void scope_end()
    {
      if (stack.size() == 0)
        {
          return;
        }
      stack.pop();
    }

    void put(Key val1, Data val2)
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
  };

  // FIXED: Some code was deleted here.
  template <typename Key, typename Data>
  std::ostream& operator<<(std::ostream& ostr,
                           const scoped_map<Key, Data>& tbl);

} // namespace misc

#include <misc/scoped-map.hxx>
