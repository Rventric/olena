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

#ifndef OLN_CORE_AUTOMATIC_IMAGE_IMAGE_BEING_VALUE_WISE_RANDOM_ACCESSIBLE_HH
# define OLN_CORE_AUTOMATIC_IMAGE_IMAGE_BEING_VALUE_WISE_RANDOM_ACCESSIBLE_HH

# include <oln/core/automatic/impl.hh>
# include <oln/morpher/tags.hh>


namespace oln
{

  // Forward declaration.
  namespace abstract
  {
    template <typename E> class image_being_value_wise_random_accessible;
    template <typename E> class mutable_image_being_value_wise_random_accessible;

  } // end of namespace oln::abstract


  namespace automatic
  {

    /// Default implementation corresponding to the interface
    /// oln::abstract::image_being_value_wise_random_accessible.
    template <typename E, typename M>
    class set_impl<abstract::image_being_value_wise_random_accessible, M, E> :
      public virtual stc::any__simple<E>
    {
    public:
      typedef oln_type_of(E, value) value_type;
      typedef oln_type_of(E, fwd_viter) fwd_viter_type;

    public:
      fwd_viter_type impl_value(const value_type& v) const;
    };


    /// Default implementation corresponding to the interface
    /// oln::abstract::mutable_image_being_value_wise_random_accessible.
    template <typename E, typename M>
    class set_impl<abstract::mutable_image_being_value_wise_random_accessible,
		   M, E> :
      public virtual stc::any__simple<E>
    {
    public:
      typedef oln_type_of(E, value) value_type;
      typedef oln_type_of(E, fwd_viter) fwd_viter_type;

    public:
      fwd_viter_type impl_value(const value_type& v);
    };



# ifndef OLN_INCLUDE_ONLY

    template <typename E, typename M>
    typename set_impl<abstract::image_being_value_wise_random_accessible, M, E>::fwd_viter_type
    set_impl<abstract::image_being_value_wise_random_accessible, M, E>::impl_value(const typename set_impl<abstract::image_being_value_wise_random_accessible, M, E>::value_type& v) const
    {
      return this->exact().delegate().value(v);
    }

    template <typename E, typename M>
    typename set_impl<abstract::mutable_image_being_value_wise_random_accessible, M, E>::fwd_viter_type
    set_impl<abstract::mutable_image_being_value_wise_random_accessible, M, E>::impl_value(const typename set_impl<abstract::mutable_image_being_value_wise_random_accessible, M, E>::value_type& v)
    {
      return this->exact().delegate().value(v);
    }

# endif

  } // end of namespace oln::automatic
  
} // end of namespace oln

#endif // ! OLN_CORE_AUTOMATIC_IMAGE_IMAGE_BEING_VALUE_WISE_RANDOM_ACCESSIBLE_HH
