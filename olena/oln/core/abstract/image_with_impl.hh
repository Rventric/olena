// Copyright (C) 2001, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_WITH_IMPL_HH
# define OLENA_CORE_ABSTRACT_IMAGE_WITH_IMPL_HH

# include <oln/core/abstract/image_with_dim.hh>
# include <oln/core/abstract/image_with_type.hh>
# include <oln/core/impl/image_impl.hh>

namespace oln {

  namespace abstract {

    template<class Impl, class Exact>
    class image_with_impl; // fwd_decl

  } // end of namespace abstract

    template<class Impl, class Exact>
    struct image_traits<abstract::image_with_impl<Impl, Exact> >: 
    public image_traits<abstract::image_with_dim<image_id<Exact>::dim, Exact> >, public image_traits<abstract::image_with_type<typename image_id<Exact>::value_type, Exact> >
    {
      typedef Impl impl_type;
    };


  namespace abstract {

    template<class Impl, class Exact>
    class image_with_impl: 
      public image_with_dim<image_id<Exact>::dim, Exact>, 
      public image_with_type<typename image_id<Exact>::value_type, Exact>
    {
    public:
      typedef typename image_traits<Exact>::point_type point_type;
      typedef typename image_traits<Exact>::iter_type iter_type;
      typedef typename image_traits<Exact>::fwd_iter_type fwd_iter_type;
      typedef typename image_traits<Exact>::bkd_iter_type bkd_iter_type;
      typedef typename image_traits<Exact>::value_type value_type;
      typedef typename image_traits<Exact>::size_type size_type;
      typedef typename image_traits<Exact>::impl_type impl_type;
      
      typedef image_with_impl<Impl, Exact> self_type;
      typedef Exact exact_type;
      typedef image_with_dim<image_id<Exact>::dim, Exact> super_image_with_dim;
      typedef image_with_type<typename image_id<Exact>::value_type, Exact> super_image_with_type;

      const value_type& at(const point_type& p) const
      {
	return impl_->at(p);
      }

      value_type& at(const point_type& p)
      {
	return impl_->at(p);
      }

      // shallow copy
      image_with_impl(self_type& rhs) 
	: super_image_with_dim(rhs),
	  super_image_with_type(rhs)
      {
	assertion(rhs.has_impl_());
	impl_ = rhs.impl();
	impl_->ref();
      }

      exact_type& assign(exact_type rhs) // shallow assignment
      {
	if ( &rhs == this )
	  return *this;
	if (impl_ != 0)
	  impl_->unref();
	impl_ = rhs.impl();
	if (impl_ != 0)
	  impl_->ref();
	return *this;
      }

      ~image_with_impl()
      {
	clear();
      }

      void clear()
      {
	if (impl_ != 0)
	  {
	    impl_->unref();
	    impl_ = 0; // security
	  }
      }

      const impl_type* impl() const
      {
	return impl_;
      }

      impl_type* impl()
      {
	return impl_;
      }

      static std::string name()
      {
	return
	  std::string("abstract::image_with_impl<")
	  + Impl::name() + ", "
	  + Exact::name() + ">";
      }

      void clone_to(impl_type* output_data) const
      {
	assertion(impl_ != 0);
	impl()->clone_to(output_data);
      }

      // borders

      void border_set_width(coord new_border, bool copy_border = false)
      {
	precondition(new_border >= 0);
	precondition(impl() != 0);
	if (border() == new_border)
	  return;			// Nothing to do.
	
	impl()->border_reallocate_and_copy(new_border, copy_border);
      }

      void border_adapt_width(coord min_border, bool copy_border =
			      false)
      {
	precondition(min_border >= 0);
	if (border() >= min_border)
	  return;			// Don't shrink.
	
	border_set_width(min_border, copy_border);
      }

      void border_adapt_copy(coord min_border)
      {
	border_adapt_width(min_border);
	impl()->border_replicate();
      }


      void border_adapt_mirror(coord min_border)
      {
	border_adapt_width(min_border);
	impl()->border_mirror();
      }

      void border_adapt_assign(coord min_border, value_type val)
      {
	border_adapt_width(min_border);
	impl()->border_assign(val);
      }


    protected:
      image_with_impl() : impl_(0)
      {}

      image_with_impl(impl_type* impl) : 
	super_image_with_dim(), 
	impl_(impl) {}
      
    private:
      impl_type* impl_;
      image_with_impl(const self_type& rhs); // w/o impl
    };

  } // end of namespace abstract
} // end of namespace oln

#endif // ! OLENA_CORE_ABSTRACT_IMAGE_WITH_IMPL_HH
