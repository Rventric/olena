// Copyright (C) 2006 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 51 Franklin Street, Fifth Floor,
// Boston, MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

// Check Static's super relationship.

#include <mlc/is_a.hh>
#include <mlc/assert.hh>

#include <stc/scoop.hh>


// Namespace equipment.
stc_scoop_equipment_for_namespace(my);


namespace my
{

  /*----.
  | A.  |
  `----*/

  struct A
  {
  };


  /*------------.
  | B ---|> A.  |
  `------------*/

  // Forward declaration.
  struct B;

  // Set super type.
  template<>
  struct set_super_type<B>
  {
    typedef A ret;
  };

  struct B : public A
  {
  };


  /*---.
  | C. |
  `---*/

  // Forward declaration.
  struct C;

  /* C doesn't derive from B, but we want its vtypes to ``inherit''
     from B's vtypes.  Note that there are actually no vtypes in this
     simple test, but it doesn't matter: we are only checking the
     super relationship here.  */

  /// Link to B (``pseudo'' inheritance).
  template<>
  struct set_pseudosuper_type<C>
  {
    typedef B ret;
  };

  struct C // no inheritance
  {
  };

} // end of namespace my


int
main()
{
  mlc::assert_<mlc_is_a_(my::B, my::A)>::check();
  mlc::assert_<mlc_is_not_a_(my::C, my::A)>::check();
}
