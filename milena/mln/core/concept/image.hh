// Copyright (C) 2007, 2008, 2009, 2011, 2012, 2013 EPITA Research and
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

#ifndef MLN_CORE_CONCEPT_IMAGE_HH
# define MLN_CORE_CONCEPT_IMAGE_HH

/// \file
///
/// Definition of the concept of mln::Image.

# include <mln/core/concept/site_set.hh>
# include <mln/core/concept/mesh.hh>

# include <mln/core/trait/all.hh> // FIXME: Move out of core!
# include <mln/core/macros.hh>
# include <mln/core/site_set/box.hh>

# include <mln/trait/concrete.hh> // FIXME: Should be in all.hh!
# include <mln/trait/images.hh>

# include <mln/metal/abort.hh>
# include <mln/metal/is_a.hh>
# include <mln/metal/equal.hh>

# include <mln/tag/init.hh>


namespace mln
{


  // Forward declarations.
  template <typename E> struct Image;
  template <typename E> struct Literal;
  template <typename D, typename W> struct w_window;


  /// \cond INTERNAL_API
  /// Image category flag type.
  template <>
  struct Image<void>
  {
    typedef Object<void> super;
  };
  /// \endcond


  /*!
    \headerfile <>
    \brief Base class for implementation of image classes.

    In Milena, every image is considered as a composition of both:
    \li A function \f$F\f$
    \f$
    ima : \left\{
    \begin{array}{lll}
    Site &\rightarrow & Value \\
    p & \mapsto & ima(p)
    \end{array}
    \right.
    \f$
    \li A domain of definition \f$D\f$.

    A domain \f$D\f$ is represented as a mln::Site_Set which provides
    the set of sites (localization information) on which the image is
    defined.
    The function \f$F\f$ returns a value for a given site.

    mln::Image is a concept class. It is not meant to be instantiated
    directly. All image classes in Milena inherit from it and
    provide the following methods and typedefs:

    \code
      typedef domain_t;
      typedef site;
      typedef psite;

      typedef piter;
      typedef fwd_piter;
      typedef bkd_piter;

      bool has(const psite& p) const;

      bool is_valid() const;

      typedef t_eligible_values_set;
      const t_eligible_values_set& values_eligible() const;

      typedef t_values_space;
      const t_values_space& values_space() const;

      typedef value;

      typedef rvalue;
      typedef lvalue;
      rvalue operator()(const psite& p) const;
      lvalue operator()(const psite& p);

      const domain_t& domain() const;

      typedef skeleton;
    \endcode

    More explanations and documentation of each typedefs/methods are
    provided in the inherited classes. A list of images is available
    in module \ref modimage. Image types are divided into two main
    categories: \ref modimageconcrete and \ref modimagemorpher.

    A list of features provided in image classes is detailed in module
    \ref modimage.

    \ingroup modconcepts
   */
  template <typename E>
  struct Image : public Object<E>
  {
    typedef Image<void> category;

    template <typename J>
    Image<E>& operator=(const J& rhs);

    // Required interface is provided by internal::image_base:

  protected:
    Image();
  };


  /*!
    \brief Conversion: image -> w_window
    \ingroup fromto
  */
  template <typename I, typename D, typename W>
  void
  from_to_(const Image<I>& from, w_window<D,W>& to);

  /*!
    \brief Conversion: values[] -> image
    \ingroup fromto
  */
  template <typename V, unsigned S, typename I>
  void
  from_to_(const V (&values)[S], Image<I>& to);


# ifndef MLN_INCLUDE_ONLY


  namespace internal
  {

    template <typename values_browsing_trait, typename E>
    struct image_values_interface_check
    {
      static void run() { /* No Requirement */}
    };

    template <typename E>
    struct image_values_interface_check<
      mln::trait::image::value_browsing::value_wise,
      E>
    {
      static void run()
      {
      }
    };


    // check_init

    template < typename E,
	       typename A >
    int check_init(void (E::*)(A))
    {
      return 0;
    }

    template < typename E,
	       typename A1, typename A2 >
    int check_init(void (E::*)(A1, A2))
    {
      return 0;
    }

    template < typename E,
	       typename A1, typename A2, typename A3 >
    int check_init(void (E::*)(A1, A2, A3))
    {
      return 0;
    }

  } // end of namespace mln::internal


  template <typename E>
  template <typename J>
  inline
  Image<E>&
  Image<E>::operator=(const J&)
  {
    /// You are assigning an image of a concrete type to
    /// an image with a concept type. It does NOT work.
    ///
    /// You forgot a call to exact() on the left operand!
    mlc_abort(E)::check();
    return *this;
  }


  template <typename E>
  inline
  Image<E>::Image()
  {
    // provided by internal::image_base:

    typedef mln_domain(E) domain_t;
    typedef mln_site(E)   site;
    typedef mln_psite(E)  psite;

    typedef mln_piter(E)     piter;
    typedef mln_fwd_piter(E) fwd_piter;
    typedef mln_bkd_piter(E) bkd_piter;

    bool (E::*m1)(const psite& p) const = & E::has;
    (void) m1;

    // Only some image types feature the 'nsites' method.
//     unsigned (E::*m2)() const = & E::nsites;
//     (void) m2;

    bool (E::*m3)() const = & E::is_valid;
    (void) m3;

    typedef typename E::t_eligible_values_set t_eligible_values_set;

    const t_eligible_values_set& (E::*m4)() const = & E::values_eligible;
    (void) m4;

    typedef typename E::t_values_space t_values_space;

    const t_values_space& (E::*m5)() const = & E::values_space;
    (void) m5;

    // to be provided in concrete image classes:

    typedef mln_value(E)  value;
    typedef mln_rvalue(E) rvalue;
    typedef mln_lvalue(E) lvalue;

    mlc_is_not_a(value, Literal)::check();

    // FIXME Doc
    //typedef mln_vset(E) vset;
    //const vset& (E::*m5)() const = & E::values;
    //(void) m5;

    rvalue (E::*m6)(const psite& p) const = & E::operator();
    (void) m6;

    lvalue (E::*m7)(const psite& p) = & E::operator();
    (void) m7;

    const domain_t& (E::*m8)() const = & E::domain;
    (void) m8;

    typedef typename E::skeleton skeleton;

    // Check E::init_ presence.  Since its signature varies from an
    // image type to another, that is the only thing we can ensure.
    internal::check_init(& E::init_);

    /// Optional interface:
    internal::image_values_interface_check<mln_trait_image_value_browsing(E),
      E>::run();
  }


  template <typename I, typename D, typename W>
  void
  from_to_(const Image<I>& from_, w_window<D,W>& to)
  {
    mlc_converts_to(mln_deduce(I, psite, delta), D)::check();
    mlc_converts_to(mln_value(I), W)::check();
    const I& ima = exact(from_);
    to.clear();
    mln_value(I) zero = literal::zero;
    mln_piter(I) p(ima.domain());
    for_all(p)
      if (ima(p) != zero)
	to.insert(ima(p), convert::to<D>(p));
  }

  template <typename V, unsigned S, typename I>
  void
  from_to_(const V (&values)[S], Image<I>& to_)
  {
    mlc_bool(S != 0)::check();
    mlc_converts_to(V, mln_value(I))::check();
    typedef mln_site(I) P;
    enum { d = P::dim,
	   s = mlc_root(d, S)::value };
    metal::bool_<(mlc_pow_int(s, d) == S)>::check();

    I& to = exact(to_);
    mln_precondition(! to.is_valid());

    box<P> b(all_to(0), all_to(s - 1));
    to.init_(b);
    mln_fwd_piter(box<P>) p(b);
    unsigned i = 0;
    for_all(p)
      to(p) = values[i++];
  }


# endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln

# include <mln/core/routine/initialize.hh>


#endif // ! MLN_CORE_CONCEPT_IMAGE_HH
