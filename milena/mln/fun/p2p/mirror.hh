// Copyright (C) 2008, 2009, 2013 EPITA Research and Development
// Laboratory (LRDE)
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

#ifndef MLN_FUN_P2P_MIRROR_HH
# define MLN_FUN_P2P_MIRROR_HH

/// \file
///
/// \brief For sites outside a given domain it returns the
/// corresponding mirror site in the domain,

# include <mln/core/concept/function.hh>

namespace mln
{

  namespace fun
  {

    namespace p2p
    {

      /*!  \brief For sites outside a given domain it returns the
       *   corresponding mirror site in the domain,

       Example:

       \verbatim
       (0,0)
         x -------- x
	 |          |
	 |          |
	 x -------- x
	          (3,3)
       \endverbatim

       \code
       fun::p2p::mirror<box2d> f_mirror(box2d(4,4));
       point2d p = f_mirror(point2d(0, 6));
       // p == (0, 1);
       \endcode

       \sa data::transform
       \ingroup funv2v
      */
      template <typename B>
      struct mirror : public Function_v2v< mirror<B> >
      {
        typedef mln_psite(B) result;

        mirror(const B& box);

        mln_psite(B)
        operator()(const mln_psite(B)&) const;

        const B& box_;
      };

# ifndef MLN_INCLUDE_ONLY

      template <typename B>
      inline
      mirror<B>::mirror(const B& box)
        : box_(box)
      {
      }

      template <typename B>
      inline
      mln_psite(B)
      mirror<B>::operator()(const mln_psite(B)& p) const
      {
        mln_precondition(box_.has(p));

        mln_psite(B) r(p);
        r[0] = box_.max()[0] - p[0];

        return r;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2p

  } // end of namespace mln::fun

} // end of namespace mln

#endif // ! MLN_FUN_P2P_MIRROR_HH
