// Copyright (C) 2001, 2003, 2004, 2005 EPITA Research and Development Laboratory
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

#ifndef OLENA_CORE_ABSTRACT_IMAGE_HH
# define OLENA_CORE_ABSTRACT_IMAGE_HH

# include <oln/core/typedefs.hh>


# define oln_type_of_(ImageType, Alias) \
mlc_type_of_(oln, oln::category::image, ImageType, Alias)

# define oln_type_of(ImageType, Alias) \
mlc_type_of(oln, oln::category::image, ImageType, Alias)



namespace oln {


  // fwd decls
  namespace abstract
  {
    template <typename I> class image_without_nbh;
    template <typename I> class readonly_image;
  }


  namespace category
  {
    struct image;
  }


  /// Default properties of any image type.

  template <>
  struct set_default_props < category::image >
  {
    typedef mlc::undefined_type concrete_type;
    typedef mlc::undefined_type value_type;
    typedef mlc::undefined_type point_type;
    typedef mlc::undefined_type size_type;
    typedef mlc::undefined_type piter_type;
    typedef mlc::undefined_type fwd_piter_type;
    typedef mlc::undefined_type bkd_piter_type;

    typedef mlc::no_type value_storage_type;
    typedef mlc::no_type storage_type;
    typedef mlc::no_type delegated_type;
    typedef mlc::no_type neighb_type;


    typedef is_a<abstract::image_without_nbh> image_neighbness_type;
    typedef is_a<abstract::readonly_image> image_constness_type;
    typedef mlc::undefined_type image_dimension_type;

    //...

  };


  /// Retrieval of any image type properties (FIXME: say 'packing').

  template <typename I>
  struct get_props < category::image, I >
  {
    typedef oln_type_of(I, concrete) concrete_type;
    typedef oln_type_of(I, value) value_type;
    typedef oln_type_of(I, point) point_type;
    typedef oln_type_of(I, size) size_type;

    typedef oln_type_of(I, piter) piter_type;
    typedef oln_type_of(I, fwd_piter) fwd_piter_type;
    typedef oln_type_of(I, bkd_piter) bkd_piter_type;

    typedef oln_type_of(I, value_storage) value_storage_type;
    typedef oln_type_of(I, storage) storage_type;
    typedef oln_type_of(I, delegated) delegated_type;
    typedef oln_type_of(I, neighb) neighb_type;


    typedef oln_type_of(I, image_neighbness) image_neighbness_type;
    typedef oln_type_of(I, image_constness) image_constness_type;
    typedef oln_type_of(I, image_dimension) image_dimension_type;

    //...

    static void echo(std::ostream& ostr)
    {
      ostr << "props_of( oln::category::image, " << mlc_to_string(I) << " ) =" << std::endl
	   << "{" << std::endl

	   << "\t concrete_type = " << mlc_to_string(concrete_type) << std::endl
	   << "\t value_type = " << mlc_to_string(value_type) << std::endl
	   << "\t point_type = " << mlc_to_string(point_type) << std::endl
	   << "\t size_type = " << mlc_to_string(size_type) << std::endl

	   << "\t piter_type = " << mlc_to_string(piter_type) << std::endl
	   << "\t fwd_piter_type = " << mlc_to_string(fwd_piter_type) << std::endl
	   << "\t bkd_piter_type = " << mlc_to_string(bkd_piter_type) << std::endl

	   << "\t value_storage_type = " << mlc_to_string(value_storage_type) << std::endl
	   << "\t storage_type = " << mlc_to_string(storage_type) << std::endl
	   << "\t delegated_type = " << mlc_to_string(delegated_type) << std::endl
	   << "\t neighb_type = " << mlc_to_string(neighb_type) << std::endl

	   << "\t image_neighbness_type = " << mlc_to_string(image_neighbness_type)  << std::endl
	   << "\t image_constness_type = " << mlc_to_string(image_constness_type) << std::endl
	   << "\t image_dimension_type = " << mlc_to_string(image_dimension_type) << std::endl

	   << "}" << std::endl;
    }

    static void ensure()
    {
      mlc::is_ok< concrete_type >::ensure();
      mlc::is_ok< value_type >::ensure();
      mlc::is_ok< point_type >::ensure();
      mlc::is_ok< size_type >::ensure();
      mlc::is_ok< piter_type >::ensure();
      mlc::is_ok< fwd_piter_type >::ensure();
      mlc::is_ok< bkd_piter_type >::ensure();

      mlc::is_ok< value_storage_type >::ensure();
      mlc::is_ok< storage_type >::ensure();
      mlc::is_ok< delegated_type >::ensure();
      mlc::is_ok< neighb_type >::ensure();

      mlc::is_ok< image_neighbness_type >::ensure();
      mlc::is_ok< image_constness_type >::ensure();
      mlc::is_ok< image_dimension_type >::ensure();
    }
  };

} // end of namespace oln


# include <oln/core/abstract/internal/image_impl.hh>
# include <oln/core/gen/internal/value_box.hh>


namespace oln
{

  /*! \namespace oln::abstract
  ** \brief oln::abstract namespace.
  */
  namespace abstract {


    /*! \class abstract::image<E>
    **
    ** The abstract::image class is the base class from whom derives
    ** every concrete image class.  Basically, an image is a set of
    ** points and a set of values associated with those points.
    **
    ** Parameter E is the exact type of image.
    */


    template <typename E>
    struct image : public internal::get_image_impl < image<E>, E >
    {

      /// typedefs
      typedef oln_type_of(E, size)  size_type;
      typedef oln_type_of(E, value) value_type;
      typedef oln_type_of(E, point) point_type;


      /*------------------*
       ! abstract methods !
       *------------------*/


      /*! \brief Return the size of the current image.  Nota bene:
      **  this method is abstract-like.it is a pseudo-abstract method.
      **
      ** \return An object deriving from abstract::size.  Ex: if the
      ** image is an image2d<something>, the returned object is a
      ** size2d.
      */

      const size_type& size() const
      {
	return this->exact().impl_size();
      }


      /*! \brief Return the number of points of the current image.
      **  Nota bene: this method is abstract-like.
      **
      ** FIXME: this method should be deferred in an abstract subclass
      ** since its complexity can vary from a concrete image to another...
      **
      ** \return A positive value.  It can be 0 when the image size is
      ** not yet defined; ex: image2d<int> ima; cout << ima.npoints();
      */

      unsigned long npoints() const
      {
	return this->exact().impl_npoints();
      }


      /*! \brief Test if the point \a p belongs to the current image.
      ** Please note that a point of the outer boundary of an image
      ** does NOT belong to the image.  Nota bene: this method is
      ** abstract-like.
      **
      ** \return True if p belongs to the current image, false otherwise.
      **
      ** \see hold_large
      */

      bool hold(const point_type& p) const
      {
	precondition(this->npoints() != 0);
	return this->exact().impl_hold(p);
      }


      /*! \brief Test if \a p is a proper point to access a value of
      ** the current image.  When an image has an outer boundary, the
      ** points of this boundary have values and these values can be
      ** accessed.  Nota bene: this method is polymorphic so it can be
      ** overriden in derived classes; by default, hold_large behaves
      ** like hold.
      **
      ** FIXME: hold_large should come only for images WITH a border!
      **
      ** \return True if (*this)[p] is ok, false otherwise.
      **
      ** \see hold
      */

      bool hold_large(const point_type& p) const
      {
	precondition(this->npoints() != 0);
	return this->exact().impl_hold_large(p);
      }


      /*! \brief Default implementation for hold_large.  If not
      **  overriden in derived class, it just calls hold(p).
      **
      ** FIXME: hold_large should come only for images WITH a border!
      **
      ** \return True if (*this)[p] is ok, false otherwise.
      **
      ** \see hold_large
      */

      bool impl_hold_large(const point_type& p) const
      {
	return this->hold(p);
      }



      /*------------------*
       ! concrete methods !
       *------------------*/


      /*! \brief Give access to the value stored at \a p in the
      ** current image.  Precisely it returns a box that encloses this
      ** value.  FIXME: say more about that box!
      **
      ** \precondition this->hold_large(p)
      **
      ** \see value_box
      */

      value_box<const E> operator[](const point_type& p) const
      {
	precondition(this->npoints() != 0);
	precondition(this->hold_large(p));
	value_box<const E> tmp(this->exact(), p);
	return tmp;
      }


      /*! \brief Gives access to the value stored at \a p in the
      ** current image.  Precisely it returns a box that encloses this
      ** value.
      **
      ** \precondition this->hold_large(p)
      **
      ** \see value_box
      */

      value_box<E> operator[](const point_type& p)
      {
	precondition(this->npoints() != 0);
	precondition(this->hold_large(p));
	value_box<E> tmp(this->exact(), p);
	return tmp;
      }


      /*! \brief Destructor.
      */

      virtual ~image()
      {
	get_props<category::image, E>::ensure();
	// FIXME: static check fails because "pointer to member conversion via virtual base"...
// 	mlc_check_method_impl(E, const size_type&, size,       ,                  const);
// 	mlc_check_method_impl(E, unsigned long,    npoints,    ,                  const);
// 	mlc_check_method_impl(E, bool,             hold,       const point_type&, const);
// 	mlc_check_method_impl(E, bool,             hold_large, const point_type&, const);
// 	mlc_check_method_impl(E, const value_type, get,        const point_type&, const);
// 	mlc_check_method_impl_2(E, void,           resize_border, size_t, bool,   const);
      }



      // explanation:
      // ------------
      // though it works (uncommented) with g++-3.3, it is commented
      // for bwd compatibility purpose because of g++-2.95 that does not accept
      // "partial specialization declared friend"...
      /*
      friend class oln::value_box<E>;
      friend class oln::value_box<const E>;
      private:
      */

      /*! \brief Read-only access to the value stored at \a p in the
      ** current image.  This method is used in value_box<I>.  Client
      ** should not use this method but
      ** abstract::image<I>::operator[](point) instead.
      **
      ** \return The value stored at \a p.
      **
      ** \see value_box, abstract::image<I>::operator[](point)
      */

      const value_type get(const point_type& p) const
      {
	return this->exact().impl_get(p);
      }


      // FIXME: patch!

      void resize_border(size_t new_border, bool copy_border = false) const
      {
	this->exact().impl_resize_border(new_border, copy_border);
      }

    protected:

      /*! \brief Constructor (protected, empty).
      */
      image() {}

      /*! \brief Cpy constructor (protected, empty).
      */
      image(const image&) {}

      /*! \brief Op= (protected, empty).
      */
      void operator=(const image&) {}

    };



    /*! \namespace oln::abstract::internal
    ** \brief oln::abstract::internal namespace.
    */
    namespace internal {

      template <typename E>
      struct set_image_impl < image<E>, E > : public virtual image_impl<E>
      {

	/// typedefs

	typedef typename image_impl<E>::D D;

	typedef oln_type_of(D, size)  size_type;
	typedef oln_type_of(D, point) point_type;
	typedef oln_type_of(D, value) value_type;


	// delegations are "template methods" (Cf. the GOF's book)

	const size_type& impl_size() const
	{
	  const size_type& s = this->delegate().size();
	  this->exact().impl_size_extra(s);
	  return s;
	}

	unsigned long impl_npoints() const
	{
	  unsigned long n = this->delegate().npoints();
	  this->exact().impl_npoints_extra(n);
	  return n;
	}

	bool impl_hold(const point_type& p) const
	{
	  this->exact().impl_hold_extra(p);
	  return this->delegate().hold(p);
	}

	bool impl_hold_large(const point_type& p) const
	{
	  this->exact().impl_hold_large_extra(p);
	  return this->delegate().hold_large(p);
	}

	value_box<D> operator[](const point_type& p) const
	{
	  precondition(this->hold_large(p));
	  return this->delegate().operator[](p);
	}

	value_box<const D> operator[](const point_type& p)
	{
	  precondition(this->hold_large(p));
	  return this->delegate().operator[](p);
	}

	const value_type impl_get(const point_type& p) const
	{
	  this->exact().impl_get_extra(p);
	  return this->delegate().get(p);
	}

	// FIXME: patch

	void impl_resize_border(size_t new_border, bool copy_border) const
	{
	  this->delegate().impl_resize_border(new_border, copy_border);
	}

	// extra code; default is "do nothing"

	void impl_size_extra(const size_type& s) const {}
	void impl_npoints_extra(unsigned long n) const {}

	void impl_hold_extra(const point_type& p) const {}
	void impl_hold_large_extra(const point_type& p) const {}

	void impl_get_extra(const point_type&) const {}
      };

    } // end of namespace oln::abstract::internal

  } // end of namespace oln::abstract

} // end of namespace oln




#endif // ! OLENA_CORE_ABSTRACT_IMAGE_HH
