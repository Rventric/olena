// Copyright (C) 2004  EPITA Research and Development Laboratory
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
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
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

#ifndef OLENA_SNAKES_ENERGIES_HH
# define OLENA_SNAKES_ENERGIES_HH

# include "node.hh"
# include <ntg/float.hh>

namespace oln {

  namespace snakes {

    /*! Base class for energy.
    */
    template <class I>
    class energy
    {
    public:
      energy() {}
      energy(void *) {}

    public:
      /*! Return the energy.
      **
      ** The first arg is the gradient of the image;
      ** the 3 nodes are the previous, the current and the next node.
      */
      ntg::float_s
      compute(const I&, const node<I>&, const node<I>&, const node<I>&)
      {
	// This is intended to cause an error. The user must define a
	// member function named `compute()' for each external energy,
	// otherwise the following method will be compiled and cause an
	// error.
	user_defined_external_energy_functor::compute();
	return 0.f;
      }

    public:
      //! FIXME: What is that?
      static void* cookie() { return 0; };

    private:
      struct user_defined_external_energy_functor{};
    };


    /*! Energy of continuity.
    **
    ** The goal of this energy is to avoid pack of nodes an lack
    ** of nodes in some part of the snake. The average distance
    ** between two consecutive points is \a average_dist. The
    ** more the distance between \a prev and \a current is
    ** far from \a average_dist, the higher the energy is.
    */
    template <class I>
    class continuity_energy : public energy<I>
    {
    public:
      typedef I image_type;

    public:
      continuity_energy(ntg::float_s* average_dist) :
	average_dist(average_dist) {}

    public:
      inline
      ntg::float_s
      compute(const I&,
	      const node<I>& prev,
	      const node<I>& current,
	      const node<I>&);

    private:
      ntg::float_s* average_dist;
    };


    /*! Energy of curvature.
    **
    ** The snake is supposed to be applied on object that
    ** have smooth edge (example: an egg). The more the
    ** \a prev \a current \a next nodes are aligned, the
    ** less the energy is.
    */
    template <class I>
    class curvature_energy : public energy<I>
    {
    public:
      typedef I image_type;

    public:
      curvature_energy(void *) {}

    public:
      static inline
      ntg::float_s
      compute(const I&,
	      const node<I>& prev,
	      const node<I>& current,
	      const node<I>& next);
    };


    /*! Energy of the gradient.
    **
    ** The snake should follow the edge of the object.
    ** The higher the gradient is, the less the energy is.
    */
    template <class I>
    class image_energy : public energy<I>
    {
    public:
      typedef I image_type;

    public:
      image_energy(void*) {}

    public:
      static inline
      ntg::float_s
      compute(const I& gradient,
	      const node<I>&,
	      const node<I>& current,
	      const node<I>&);
    };


    /*! This macro allows the user to define his own external energy.
    **
    ** \see dummy_energy
    */
#define oln_snakes_define_external_energy(Energy, Gradient, PrevNode, CurrentNode, NextNode)	\
												\
template<class I>										\
class Energy : public energy<I>									\
{												\
public:												\
  typedef I image_type;										\
  												\
public:												\
  Energy(void*) {}										\
  												\
public:												\
  ::ntg::float_s										\
  compute(const I& gradient,									\
	  const ::oln::snakes::node<I>&,							\
	  const ::oln::snakes::node<I>&,							\
	  const ::oln::snakes::node<I>&);							\
};												\
    												\
template<class I>										\
::ntg::float_s											\
Energy<I>::compute(const I& Gradient,								\
		   const ::oln::snakes::node<I>& PrevNode,					\
		   const ::oln::snakes::node<I>& CurrentNode,					\
		   const ::oln::snakes::node<I>& NextNode)


    //! Default external energy.
    oln_snakes_define_external_energy(dummy_energy,,,,)
    {
      return 0;
    }

  } // end snakes

} // end oln

#include <oln/snakes/energies.hxx>

#endif // !OLENA_SNAKES_ENERGIES_HH
