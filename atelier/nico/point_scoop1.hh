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

#ifndef POINT_SCOOP1_HH_
# define POINT_SCOOP1_HH_

# include <mlc/int.hh>
# include <cassert>
# include <set>
# include <algorithm>
# include "../local/scoop.hh"

namespace oln_point
{

  //macro for scoop1 paradigm
#define stc_simple_typename(Type)     typedef typename vtypes<Exact>::Type Type
#define stc_simple_using(Type, Where) typedef typename vtypes<Where>::Type Type
#define stc_simple_find(Type, Where)  typename vtypes<Where>::Type




  //mandatory for using scoop 2 mecanism
  stc_equip_namespace;

  /////////////////////////////////////////////////////////////
  //all Grid class
  template < class Exact >
  struct Grid
  {
  };

  struct grid1d : public Grid< grid1d >
  {
  };

  struct grid2d : public Grid< grid2d>
  {
  };


  //End Grid

  //////////////////////////////////////////////////////////////
  //all Point class




  template < typename Exact >
  class Point : public any<Exact>
  {
  public:
    stc_simple_typename(grid);
    stc_simple_typename(coord);
    stc_simple_typename(dim);

    enum { n = mlc_value(dim) };

    typedef Point<Exact> self;


    bool operator==(self const& rhs) const  { return this->exact().impl_egal(rhs.exact()); }
    bool operator<(self const& rhs) const   { return this->exact().impl_inf(rhs.exact()); }
    bool operator!=(self const& rhs) const  { return this->exact().impl_diff(rhs.exact()); }
    bool operator>(self const& rhs) const   { return this->exact().impl_sup(rhs.exact()); }
    bool operator>=(self const& rhs) const  { return this->exact().impl_sup_egal(rhs.exact()); }
    bool operator<=(self const& rhs) const  { return this->exact().impl_inf_egal(rhs.exact()); }
    coord operator[](unsigned i) const      { return this->exact().impl_acces(i); }
    coord& operator[](unsigned i)           { return this->exact().impl_acces(i); }
  protected:
    Point() {}
  };

# include "../local/undefs.hh"


  template <typename C>
  class point2d_;


  template<typename C>
  struct vtypes<point2d_< C > >
  {
    typedef stc::is<Point> category;
    typedef mlc::none super_type;
    typedef grid2d grid;
    typedef C coord;
    typedef mlc::uint_<2> dim;
  };

  template < typename C >
  class point2d_ : public Point< point2d_<C> >
  {
  public:
    stc_simple_using(coord, point2d_);

    C col;
    C row;

    typedef point2d_<C> self;
    bool impl_egal(self const& rhs) const      { return row == rhs.row and col == rhs.col; }
    bool impl_inf(self const& rhs)  const      { return row < rhs.row or (row == rhs.row and col < rhs.col); }
    bool impl_diff(self const& rhs) const      { return row != rhs.row or col != rhs.col; }
    bool impl_sup(self const& rhs)  const      { return row > rhs.row or (row == rhs.row and col < rhs.col); }
    bool impl_sup_egal(self const& rhs) const  { return row >= rhs.row or (row == rhs.row and col < rhs.col); }
    bool impl_inf_egal(self const& rhs) const  { return row <= rhs.row or (row == rhs.row and col <= rhs.col); }
    coord operator[](unsigned i) const         {
                                                 if (i == 0) return  row;
                                                 return col;
                                               }
    coord& operator[](unsigned i)               {
                                                 if (i == 0) return  row;
                                                 return col;
                                               }
  };

# include "../local/undefs.hh"





  /////////////////////////////////////////////////////////////
  //Point Test procedure
  template <typename Exact>
  bool bidon(const Point<Exact>& p1, const Point<Exact>& p2)
  {
    return p1 == p2;
  }
  //
  ///////////////////////////////////////////////////////////

  //End point class
  /////////////////////////////////////////////////////////////


  ///////////////////////////////////////////////////////////
  // Point set class

  //Properties
  stc_decl_associated_type(value);
  stc_decl_associated_type(point);
  stc_decl_associated_type(grid);
  stc_decl_associated_type(iter);
  stc_decl_associated_type(box);



  //abstract world

# define current   Point_Set<Exact>
# define super      any<Exact>
# define templ      template <typename Exact>
# define classname  Point_Set

  template <typename Exact>
  struct Point_Set : super
  {
    stc_typename(point);
    stc_typename(grid);
    stc_typename(box);


    unsigned int npoint() const { return this->exact().impl_npoint();  };
    bool includes() const { return this->exact().impl_includes();  };
    box bbox() const { return this->exact().impl_includes(); }
  };

# include "../local/undefs.hh"

  // concrete world

# define current   point_set_base<Exact>
# define super      top <Exact>
# define templ      template <typename Exact>
# define classname  point_set_base

  stc_Header;
  typedef stc::is<Point_Set> category;
  typedef stc::abstract point;
  typedef stc::abstract grid;
  typedef stc::abstract box;
  stc_End;


  template <typename Exact>
  class point_set_base : super
  {
  public:
    stc_using(point);
    stc_using(box);


    // unsigned int npoint() const { return this->exact().impl_npoint();  };
//     bool includes() const { return this->exact().impl_includes();  };
//     box bbox () const  { return this->exact().impl_includes(); }
  protected:
    point_set_base() {}
  };


# include "../local/undefs.hh"
  // End Point class
  //////////////////////////////////////////////////////////

  //////////////////////////////////////////////////////////
  //Iterator

  // abstract world
  template <typename Exact>
  struct Iterator : public any <Exact>
  {
    stc_typename(value);

    void start() { this->exact().impl_start(); }
    void next() { this->exact().impl_next(); }
    void invalidate() { this->exact().impl_invalidate(); }
    bool is_valid() const { return this->exact().impl_is_valid(); }
  };

# include "../local/undefs.hh"

  template <typename Exact>
  struct Iterator_on_Points;


# define current   Iterator_on_Points<Exact>
# define super     Iterator <Exact>
# define templ      template <typename Exact>
# define classname  Iterator_on_Points





  template <typename Exact>
  struct Iterator_on_Points : public super
  {
    stc_typename(point);

    typedef Iterator_on_Points<Exact> self;

    operator stc_type(Exact, point)() const { return this->exact().impl_cast(); }
    stc_type(Exact, point) to_point() const { return this->exact().impl_to_point(); }
    stc_type(Exact, point) const* point_adr() const { return this->exact().impl_point_adr(); }
  };

# include "../local/undefs.hh"


  // Concret world


# define current    simple_iter<C>
# define super      top <current>
# define templ      template <typename C>
# define classname  simple_iter


  stc_Header;
  typedef stc::is<Iterator> category;
  typedef C value;
  typedef oln_point::point2d_<C> point;
  stc_End;


  template < typename C >
  class simple_iter : public super
  {
  public:
    typedef stc_type(simple_iter<C>, point) point;

    simple_iter(const point& init) : i_(0)
    {
      for (int i = 0; i < 15; ++i)
	v_[i] = init;
    }

    void impl_start() { i_ = 0; }
    void impl_next() { ++i_; }
    void impl_invalidate() { i_ = 42; }
    bool impl_is_valid() const { return i_ < 15; }
    point impl_to_point() const { return (point) *this; }
    point const* point_adr() const { return v_; }

    operator point const()
    {
      if (i_ < 16)
	return v_[i_];
      return (v_[0]);
    }

  protected:
    int		  i_;
    point v_[15];
  };

# include "../local/undefs.hh"

  template <typename P>
  class box_;



# define current    box_iterator_<P>
# define super      top  <current>
# define templ      template <typename P>
# define classname  box_iterator_



  stc_Header;
  typedef stc::is<Iterator> category;
  typedef P point;
  typedef P value;
  stc_End;


  template <typename P>
  class box_;


  template < typename P >
  class box_iterator_ : public super
  {
  public:
    typedef stc_type(box_iterator_, point) point;

    box_iterator_(box_<point> &box) : bb_(box)
    {
      nop_ = box.pmax;
      ++nop_[0];
    }

    void impl_start() { p_ = bb_.pmin; }
    void impl_next();
    void impl_invalidate() { p_ = nop_; }
    bool impl_is_valid() const { return p_ != nop_; }
    point impl_to_point() const { return p_; }
    point const* point_adr() const { return &p_; }

  protected:
    point p_;
    point nop_;
    box_<point> &bb_;
  };



  //// methode implementation
  template < typename P >
  void box_iterator_< P >::impl_next()
  {
    typedef typename box_iterator_<P>::point point;
    for (int i = point::n - 1; i >= 0; --i)
      if (p_[i] == bb_.pmax(i))
	p_[i] = bb_.pmin(i);
      else
      {
	++p_[i];
	break;
      }
    if (p_ == bb_.pmin())
      p_ = nop_;
  }

  typedef box_iterator_< point2d_<int> > iter2d;

# include "../local/undefs.hh"


  template <typename P>
  class pset_std;

# define current    pset_std_iterator_<P>
# define super      top  <current>
# define templ      template <typename P>
# define classname  pset_std_iterator_

  stc_Header;
  typedef stc::is<Iterator> category;
  typedef P point;
  typedef P value;
  stc_End;

  template < typename P >
  class pset_std_iterator_ : public super
  {
  public:
    typedef stc_type(pset_std_iterator_, point) point;

    pset_std_iterator_(std::set<P> &box) : bb_(box) {}

    void impl_start() { iter = bb_.begin(); }
    void impl_next() { ++iter;  }
    void impl_invalidate() { iter = bb_.end(); }
    bool impl_is_valid() const { return *iter != bb_.end(); }
    point impl_to_point() const { return *iter; }
    point const* point_adr() const { return iter; }

  protected:
    typename std::set<P> &bb_;
    typename std::set< P >::iterator iter;
  };


# include "../local/undefs.hh"

//End Iterator
//////////////////////////////////////////////////



//////////////////////////////////////////////////
/// set

# define current    pset_std<P>
# define super      point_set_base< current >
# define templ      template <typename P>
# define classname  pset_std

  stc_Header;
  typedef P point;
  typedef stc::final<stc_type(point, grid)> grid;
  typedef current box;
  stc_End;

  template <typename P>
  class pset_std : public super
  {
  public:
    stc_using(box);
    stc_using(point);
    point pmin;
    point pmax;

    enum { n = mlc_value(typename point::dim) };


    pset_std(const P& p1, const P& p2) : pmin(p1), pmax(p2) { set_.insert(pmin); set_.insert(pmax); }
    const pset_std& impl_bbox() const { return *this; }
    bool impl_includes(const point& p) const
    {
      for (unsigned i = 0; i < n; ++i)
	if (p[i] < pmin[i] or p[i] > pmax[i])
	  return false;
      return true;
    }
  protected:
    std::set<point> set_;
  };


# include "../local/undefs.hh"


//// start box


# define current    box_<P>
# define super      point_set_base< current >
# define templ      template <typename P>
# define classname  box_

  stc_Header;
  typedef P point;
 //  typedef stc_deferred(point) point__;
  typedef stc::final<stc_type(point, grid)> grid;
  typedef current box;
  stc_End;

  template <typename P>
  class box_: public super
  {
  public:
    stc_using(box);
    stc_using(point);

    enum { n = mlc_value(typename point::dim) };
    point pmin;
    point pmax;

    box_(const P& p1, const P& p2) : pmin(p1), pmax(p2) {  }
    const box_& impl_bbox() const { return *this; }
    bool impl_includes(const point& p) const
    {
      for (unsigned i = 0; i < n; ++i)
	if (p[i] < pmin[i] or p[i] > pmax[i])
	  return false;
      return true;
    }
  };

# include "../local/undefs.hh"

  typedef box_< point2d_<int> > box2d;

}

  /// end set
  ////////////////////////////////////////////////

#endif /* !POINT_SCOOP1_HH_ */
