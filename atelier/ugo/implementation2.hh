#ifndef   	IMPLEMENTATION2_HH_
# define   	IMPLEMENTATION2_HH_

#include	"concept2.hh"
#include	"tracked_ptr.hh"

namespace ugo
{

  //----------------------------------------------------------------------------
  //              ARRAY
  //----------------------------------------------------------------------------

  template <typename value_t, typename coord_t = int>
  struct array2d
  {
      array2d(coord_t imin, coord_t jmin, coord_t imax, coord_t jmax);
      array2d(coord_t ilen, coord_t jlen);

      ~array2d();

      value_t  operator()(coord_t i, coord_t j) const;
      value_t& operator()(coord_t i, coord_t j);

      bool has(coord_t i, coord_t j) const;

      std::size_t memsize() const;

    protected:

      coord_t   imin_, jmin_, imax_, jmax_;
      coord_t   ilen_, jlen_;
      value_t*  buffer_;
      value_t** array_;

    private:

      void allocate_();
      void deallocate_();
  };

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::array2d(coord_t imin, coord_t jmin,
				     coord_t imax, coord_t jmax) :
    imin_(imin),
    jmin_(jmin),
    imax_(imax),
    jmax_(jmax)
  {
    precondition(imax >= imin and jmax >= jmin);
    ilen_ = imax - imin + 1;
    jlen_ = jmax - jmin + 1;
    allocate_();
  }

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::array2d(coord_t ilen, coord_t jlen) :
    imin_(0),
    jmin_(0),
    ilen_(ilen),
    jlen_(jlen)
  {
    precondition(ilen > 0 and jlen > 0);
    imax_ = imin_ + ilen_;
    jmax_ = jmin_ + ilen_;
    allocate_();
  }

  template <typename value_t, typename coord_t>
  array2d<value_t, coord_t>::~array2d()
  {
    deallocate_();
  }

  template <typename value_t, typename coord_t>
  value_t array2d<value_t, coord_t>::operator()(coord_t i, coord_t j) const
  {
    precondition(has(i, j));
    return array_[i][j];
  }

  template <typename value_t, typename coord_t>
  value_t& array2d<value_t, coord_t>::operator()(coord_t i, coord_t j)
  {
    precondition(has(i, j));
    return array_[i][j];
  }

  template <typename value_t, typename coord_t>
  bool array2d<value_t, coord_t>::has(coord_t i, coord_t j) const
  {
    return
      i >= imin_ and i <= imax_ and
      j >= jmin_ and j <= jmax_;
  }

  template <typename value_t, typename coord_t>
  size_t array2d<value_t, coord_t>::memsize() const
  {
    return
      // buffer_
      size_t(ilen_) * size_t(jlen_) * sizeof(value_t)
      +
      // array_
      size_t(ilen_) * sizeof(value_t*);
  }

  template <typename value_t, typename coord_t>
  void array2d<value_t, coord_t>::allocate_()
  {
    buffer_        = new value_t[size_t(ilen_) * size_t(jlen_)];
    array_         = new value_t*[size_t(ilen_)];
    value_t* buf   = buffer_ - jmin_;
    for (coord_t i = 0; i < ilen_; ++i)
    {
      array_[i] = buf;
      buf += jlen_;
    }
    array_ -= imin_;
  }

  template <typename value_t, typename coord_t>
  void array2d<value_t, coord_t>::deallocate_()
  {
    precondition(buffer_ != 0 and array_ != 0);
    delete[] buffer_;
    buffer_ = 0; // safety
    array_ += imin_;
    delete[] array_;
    array_ = 0;  // safety
  }


  //----------------------------------------------------------------------------
  //              POINT
  //----------------------------------------------------------------------------

  //--Grid----------------------

  template <typename Exact> struct Grid { typedef Exact type; };

  struct grid1d : public Grid< grid1d > { };

  struct grid2d : public Grid< grid2d > { };



  //--Point2d_------------------
  //  is a Point
  // mathematical point in 2 dimension

  template <typename T> struct point2d_;

  template <typename T>
  struct vtypes< point2d_<T> >
  {
      typedef point2d_<T>	current;
      typedef top< current >	super_type;

      typedef grid2d		grid;
      typedef T			coord;
      typedef mlc::uint_<2>	dim;

      typedef stc::is<Point>	category;
  };

  template <typename T>
  struct point2d_ : public top< point2d_<T> >
  {
      typedef point2d_<T>	current;
      typedef top< current >	super;

      stc_using( grid  );
      stc_using( coord );
      stc_using( dim   );

      enum { n = mlc_value(dim) };
      point2d_(coord x, coord y) : row(x), col(y) { }

      bool impl_equal(point2d_<T> const& rhs) const  { return row == rhs.row and col == rhs.col;		   }
      bool impl_inf(point2d_<T> const& rhs) const    { return row < rhs.row or (row == rhs.row and col < rhs.col); }
      bool impl_dif(point2d_<T> const& rhs) const    { return not impl_equal(rhs);				   }
      bool impl_sup(point2d_<T> const& rhs) const    { return not impl_inf(rhs) and not impl_equal(rhs);	   }
      bool impl_supeq(point2d_<T> const& rhs) const  { return not impl_inf(rhs);				   }
      bool impl_infeq(point2d_<T> const& rhs) const  { return not impl_sup(rhs);				   }

      coord impl_croch(int i) const
      {
	switch (i)
	{
	  case 0: return row;
	    break;
	  case 1: return col;
	    break;
	  default: abort(); return row;
	}
      }

      coord& impl_croch(int i)
      {
	switch (i)
	{
	  case 0: return row;
	    break;
	  case 1: return col;
	    break;
	  default: abort(); return row;
	}
      }

      point2d_() : row(), col() { }

      coord	row, col;
  };


  typedef point2d_<int> point2d;




  //----------------------------------------------------------------------------
  //              POINT_SET
  //----------------------------------------------------------------------------

  //-point_set_base------------------
  //  is a Point-Set
  // Point-set factorisation.

  template <typename T> struct point_set_base;

  template <typename Exact>
  struct vtypes< point_set_base<Exact> >
  {
      typedef point_set_base<Exact>	current;
      typedef top<Exact>		super_type;

      typedef stc::abstract		point;
      typedef stc_deferred(point) point__;
      typedef stc::final<stc_type(point__, grid)> grid;
      typedef Iterator<grid>		iter;

      typedef stc::is<Point_set>	category;
  };

  template <typename Exact>
  class point_set_base : public top<Exact>
  {
    protected:
      point_set_base() {}
  };



  //--box_------------------------
  //  is a Point-Set.
  // Bounding box

  template <typename P> struct box_;

  template <typename P>
  struct vtypes< box_<P> >
  {
      typedef box_<P>			current;
      typedef point_set_base<current>	super_type;

      typedef P point;
  };

  template <typename P>
  struct box_ : public point_set_base< box_<P> >
  {
      typedef box_<P>			current;
      typedef point_set_base< current >	super;

      stc_using( point );

      box_() : pmin(), pmax() {}
      box_(point p1, point p2) : pmin(p1), pmax(p2) {}

      enum { n = mlc_value(typename P::dim) };
      const box_& impl_bbox() const { return *this; }
      bool impl_includes(const point& p) const
      {
	for (unsigned i = 0; i < n; ++i)
	  if (p[i] < pmin[i] or p[i] > pmax[i])
	    return false;
	return true;
      }

      point pmin, pmax;
  };


  //-----

  typedef box_<point2d>	         box2d;

  //----------------------------------------------------------------------------
  //              ITERATORS
  //----------------------------------------------------------------------------

  //--box_iterator_----------------
  //  is an iterator_on_point
  // iterator on Box

  template <typename P> struct box_iterator_;

  template <typename P>
  struct vtypes< box_iterator_<P> >
  {
      typedef box_iterator_<P>	current;
      typedef top<current>	super_type;

      typedef P			point;
      typedef P			value;

      typedef stc::is<Iterator_on_Points>	category;
  };

  template <typename P>
  // P = type de point
  struct box_iterator_ : public top< box_iterator_<P> >
  {
      typedef box_iterator_<P>	current;
      typedef top<current>	super;

      stc_using( value );
      stc_using( point );

      box_iterator_(const box_<point>& box)
      {
	bbox_ = box;
	p_    = bbox_.pmin;
      }

      void impl_start() { p_ =  bbox_.pmin; }

      // implementation modifee pour que la totalite de l image soit
      // parcourue (pmax inclu).
      void impl_next()
      {
	if (p_ == bbox_.pmax)
	  impl_invalidate();

	for (int i = 0; i < point::n; i++)
	  if (p_[i] == bbox_.pmax[i])
	    p_[i] = bbox_.pmin[i];
	  else
	  {
	    ++p_[i];
	    break;
	  }
	if (p_ == bbox_.pmin)
	  p_ = bbox_.pmax;
      }

      void  impl_invalidate()             {
	for (int i = 0; i < point::n; i++)
	  p_[i] = -1;
      }

      bool  impl_is_valid() const         {
	for (int i = 0; i < point::n; i++)
	  if (p_[i] < 0)
	    return (false);
	return (true);
      }
      point impl_to_point() const  	  { return p_;         }
      point const* impl_point_adr() const { return &p_;	       }
      point impl_cast()	const             { return p_;         }
      point impl_cast()		          { return p_;         }

    protected:
      point		p_;
      box_<point>	bbox_;
  };



  //-----

  typedef box_iterator_<point2d> iter2d;

  //----------------------------------------------------------------------------
  //              IMAGE
  //----------------------------------------------------------------------------

  //--image_base-------------------

# define current    image_base<Exact>
# define super      top<Exact>
# define templ      template <typename Exact>
# define classname  image_base

  stc_Header;

  typedef stc::abstract		data;
  typedef stc::abstract         point;
  typedef stc::abstract         value;
  typedef stc::abstract         iter;

  typedef stc::final< stc::is<Image> > category;

  stc_End;


  template <typename Exact>
  class image_base : public super
  {
      stc_using( point );
      stc_using( value );
      stc_using( box   );
      stc_using( iter  );
      stc_using( grid  );
      stc_using( coord );

      stc_lookup(data);
      typedef data data_t;

      bool has_data() const { return data_ != 0; }
    protected:
      image_base() { }
      oln::internal::tracked_ptr<data_t> data_;
  };

# include "../local/undefs.hh"

  //--image2d-------------------

# define current    image2d_<T>
# define super      top<current>
# define templ      template <typename T>
# define classname  image2d_

  stc_Header;
  typedef iter2d	 iter;
  typedef point2d	 point;
  typedef point::coord	 coord;
  typedef T		 value;
  typedef grid2d	 grid;
  typedef box2d		 box;
  typedef array2d<value> data;

  typedef stc::is<Image> category;

  stc_End;


  template <typename T>
  struct image2d_ : public super
  {
    public:
      stc_using( point );
      stc_using( value );
      stc_using( box   );
      stc_using( iter  );
      stc_using( grid  );
      stc_using( data  );

      image2d_(box &box_init) : bbox(box_init) {
	data_ = new data(bbox.pmin.row, bbox.pmin.col,
			 bbox.pmax.row, bbox.pmax.col);
      }

      bool   impl_owns(const point& p) const        { return bbox.includes(p);        }
      value  impl_point_value(const point& p) const { return (*data_)(p.row, p.col);  }
      value& impl_point_value(const point& p)       { return (*data_)(p.row, p.col);  }
      box    impl_bbox() const		            { return bbox;		     }

      box&	bbox;
    protected:
      data*	data_;
  };

  typedef	image2d_<int>	image2d;

# include "../local/undefs.hh"
}

#endif	    /* !IMPLEMENTATION2_HH_ */
