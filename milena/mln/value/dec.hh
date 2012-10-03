// Copyright (C) 2010, 2012 EPITA Research and Development Laboratory
// (LRDE)
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

#ifndef MLN_VALUE_DEC_HH
# define MLN_VALUE_DEC_HH

/// \file
///
/// Return the dec value.

# include <mln/value/iota.hh>
# include <mln/value/label.hh>


namespace mln
{

  namespace value
  {

    template <typename V>
    void
    dec(V&);


# ifndef MLN_INCLUDE_ONLY


    // Implementations

    namespace implementation
    {

      namespace generic
      {

	template <typename V>
	void
	dec(V& v)
	{
	  v -= mln::value::iota<V>::value();
	}

      } // end of namespace mln::value::implementation::generic


      template <typename V>
      void
      dec_label(V& v)
      {
	--v;
      }

    } // end of namespace mln::value::implementation



    // Dispatch

    namespace internal
    {

      template <unsigned n>
      void
      dec_dispatch(label<n>& v)
      {
	implementation::dec_label(v);
      }

      template <typename V>
      void
      dec_dispatch(V& v)
      {
	implementation::generic::dec(v);
      }

    } // end of namespace mln::value::internal



    // Facade

    template <typename V>
    void
    dec(V& v)
    {
      internal::dec_dispatch(v);
    }


# endif // ! MLN_INCLUDE_ONLY


  } // end of namespace mln::value

} // end of namespace mln

#endif // ! MLN_VALUE_DEC_HH
