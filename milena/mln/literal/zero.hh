// Copyright (C) 2007, 2008, 2009, 2011, 2013 EPITA Research and
// Development Laboratory (LRDE)
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

#ifndef MLN_LITERAL_ZERO_HH
# define MLN_LITERAL_ZERO_HH

/// \file
///
/// Definition of the literal of mln::zero.
///
/// \todo Macro-ification of similar code (when no trivial conversion ops are involved).

# include <mln/core/concept/literal.hh>
# include <mln/metal/converts_to.hh>


namespace mln
{

  namespace literal
  {

    /// Type of literal zero.
    struct zero_t : public Literal<zero_t>
    {
      // This default constructor is needed for compilation with gcc
      // 4.6.0, gcc 4.6.1 and Clang.
      zero_t();

      // FIXME: (for the record) Add "friend class Literal<zero_t>;" and a protected ctor.

      template <typename T>
      operator const T () const;
    };

    /*!
      \brief Literal zero.

      \relatesalso mln::literal::zero_t
      \relatesalso mln::algebra::vec
      \relatesalso mln::value::int_s
      \relatesalso mln::value::int_u_sat
      \relatesalso mln::value::qt::rgb32
      \relatesalso mln::value::rgb
      \relatesalso mln::value::sign

      \ingroup modliteral
    */
    extern const zero_t zero;

# ifndef MLN_INCLUDE_ONLY

    inline
    zero_t::zero_t()
    {
    }

    template <typename T>
    inline
    zero_t::operator const T () const
    {
      mlc_converts_to(int, T)::check();
      return 0;
    }

#  ifndef MLN_WO_GLOBAL_VARS

    const zero_t zero;

#  endif // !MLN_WO_GLOBAL_VARS

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::literal

} // end of namespace mln


#endif // ! MLN_LITERAL_ZERO_HH
