// Copyright (C) 2007 EPITA Research and Development Laboratory.
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

#include "../local/scoop.hh"

namespace oln
{


  //needed for get_imp
  namespace automatic
  {
    //default behaviour identity
    template <template <class> class abstraction, typename Exact>
    struct set_impl< abstraction, behavior::identity, Exact> : impl< abstraction, behavior::identity, Exact >
    {};


    template <typename Exact>
    struct set_impl<Image, behavior::identity, Exact> : public virtual any<Exact>
    {
      stc_typename(point);
      stc_typename(rvalue);
      stc_typename(psite);
      stc_typename(box);

      rvalue impl_rvalue_access(const psite& p) const { return this->exact().image()(p) ;}
      bool impl_owns(const psite& p) const { return this->exact().image().owns_(p); }
      //      bool impl_has_data() const { return this->exact().image().has_data(); }
      box impl_bbox() const { return this->exact().image().bbox(); }
    };

    template <typename Exact>
    struct set_impl<Image2d, behavior::identity, Exact> : public virtual any<Exact>
    {
    };

    template <typename Exact>
    struct set_impl<Signal, behavior::identity, Exact> : public virtual any<Exact>
    {
    };


    template <typename Exact>
    struct set_impl<Mutable_Image, behavior::identity, Exact> : public virtual any<Exact>
    {
      stc_typename(lvalue);
      stc_typename(psite);

      lvalue impl_lvalue_access(const psite& p) { return this->exact().image()(p) ;}
    };

    template <typename Exact>
    struct set_impl<Point_Wise_Accessible_Image, behavior::identity, Exact> : public virtual any<Exact>
    {
      stc_typename(point);

      bool impl_has(const point& pt) { return this->exact().image().has(pt); }
    };
  }
}
