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

#ifndef OLENA_CORE_FWD_ITER2D_HH
# define OLENA_CORE_FWD_ITER2D_HH

# include <oln/core/internal/iter2d.hh>


namespace oln {

  template<class Exact = mlc::final>
  class fwd_iter2d : public internal::_iter2d<typename mlc::exact_vt<fwd_iter2d<Exact>, Exact>::ret>,
                     public fwd_iter<typename mlc::exact_vt<fwd_iter2d<Exact>, Exact>::ret>
  {
  public:

    typedef internal::_iter2d<typename mlc::exact_vt<fwd_iter2d<Exact>, Exact>::ret> super;
    typedef fwd_iter<typename mlc::exact_vt<fwd_iter2d<Exact>, Exact>::ret> super2;

    enum { dim = 2 };
    typedef point2d point;

    template<class Image>
    fwd_iter2d(const Image& ima) :
      super(ima.size()), super2()
    {
    }

    template<class U> U operator=(U u) { return super::operator=(u); }

    void _goto_begin()
    {
      _p.row() = _p.col() = 0;
    }

    void _goto_end()
    {
      _p.row() = _nrows;
    }

    bool _is_at_end() const
    {
      return _p.row() == _nrows;
    }

    void _goto_next()
    {
      ++_p.col();
      if (_p.col() < _ncols)
	return;
      _p.col() = 0;
      ++_p.row();
    }

    static std::string name() { return "fwd_iter2d<" + Exact::name() + ">"; }

  };


} // end of oln


#endif // ! OLENA_CORE_FWD_ITER2D_HH
