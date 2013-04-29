// Copyright (C) 2007, 2008, 2009, 2013 EPITA Research and Development
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

#ifndef MLN_FUN_P2B_CHESS_HH
# define MLN_FUN_P2B_CHESS_HH

/// \file
///
/// \brief Generates a boolean image based on a chess-board canvas
/// (alternate True/False).


# include <mln/core/concept/function.hh>
# include <mln/core/alias/point2d.hh>


namespace mln
{

  namespace fun
  {

    namespace p2b
    {

      /* \brief Generates a boolean image based on a chess-board
	 canvas (alternate True/False).

	 This function allows to specify how large are the square in
	 the chess-board canvas.

	 \verbatim
	 | - | - | -
	 - | - | - |
	 | - | - | -
	 \endverbatim

	 \sa data::transform
	 \ingroup funv2v
      */
      struct chess : public Function_v2b< chess >
      {
	typedef bool result;
	bool operator()(const point2d& p) const;
      };


# ifndef MLN_INCLUDE_ONLY

      inline
      bool
      chess::operator()(const point2d& p) const
      {
	return (p.row() + p.col()) % 2 == 0;
      }

# endif // ! MLN_INCLUDE_ONLY

    } // end of namespace mln::fun::p2b

  } // end of namespace mln::fun

} // end of namespace mln


#endif // ! MLN_FUN_P2B_CHESS_HH
