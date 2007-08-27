// Copyright (C) 2007 EPITA Research and Development Laboratory
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

#ifndef MLN_ACCU_MEAN_HH
# define MLN_ACCU_MEAN_HH

/*! \file mln/accu/mean.hh
 *
 * \brief Define an accumulator that computes a mean.
 */

# include <mln/accu/count.hh>
# include <mln/accu/sum.hh>


namespace mln
{

  namespace accu
  {


    /*! Generic mean accumulator class.
     *
     * Parameter \c V is the type of values that we sum.  Parameter \c
     * S is the type to store the sum of values; the default type of
     * \c S is the summation type (property) of \c V.  Parameter \c M
     * is the type of the mean value; the default type of \c M is \c
     * S.
     */
    template <typename V,
	      typename S = mln_sum(V),
	      typename M = S>
    struct mean : public Accumulator< mean<V,S,M> >
    {
      typedef V value;

      mean();

      void init();
      void take(const value& v);
      void take(const mean<V,S,M>& other);

      M to_value() const;

    protected:

      accu::count<V> count_;
      accu::sum<V,S>   sum_;
    };



# ifndef MLN_INCLUDE_ONLY

    template <typename V, typename S, typename M>
    mean<V,S,M>::mean()
    {
      init();
    }

    template <typename V, typename S, typename M>
    void
    mean<V,S,M>::init()
    {
      count_.init();
      sum_.init();
    }

    template <typename V, typename S, typename M>
    void mean<V,S,M>::take(const value& v)
    {
      count_.take(v);
      sum_.take(v);
    }

    template <typename V, typename S, typename M>
    void
    mean<V,S,M>::take(const mean<V,S,M>& other)
    {
      count_.take(other.count_);
      sum_.take(other.sum_);
    }

    template <typename V, typename S, typename M>
    M
    mean<V,S,M>::to_value() const
    {
      return sum_.to_value() / count_.to_value();
    }

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace mln::accu

} // end of namespace mln


#endif // ! MLN_ACCU_MEAN_HH
