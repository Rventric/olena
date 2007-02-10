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
// the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
// Boston, MA 02110-1301, USA.
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

#ifndef OLENA_CORE_WINITER_HH
# define OLENA_CORE_WINITER_HH

# include <mlc/objs.hh>
# include <oln/core/abstract/iter.hh>

# include <string>

namespace oln {

  template< class Win >
  struct winiter {

    winiter(const typename Win::abstract_type& win) :
      win_(win.exact()), pos_(0) 
    {}

    mlc::begin_type
    operator=(mlc::begin_type)
    {
      pos_ = 0;
      return begin;
    }

    mlc::end_type
    operator=(mlc::end_type)
    {
      pos_ = win_.card();
      return end;
    }

    bool
    operator==(mlc::end_type) const
    {
      return pos_ == win_.card();
    }

    bool
    operator!=(mlc::end_type) const
    {
      return pos_ != win_.card();
    }

    winiter&
    operator++()
    {
      precondition(*this != end);
      ++pos_;
      return *this;
    }

    winiter
    operator++(int)
    {
      precondition(*this != end);
      winiter i = *this;
      ++(*this);
      return i;
    }

    operator typename Win::dpoint_type() const
    {
      precondition(*this != end);
      return win_.dp(pos_);
    }

    // it's convenient to type `it.cur()' instead of `(dpoint)it' when
    // necessary.
    typename Win::dpoint_type 
    cur() const
    {
      return *this;
    }

    static std::string 
    name()
    {
      return std::string("winiter<") + Win::name() + ">";
    }

  private:

    const Win& win_;
    unsigned pos_;
  };

} // oln

#endif // OLENA_CORE_WINITER_HH
