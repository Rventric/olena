// Copyright (C) 2007, 2008, 2009 EPITA Research and Development Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef MLN_LITERAL_ONE_HH
# define MLN_LITERAL_ONE_HH

/// \file
///
/// Definition of the literal of mln::one.

# include <mln/core/concept/literal.hh>
# include <mln/metal/converts_to.hh>


namespace mln
{

  namespace literal
  {

    /// Type of literal one.
    struct one_t : public Literal<one_t>
    {
      // FIXME: Cf. comments in literal/zero.hh.

      template <typename T>
      operator const T () const;
    };


    /// Literal one.
    extern const one_t& one;


# ifndef MLN_INCLUDE_ONLY

    template <typename T>
    inline
    one_t::operator const T () const
    {
      mlc_converts_to(int, T)::check();
      return 1;
    }

#  ifndef MLN_WO_GLOBAL_VARS

    const one_t& one = one_t();

#  endif // !MLN_WO_GLOBAL_VARS

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_ONE_HH
