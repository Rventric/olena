// Copyright (C) 2005 EPITA Research and Development Laboratory
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

#ifndef INTEGRE_ENUM_ENUM_HH
# define INTEGRE_ENUM_ENUM_HH

# include <mlc/any.hh>

# include <ntg/core/cats.hh>
# include <ntg/core/props.hh>

namespace ntg {

  template <typename E> struct enum_value;

  template <typename E>
  struct category_type< enum_value<E> > { typedef cat::enum_value ret; };

  template <>
  struct default_props < cat::enum_value >
  {
    enum { max_val = 0 };
    enum { depth = 1 };
    typedef mlc::no_type io_type;

  protected:
    default_props() {}
  };

  template <typename E>
  struct enum_value : public mlc::any__best_memory<E>
  {
    typedef E exact_type;

    bool operator==(int rhs) const
    {
      return this->exact().impl_eq(rhs);
    }

    template <typename I>
    bool operator==(const enum_value<I>& rhs) const
    {
      return this->exact().impl_eq(rhs.exact());
    }

    template <typename V>
    bool operator!=(const V& rhs) const
    {
      return ! this->operator==(rhs);
    }

    template <typename V>
    exact_type& operator=(const V& rhs)
    {
      return this->exact().impl_assign(rhs);
    }

    template <typename V>
    exact_type& operator+(const V& rhs) const
    {
      return this->exact().impl_add(rhs);
    }

  protected:
    enum_value() {}
  };

} // end of namespace ntg




#endif // ! INTEGRE_ENUM_ENUM_HH
