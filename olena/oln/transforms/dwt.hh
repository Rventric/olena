// Copyright (C) 2001, 2002, 2003  EPITA Research and Development Laboratory
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

#ifndef OLENA_TRANSFORMS_DWT_HH
# define OLENA_TRANSFORMS_DWT_HH

# include <oln/config/system.hh>
# include <oln/basics.hh>

# include <ntg/basics.hh>
# include <ntg/float.hh>
# include <ntg/utils/cast.hh>
# include <mlc/array/1d.hh>

// FIXME: this file has not been adjusted to the coding style since it
// will be completely rewritten in next release.

namespace oln {

  namespace transforms {

    // macros used to define all the wavelets coefficients
    
# define Wavelet_coeffs_definition(Name, Type, Size) \
    struct Name : public oln::internal::wavelet_coeffs_<Type, Size, Name> \
    { \
      Name()

# define Wavelet_coeffs_begin \
      h = (wc_start =

# define Wavelet_coeffs_end \
        , end); \
      init(); \
    }

    typedef enum {
      dwt_std,
      dwt_non_std
    } dwt_transform_type;

  } // end of namespace transforms

  namespace internal
  {

    static const ntg::float_d ln_2_ = 0.6931471805599453092;

    //
    // wavelet_coeffs_<T, N, Self>
    //
    //////////////////////////////////////

    template <class T, unsigned N, class Self>
    struct wavelet_coeffs_
    {
      typedef T		value_t;
      typedef Self	self_t;

    public:

      // accessors
      const value_t getG(unsigned i) const { return g[i]; }
      const value_t getInvG(unsigned i) const { return ig[i]; }
      const value_t getH(unsigned i) const { return h[i]; }
      const value_t getInvH(unsigned i) const { return ih[i]; }

      const unsigned size() const { return size_; }

    protected:

      void init()
      {
	for (unsigned i = 0; i < size_; i += 2) {
	  g[i] = -h[size_ - 1 - i];
	  g[i + 1] = h[size_ - 2 - i];
	  ig[size_ - 1 - i] = g[i + 1];
	  ig[size_ - 2 - i] = h[i + 1];
	  ih[size_ - 1 - i] = g[i];
	  ih[size_ - 2 - i] = h[i];
	}
      }

      wavelet_coeffs_(){}

      ~wavelet_coeffs_()
      {
	mlc::is_false<N % 2>::ensure();
      }

      mlc::array1d< mlc::array1d_info<N>, value_t>	h;
      mlc::internal::array1d_start_<value_t>		wc_start;

    private:

      value_t	g[N];
      value_t	ih[N];
      value_t	ig[N];
      enum {
	size_ = N
      };
    };

    // _dwt_transform_step

    template <class I, class K>
    void dwt_transform_step_(abstract::image<I>& im,
			     const oln_point_type(I)& p_,
			     const unsigned d,
			     const unsigned n,
			     const K& coeffs)
    {
      assertion(n >= coeffs.size());

      const unsigned half = n >> 1;
      unsigned lim = n + 1 - coeffs.size();
      ntg::float_d* tmp = new ntg::float_d[n];
     oln_point_type(I) p(p_);
      unsigned i, j, k;

      for (i = 0, j = 0; j < lim; j += 2, i++) {
	tmp[i] = 0;
	tmp[i + half] = 0;
	for (k = 0; k < coeffs.size(); k++) {
	  p.nth(d) = j + k;
	  tmp[i] += im[p] * coeffs.getH(k);
	  tmp[i + half] += im[p] * coeffs.getG(k);
	}
      }
      for (; i < half; j += 2, i++) {
	tmp[i] = 0;
	tmp[i + half] = 0;
	for (unsigned k = 0; k < coeffs.size(); k++) {
	  p.nth(d) = (j + k) % n;
	  tmp[i] += im[p] * coeffs.getH(k);
	  tmp[i + half] += im[p] * coeffs.getG(k);
	}
      }

      for (i = 0; i < n; i++) {
	p.nth(d) = i;
	im[p] = tmp[i];
      }

      delete[] tmp;
    }

    // _dwt_transform_inv_step

    template <class I, class K>
    void dwt_transform_inv_step_(abstract::image<I>& im,
				 const oln_point_type(I)& p_,
				 const unsigned d,
				 const unsigned n,
				 const K& coeffs)
    {
      assertion(n >= coeffs.size());

      const unsigned half = n >> 1;
      unsigned lim = coeffs.size() - 2;
      ntg::float_d* tmp = new ntg::float_d[n];
     oln_point_type(I) p(p_), q(p_);
      unsigned i, j, k, l;

      for (i = half - lim / 2, j = 0; j < lim; i++, j += 2) {
	tmp[j] = 0;
	tmp[j + 1] = 0;
	for (k = 0, l = 0; l < coeffs.size(); k++, l += 2) {
	  p.nth(d) = (i + k) % half;
	  q.nth(d) = p.nth(d) + half;
	  tmp[j] += im[p] * coeffs.getInvH(l) + im[q] * coeffs.getInvH(l + 1);
	  tmp[j + 1] += im[p] * coeffs.getInvG(l) + im[q] * coeffs.getInvG(l + 1);
	}
      }
      lim = n - 1;
      for (i = 0; j < lim; i++, j += 2) {
	tmp[j] = 0;
	tmp[j + 1] = 0;
	for (k = 0, l = 0; l < coeffs.size(); k++, l += 2) {
	  p.nth(d) = i + k;
	  q.nth(d) = p.nth(d) + half;
	  tmp[j] += im[p] * coeffs.getInvH(l) + im[q] * coeffs.getInvH(l + 1);
	  tmp[j + 1] += im[p] * coeffs.getInvG(l) + im[q] * coeffs.getInvG(l + 1);
	}
      }

      for (i = 0; i < n; i++) {
	p.nth(d) = i;
	im[p] = tmp[i];
      }

      delete[] tmp;
    }

    // Functions used to iterate over all dimensions except one

    typedef enum {
      dwt_fwd,
      dwt_bwd
    } dwt_transform_dir_;

    template <unsigned dim, unsigned skip,
	      unsigned current = dim>
    struct dim_skip_iterate_rec_
    {
      template <class I, class K>
      static void doit(abstract::image<I>& im,
		      oln_point_type(I)& p,
		       const unsigned l1,
		       const unsigned l2,
		       const K& coeffs,
		       dwt_transform_dir_ d)
      {
	if (current != skip) {
	  for (unsigned i = 0; i < l2; i++) {
	    p.nth(current - 1) = i;
	    dim_skip_iterate_rec_<dim, skip, current - 1>::
	      doit(im, p, l1, l2, coeffs, d);
	  }
	  p.nth(current - 1) = 0;
	}
	else
	  dim_skip_iterate_rec_<dim, skip, current - 1>::
	    doit(im, p, l1, l2, coeffs, d);
      }
    };

    template <unsigned dim, unsigned skip>
    struct dim_skip_iterate_rec_<dim, skip, 0>
    {
      template <class I, class K>
      static void doit(abstract::image<I>& im,
		      oln_point_type(I)& p,
		       const unsigned l1,
		       const unsigned l2,
		       const K& coeffs,
		       dwt_transform_dir_ d)
      {
	unsigned n;

	switch (d) {
	case dwt_fwd:
	  for (n = l2; n >= l1; n >>= 1)
	    dwt_transform_step_(im, p, skip - 1, n, coeffs);
	  break;
	case dwt_bwd:
	  for (n = l1; n <= l2; n <<= 1)
	    dwt_transform_inv_step_(im, p, skip - 1, n, coeffs);
	  break;
	};
      }
    };

    template <unsigned dim, unsigned skip>
    struct dim_iterate_rec_
    {
      template <class I, class K>
      static void doit(abstract::image<I>& im,
		      oln_point_type(I)& p,
		       const unsigned l1,
		       const unsigned l2,
		       const K& coeffs,
		       dwt_transform_dir_ d)
      {
	dim_skip_iterate_rec_<dim, skip>::doit(im, p, l1, l2, coeffs, d);
	dim_iterate_rec_<dim, skip - 1>::doit(im, p, l1, l2, coeffs, d);
      }
    };

    template <unsigned dim>
    struct dim_iterate_rec_<dim, 0>
    {
      template <class I, class K>
      static void doit(abstract::image<I>& ,
		      oln_point_type(I)& ,
		       const unsigned ,
		       const unsigned ,
		       const K& ,
		       dwt_transform_dir_ )
      {
	// end of recursion
      }
    };

    template <class I, class K>
    void dwt_transform_(abstract::image<I>& im,
			const unsigned l1,
			const unsigned l2,	
			const K& coeffs,
			transforms::dwt_transform_type t)
    {
     oln_point_type(I) p;

      switch (t) {
      case transforms::dwt_std:
	dim_iterate_rec_<I::dim, I::dim>::doit(im, p, l1, l2, coeffs, dwt_fwd);
	break;
      case transforms::dwt_non_std:
	for (unsigned n = l2; n >= l1; n >>= 1)
	  dim_iterate_rec_<I::dim, I::dim>::doit(im, p, n, n, coeffs, dwt_fwd);	  
	break;
      }
    }

    template <class I, class K>
    void dwt_transform_inv_(abstract::image<I>& im,
			    const unsigned l1,
			    const unsigned l2,	
			    const K& coeffs,
			    transforms::dwt_transform_type t)
    {
     oln_point_type(I) p;
      
      switch (t) {
      case transforms::dwt_std:
	dim_iterate_rec_<I::dim, I::dim>::doit(im, p, l1, l2, coeffs, dwt_bwd);
	break;
      case transforms::dwt_non_std:
	for (unsigned n = l1; n <= l2; n <<= 1)
	  dim_iterate_rec_<I::dim, I::dim>::doit(im, p, n, n, coeffs, dwt_bwd);	  
	break;
      }
    }

  } // end of namespace internal

  namespace transforms {

    //
    // dwt<T, K>
    //
    //////////////////////////////////////

    template <class I, class K>
    class dwt
    {
    public:

      typedef I					original_im_t;
      typedef oln_value_type(I)				original_im_value_t;
      typedef typename mute<I, ntg::float_d>::ret	trans_im_t;
      typedef typename K::self_t		coeffs_t;

      dwt(const original_im_t& im) : original_im(im)
      {
	ntg_is_a(original_im_value_t, ntg::real)::ensure();

	im_size = im.size().nth(0);
	assertion(im_size >= coeffs.size());
	for (unsigned i = 1; i < original_im_t::dim; i++)
	  assertion(im_size == static_cast<unsigned>(im.size().nth(i)));

	max_level = static_cast<unsigned>(log(2 * im_size / coeffs.size()) /
			       internal::ln_2_);

	current_level = max_level;

	assertion(!(im_size % (1 << max_level)));
	
	trans_im = trans_im_t(im.size());
      }

      const trans_im_t transformed_image() const
      {
	return trans_im;
      }

      trans_im_t& transformed_image()
      {
	return trans_im;
      }

      trans_im_t transform(dwt_transform_type t = dwt_non_std,
			   bool normalized = true, unsigned l = 0)
      {
	if (l == 0) {
	  l = max_level;
	  current_level = max_level;
	} else {
	  if (l > max_level)
	    l = max_level;
	  current_level = l;
	}

oln_iter_type(trans_im_t) it(trans_im);
	if (normalized) {
	  norm = pow(sqrt(2), original_im_t::dim * l);
	  for_all(it)
	    trans_im[it] = original_im[it] / norm;
	}
	else {
	  norm = 1;
 	  for_all(it)
 	    trans_im[it] = original_im[it];
	}

	unsigned lim = im_size >> (l - 1);
	transform_type = t;

	internal::dwt_transform_(trans_im, lim, im_size, coeffs, t);

	return trans_im;
      }

      trans_im_t transform_inv(unsigned l = 0)
      {
	if (l == 0)
	  l = current_level;
	else if (l > current_level)
	  l = current_level;

	trans_im_t new_im(trans_im.size());
oln_iter_type(trans_im_t) it(trans_im);
	if (norm != 1) {
	  for_all(it)
	    new_im[it] = trans_im[it] * norm;
	}
	else
	  for_all(it)
	    new_im[it] = trans_im[it];

	unsigned lim1 = im_size >> (current_level - 1);
	unsigned lim2 = im_size >> (current_level - l);

	internal::dwt_transform_inv_(new_im, lim1, lim2, coeffs, transform_type);

	return new_im;
      }

      template <class T1>
      typename mute<I, T1>::ret transform_inv(unsigned l = 0)
      {
	ntg_is_a(T1, ntg::real)::ensure();

	trans_im_t tmp_im = transform_inv(l);
	typename mute<I, T1>::ret new_im(tmp_im.size());

oln_iter_type(trans_im_t) it(tmp_im);
	for_all(it)
	  new_im[it] = (tmp_im[it] >= ntg_inf_val(T1) ?
			(tmp_im[it] <= ntg_sup_val(T1) ?
			 tmp_im [it] :
			 ntg_sup_val(T1)) :
			ntg_inf_val(T1));
	return new_im;
      }

    private:

      const original_im_t&	original_im;
      trans_im_t		trans_im;
      const coeffs_t		coeffs;
      unsigned			im_size;
      unsigned			max_level;
      unsigned			current_level;
      ntg::float_d		norm;
      dwt_transform_type	transform_type;
    };

  } // end of namespace transforms

} // end of namespace oln

#endif // ! OLENA_TRANSFORMS_DWT_HH
