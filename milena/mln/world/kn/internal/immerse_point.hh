// Copyright (C) 2012 EPITA Research and Development Laboratory (LRDE)
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

/// \file
///
/// \brief Returns the corresponding point in Kn from a point in K0.

#ifndef MLN_WORLD_KN_IMMERSE_POINT_HH
# define MLN_WORLD_KN_IMMERSE_POINT_HH

# include <cmath>
# include <mln/core/alias/point2d.hh>

namespace mln
{

  namespace world
  {

    namespace kn
    {

      namespace internal
      {


	/// \brief Returns the corresponding point in Kn from a point
	/// in K0.
	point2d	immerse_point(const def::coord& row,
			      const def::coord& col,
			      const unsigned n,
			      const unsigned inner_border_thickness);

	/// \overload
	/// \p inner_border_thickness is set to 0.
	point2d	immerse_point(const def::coord& row,
			      const def::coord& col,
			      const unsigned n);

	/// \overload
	point2d	immerse_point(const point2d& p, const unsigned n,
			      const unsigned inner_border_thickness);

	/// \overload
	/// \p inner_border_thickness is set to 0.
	point2d	immerse_point(const point2d& p, const unsigned n);


# ifndef MLN_INCLUDE_ONLY


	inline
	point2d	immerse_point(const point2d& p, const unsigned n,
			      const unsigned inner_border_thickness)
	{
	  return immerse_point(p.row(), p.col(), n,
			       inner_border_thickness);
	}

	inline
	point2d	immerse_point(const point2d& p, const unsigned n)
	{
	  return immerse_point(p.row(), p.col(), n, 0);
	}


	inline
	point2d	immerse_point(const def::coord& row,
			      const def::coord& col,
			      const unsigned n,
			      const unsigned inner_border_thickness)
	{
	  point2d tmp(std::pow(2u, n) * row + 2 * inner_border_thickness,
		      std::pow(2u, n) * col + 2* inner_border_thickness);
	  return tmp;
	}

	inline
	point2d	immerse_point(const def::coord& row,
			      const def::coord& col,
			      const unsigned n)
	{
	  return immerse_point(row, col, n, 0);
	}


# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace mln::work::kn::internal

    } // end of namespace mln::world::kn

  } // end of namespace mln::world

} // end of namespace mln

#endif // ! MLN_WORLD_KN_IMMERSE_POINT_HH


