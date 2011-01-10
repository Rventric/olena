// Copyright (C) 2007, 2008, 2009, 2010, 2011 EPITA Research and Development
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

#ifndef MLN_CORE_STATIC_DPOINTS_PIXTER_HH
# define MLN_CORE_STATIC_DPOINTS_PIXTER_HH

// FIXME: Review and update the documentation.

/// \file
/// 
/// \brief Definition of forward and backward mln::dpoint-based
/// iterators for pixels iterations.
///
/// \todo In ::init_ use offsets_wrt (defined in concept/window.hh).

# include <cassert>
# include <vector>

# include <mln/core/concept/proxy.hh>
# include <mln/core/concept/pixel_iterator.hh>
# include <mln/core/internal/pixel_impl.hh>
# include <mln/metal/converts_to.hh>

# include "apps/bench/static_array.hh"

namespace mln
{

  /*----------------------------------.
  | static_dpoints_fwd_pixter<I, W>.  |
  `----------------------------------*/

  /// \brief A generic forward iterator on the pixels of a
  /// dpoint-based window or neighborhood.
  /// 
  /// Parameter \c I is the image type.
  template <typename I, typename W>
  class static_dpoints_fwd_pixter
    : public Pixel_Iterator< static_dpoints_fwd_pixter<I, W> >,
      public internal::pixel_impl_< I, static_dpoints_fwd_pixter<I, W> >
  {
    typedef typename internal::pixel_impl_< I, static_dpoints_fwd_pixter<I, W> > super_;

  public:
    /// \brief Constructor (using an image).
    /// 
    /// \param[in] image The image to iterate over.
    /// \param[in] dps   An object (neighborhood or window) that can
    ///                  provide a set of delta-points.
    /// \param[in] p_ref Center (resp. reference) point of the
    ///                  neighborhood (resp. window).
    template <typename Dps, typename Pref>
    static_dpoints_fwd_pixter(I& image,
			      const Dps& dps,
			      const Pref& p_ref);

    /// \brief Constructor (using a generalized pixel).
    /// 
    /// \param[in] pxl_ref Center (generalized) pixel to iterate around.
    /// \param[in] dps     An object (neighborhood or window) that can
    ///                    provide a set of delta-points.
    template <typename Dps, typename Pref>
    static_dpoints_fwd_pixter(const Generalized_Pixel<Pref>& pxl_ref,
			      const Dps& dps);

    /// Manipulation.
    /// \{
    /// Start an iteration.
    void start();
    /// Go to the next pixel.
    void next_();

    /// Invalidate the iterator.
    void invalidate();
    /// Test the iterator validity.
    bool is_valid() const;

    /// Force this iterator to update its location to take into
    /// account that its center point may have moved.
    void update();
    /// \}

    /// The value around which this iterator moves.
    const mln_value(I)& center_val() const;

  private:
    template <typename Dps>
    void init_(const Dps& dps);

  private:
    enum { n = W::length };

    /// \brief Offset of each delta-point.
    ///
    /// offset_[0] is absolute, while other offsets are relative
    /// (i.e., offset_[i] is the memory difference to go from pixel
    /// i-1 to pixel i.
    util::static_array<int, n> offset_;
    /// Current offset.
    std::size_t i_;

    /// \brief Reference value or pixel.
    ///
    /// One and only one of these pointers should be non-null.
    /// \{
    /// Reference value the image
    mln_qlf_value(I)** value_ref_;
    /// Reference pixel / point in the image
    const mln_psite(I)* p_ref_;
    /// \}
  };


  /*----------------------------------.
  | static_dpoints_bkd_pixter<I, W>.  |
  `----------------------------------*/

  /// \brief A generic backward iterator on the pixels of a
  /// dpoint-based window or neighborhood.
  /// 
  /// Parameter \c I is the image type.
  template <typename I, typename W>
  class static_dpoints_bkd_pixter
    : public Pixel_Iterator< static_dpoints_bkd_pixter<I, W> >,
      public internal::pixel_impl_< I, static_dpoints_bkd_pixter<I, W> >
  {
    typedef typename internal::pixel_impl_< I, static_dpoints_bkd_pixter<I, W> > super_;

  public:
    /// \brief Constructor (using an image).
    /// 
    /// \param[in] image The image to iterate over.
    /// \param[in] dps   An object (neighborhood or window) that can
    ///                  provide a set of delta-points.
    /// \param[in] p_ref Center (resp. reference) point of the
    ///                  neighborhood (resp. window).
    template <typename Dps, typename Pref>
    static_dpoints_bkd_pixter(I& image,
			      const Dps& dps,
			      const Pref& p_ref);

    /// \brief Constructor (using a generalized pixel).
    /// 
    /// \param[in] pxl_ref Center (generalized) pixel to iterate around.
    /// \param[in] dps     An object (neighborhood or window) that can
    ///                    provide a set of delta-points.
    template <typename Dps, typename Pref>
    static_dpoints_bkd_pixter(const Generalized_Pixel<Pref>& pxl_ref,
			      const Dps& dps);

    /// Manipulation.
    /// \{
    /// Start an iteration.
    void start();
    /// Go to the next pixel.
    void next_();

    /// Invalidate the iterator.
    void invalidate();
    /// Test the iterator validity.
    bool is_valid() const;

    /// Force this iterator to update its location to take into
    /// account that its center point may have moved.
    void update();
    /// \}

    /// The value around which this iterator moves.
    const mln_value(I)& center_val() const;

  private:
    template <typename Dps>
    void init_(const Dps& dps);

  private:
    enum { n = W::length };

    /// \brief Offset of each delta-point.
    ///
    /// offset_[dps.size() - 1] is absolute, while other offsets
    /// are relative (i.e., offset_[i] is the memory difference to go
    /// from pixel i+1 to pixel i.
    util::static_array<int, n> offset_;
    /// Current offset.
    std::size_t i_;

    /// \brief Reference value or pixel.
    ///
    /// One and only one of these pointers should be non-null.
    /// \{
    /// Reference value the image
    mln_qlf_value(I)** value_ref_;
    /// Reference pixel / point in the image
    const mln_psite(I)* p_ref_;
    /// \}
  };



#ifndef MLN_INCLUDE_ONLY

  /*----------------------------------.
  | static_dpoints_fwd_pixter<I, W>.  |
  `----------------------------------*/

  template <typename I, typename W>
  template <typename Dps, typename Pref>
  inline
  static_dpoints_fwd_pixter<I, W>::static_dpoints_fwd_pixter(I& image,
							     const Dps& dps,
							     const Pref& p_ref)
    : super_(image)
  {
    mln_precondition(image.is_valid());

    mlc_converts_to(Pref, const mln_psite(I)&)::check();
    p_ref_ = & static_cast< const mln_psite(I)& >(p_ref);

    value_ref_ = 0;
    init_(dps);
  }

  template <typename I, typename W>
  template <typename Dps, typename Pref>
  inline
  static_dpoints_fwd_pixter<I, W>::static_dpoints_fwd_pixter(const Generalized_Pixel<Pref>& pxl_ref_,
							     const Dps& dps)
    : super_(internal::force_exact<Pref>(pxl_ref_).ima())
  {
    const Pref& pxl_ref = internal::force_exact<Pref>(pxl_ref_);
    mln_precondition(pxl_ref.ima().is_valid());
    p_ref_ = 0;
    // Potential promotion from (T**) to (const T**) shall be forced.
    value_ref_ = const_cast<mln_qlf_value(I)**>(pxl_ref.address_());
    init_(dps);
  }

  template <typename I, typename W>
  inline
  const mln_value(I)&
  static_dpoints_fwd_pixter<I, W>::center_val() const
  {
    mln_invariant(value_ref_ != 0 || p_ref_ != 0);
    if (p_ref_)
      return image_(*p_ref_);
    else
      return **value_ref_;
  }

  template <typename I, typename W>
  template <typename Dps>
  inline
  void
  static_dpoints_fwd_pixter<I, W>::init_(const Dps& dps)
  {
    for (unsigned i = 0; i < dps.size(); ++i)
      offset_[i] = this->image_.delta_index(dps.dp(i));
    // offset_[0] is absolute
    // other offsets are relative:
    if (dps.size() > 1)
      for (unsigned i = dps.size() - 1; i > 0; --i)
	offset_[i] -= offset_[i - 1];
    invalidate();
  }

  template <typename I, typename W>
  inline
  void
  static_dpoints_fwd_pixter<I, W>::update()
  {
    if (is_valid())
      {
	if (p_ref_)
	  this->value_ptr_ = & image_(*p_ref_) + offset_[i_];
	else
	  this->value_ptr_ = * value_ref_ + offset_[i_];
      }
  }

  template <typename I, typename W>
  inline
  void
  static_dpoints_fwd_pixter<I, W>::start()
  {
    i_ = 0;
    update();
  }

  template <typename I, typename W>
  inline
  void
  static_dpoints_fwd_pixter<I, W>::next_()
  {
    ++i_;
    if (is_valid())
      this->value_ptr_ += offset_[i_];
  }

  template <typename I, typename W>
  inline
  bool
  static_dpoints_fwd_pixter<I, W>::is_valid() const
  {
    return i_ < n;
  }

  template <typename I, typename W>
  inline
  void
  static_dpoints_fwd_pixter<I, W>::invalidate()
  {
    i_ = n;
  }


  /*----------------------------------.
  | static_dpoints_bkd_pixter<I, W>.  |
  `----------------------------------*/

  template <typename I, typename W>
  template <typename Dps, typename Pref>
  inline
  static_dpoints_bkd_pixter<I, W>::static_dpoints_bkd_pixter(I& image,
							     const Dps& dps,
							     const Pref& p_ref)
    : super_(image)
  {
    mln_precondition(image.is_valid());
    internal::get_adr(p_ref_, p_ref);
    value_ref_ = 0;
    init_(dps);
  }

  template <typename I, typename W>
  template <typename Dps, typename Pref>
  inline
  static_dpoints_bkd_pixter<I, W>::static_dpoints_bkd_pixter(const Generalized_Pixel<Pref>& pxl_ref_,
							     const Dps& dps)
    : super_(internal::force_exact<Pref>(pxl_ref_).ima())
  {
    const Pref& pxl_ref = internal::force_exact<Pref>(pxl_ref_);
    mln_precondition(pxl_ref.ima().is_valid());
    p_ref_ = 0;
    // Potential promotion from (T**) to (const T**) shall be forced.
    value_ref_ = const_cast<mln_qlf_value(I)**>(pxl_ref.address_());
    init_(dps);
  }

  template <typename I, typename W>
  inline
  const mln_value(I)&
  static_dpoints_bkd_pixter<I, W>::center_val() const
  {
    mln_invariant(value_ref_ != 0 || p_ref_ != 0);
    if (p_ref_)
      return image_(*p_ref_);
    else
      return **value_ref_;
  }

  template <typename I, typename W>
  template <typename Dps>
  inline
  void
  static_dpoints_bkd_pixter<I, W>::init_(const Dps& dps)
  {
    for (unsigned i = 0; i < dps.size(); ++i)
      offset_[i] = this->image_.delta_index(dps.dp(i));
    // offset_[size() - 1] is absolute
    // other offsets are relative:
    if (dps.size() > 1)
      for (unsigned i = 0; i < dps.size() - 1; ++i)
	offset_[i] -= offset_[i + 1];
    invalidate();
  }

  template <typename I, typename W>
  inline
  void
  static_dpoints_bkd_pixter<I, W>::update()
  {
    if (is_valid())
      {
	if (p_ref_)
	  this->value_ptr_ = & image_(*p_ref_) + offset_[n - 1 - i_];
	else
	  this->value_ptr_ = * value_ref_ + offset_[n - 1 - i_];
      }
  }

  template <typename I, typename W>
  inline
  void
  static_dpoints_bkd_pixter<I, W>::start()
  {
    i_ = 0;
    update();
  }

  template <typename I, typename W>
  inline
  void
  static_dpoints_bkd_pixter<I, W>::next_()
  {
    ++i_;
    if (is_valid())
      this->value_ptr_ += offset_[n - 1 - i_];
  }

  template <typename I, typename W>
  inline
  bool
  static_dpoints_bkd_pixter<I, W>::is_valid() const
  {
    return i_ < n;
  }

  template <typename I, typename W>
  inline
  void
  static_dpoints_bkd_pixter<I, W>::invalidate()
  {
    i_ = n;
  }

#endif // ! MLN_INCLUDE_ONLY

} // end of namespace mln


#endif // ! MLN_CORE_STATIC_DPOINTS_PIXTER_HH
