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

  private:
    std::stack<std::map<Key, Data>> stack{};

  public:
    scoped_map();
    template <Is_pointer T> T get(Key key);
    Data get(Key key);
    void scope_begin();
    void scope_end();
    void put(Key val1, Data val2);
  };

  // FIXED: Some code was deleted here.
  template <typename Key, typename Data>
  std::ostream& operator<<(std::ostream& ostr,
                           const scoped_map<Key, Data>& tbl);

} // namespace misc

#include <misc/scoped-map.hxx>
