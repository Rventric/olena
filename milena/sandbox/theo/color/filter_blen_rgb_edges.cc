
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/box3d.hh>

#include <mln/core/alias/neighb2d.hh>
#include <mln/make/double_neighb2d.hh>

#include <mln/value/label_8.hh>
#include <mln/value/label_16.hh>

#include <mln/core/image/image_if.hh>
#include <mln/core/routine/extend.hh>
#include <mln/core/routine/duplicate.hh>
#include <mln/data/paste.hh>

#include <mln/io/pgm/load.hh>
#include <mln/io/pgm/save.hh>
#include <mln/io/ppm/load.hh>
#include <mln/io/ppm/save.hh>

#include <mln/debug/colorize.hh>

#include <mln/morpho/dilation.hh>
#include <mln/morpho/erosion.hh>
#include <mln/morpho/watershed/flooding.hh>
#include <mln/labeling/compute.hh>

#include <mln/accu/count.hh>
#include <mln/accu/volume.hh>
#include <mln/accu/mean.hh>

#include "sum_pix.hh"
#include "segment.hh"



namespace mln
{

  point3d color2point(const value::rgb8& c)
  {
    point3d p(c.red(), c.green(), c.blue());
    return p;
  }


  image2d<value::rgb8> blen_image;


  template <typename B>
  void accu_blen_take(B& b, const point2d& e)
  {
    if (e.row() % 2)
      {
	b.take(color2point(blen_image(e + up)));
	b.take(color2point(blen_image(e + down)));
      }
    else
      {
	b.take(color2point(blen_image(e + left)));
	b.take(color2point(blen_image(e + right)));
      }
  }

}

#include "blen_pix.hh"




namespace mln
{

  // Functions.

  inline
  bool is_row_odd(const point2d& p)
  {
    return p.row() % 2;
  }

  inline
  bool is_square(const point2d& p)
  {
    return p.row() % 2 == 0 && p.col() % 2 == 0;
  }

  inline
  bool is_edge(const point2d& p)
  {
    return p.row() % 2 + p.col() % 2 == 1;
  }

  inline
  bool is_point(const point2d& p)
  {
    return p.row() % 2 && p.col() % 2;
  }


  // Neighborhoods.

  typedef neighb< win::multiple<window2d, bool(*)(const point2d&)> > dbl_neighb2d;
  
  const dbl_neighb2d& e2c()
  {
    static bool e2c_h[] = { 0, 1, 0,
			    0, 0, 0,
			    0, 1, 0 };
    static bool e2c_v[] = { 0, 0, 0,
			    1, 0, 1,
			    0, 0, 0 };
    static dbl_neighb2d nbh = make::double_neighb2d(is_row_odd, e2c_h, e2c_v);
    return nbh;
  }

  const dbl_neighb2d& e2e()
  {
    static bool e2e_h[] = { 0, 0, 1, 0, 0,
			    0, 1, 0, 1, 0,
			    0, 0, 0, 0, 0,
			    0, 1, 0, 1, 0,
			    0, 0, 1, 0, 0 };
    static bool e2e_v[] = { 0, 0, 0, 0, 0,
			    0, 1, 0, 1, 0,
			    1, 0, 0, 0, 1,
			    0, 1, 0, 1, 0,
			    0, 0, 0, 0, 0 };
    static dbl_neighb2d nbh = make::double_neighb2d(is_row_odd, e2e_h, e2e_v);
    return nbh;
  }


  // Transforms.

  template <typename T>
  image2d<T>
  image2full(const image2d<T>& input)
  {
    image2d<T> output(2 * input.nrows() - 1,
		      2 * input.ncols() - 1);
    for (int row = 0; row < input.nrows(); ++row)
      for (int col = 0; col < input.ncols(); ++col)
	opt::at(output, 2 * row, 2 * col) = opt::at(input, row, col);
    return output;
  }
  
  template <typename T>
  image2d<T>
  full2image(const image2d<T>& input)
  {
    image2d<T> output((input.nrows() + 1) / 2,
		      (input.ncols() + 1) / 2);
    for (int row = 0; row < input.nrows(); row += 2)
      for (int col = 0; col < input.ncols(); col += 2)
	opt::at(output, row / 2, col / 2) =
	  opt::at(input, row, col);
    return output;
  }


  // Display.

  template <typename I>
  I display_edge(const I& ima, mln_value(I) bg, unsigned zoom)
  {
    unsigned nrows = ima.nrows() / 2 + 1;
    unsigned ncols = ima.ncols() / 2 + 1;
    I output(nrows * (zoom + 1) - 1,
	     ncols * (zoom + 1) - 1);
    data::fill(output, bg);
    mln_VAR( edge, ima | is_edge );
    mln_piter(edge_t) p(edge.domain());
    for_all(p)
      if (p.row() % 2) // horizontal edge
	{
	  unsigned row = (p.row() / 2 + 1) * (zoom + 1) - 1;
	  unsigned col = (p.col() / 2) * (zoom + 1);
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row, col + i) = ima(p);
	}
      else // vertical edge
	{
	  unsigned row = (p.row() / 2) * (zoom + 1);
	  unsigned col = (p.col() / 2 + 1) * (zoom + 1) - 1;
	  for (unsigned i = 0; i < zoom; ++i)
	    opt::at(output, row + i, col) = ima(p);
	}
    return output;
  }



  // Distance, stored on edges, of a couple of colors.

  template <typename I, typename N>
  image2d<value::int_u8>
  dist_on_edges(const I& input, const N& nbh)
  {
    image2d<value::int_u8> output;
    initialize(output, input);
    data::fill(output, 0);

    mln_piter(I) p(input.domain());
    mln_niter(N) n(nbh, p);
    for_all(p)
    {
      n.start();
      value::rgb8 c1 = input(n);
      n.next();
      value::rgb8 c2 = input(n);
      output(p) = dist(c1, c2);
    }

    io::pgm::save(output, "temp_dist.pgm");

    return output;
  }


  value::int_u8 L_to_int_u8(unsigned l)
  {
    return l == 0 ?
      0 : // wshed line
      1 + (l - 1) % 255; // basin
  }


} // mln



void usage(char* argv[])
{
  std::cerr << "usage: " << argv[0] << " input.ppm lambda echo output.ppm" << std::endl;
  std::cerr << "  lambda >= 0" << std::endl;
  std::cerr << "  echo = 0 (mute) or 1 (verbose)" << std::endl;
  abort();
}



int main(int argc, char* argv[])
{
  using namespace mln;
  using value::int_u8;
  using value::rgb8;

  if (argc != 5)
    usage(argv);


  // Color version.
  // --------------


  image2d<rgb8> input;
  io::ppm::load(input, argv[1]);


  int lambda = atoi(argv[2]);
  if (lambda < 0)
    usage(argv);

  bool echo = atoi(argv[3]);


  // Changing input into 'f on edges'.

  image2d<int_u8> f_;
  image2d<rgb8> input_ = image2full(input);
  {
    f_ = dist_on_edges(extend(input_ | is_edge, pw::value(input_)),
		       e2c());
  }
  mln_VAR(f, f_ | is_edge);
  typedef f_t I;


  typedef p_array<mln_psite_(I)> S;
  S s = level::sort_psites_decreasing(f);
  
  typedef morpho::tree::data<I,S> tree_t;
  tree_t t(f, s, e2e());


  blen_image = input_;
  accu::blen_pix<I> a_;
  mln_VAR(a, compute_attribute_on_nodes(a_, t));


  f_t g = do_filter(a, t, e2e(), lambda);


  if (echo)
    debug::println("activity (g != f) = ", (pw::value(g) != pw::value(f)) | f.domain());

  // Watershed transform.

  typedef value::label_16 L;
  L nbasins;
  mln_ch_value_(f_t, L) w = morpho::watershed::flooding(g, e2e(), nbasins);

  std::cout << "n basins = " << nbasins << std::endl;
  
  io::pgm::save(display_edge(w.unmorph_(), 0, 3), "temp_w.pgm");

  if (echo)
    {
      image2d<int_u8> g_(f_.domain());

      data::fill(g_, 0);
      data::paste(g | (pw::value(w) != 0), g_);
      debug::println("g | basins = ", g_ | is_edge);

      data::fill(g_, 0);
      data::paste(g | (pw::value(w) == 0), g_);
      debug::println("g | w line = ", g_ | is_edge);
    }

  image2d<L> w_all = w.unmorph_();

  // edges -> squares
  mln_VAR(w_squares, w_all | is_square);
  data::paste(morpho::dilation(extend(w_squares, pw::value(w_all)),
			       c4().win()),
	      w_all);
  // edges -> points
  mln_VAR(w_points, w_all | is_point);
  data::paste(morpho::erosion(extend(w_points, pw::value(w_all)),
			      c4().win()),
	      w_all);


  io::ppm::save(debug::colorize(rgb8(),
				w_all,
				nbasins),
		"temp_w_all.ppm");
 

  // Outputing.
  // ----------

  image2d<L> out_w = full2image(w_all);

//   io::ppm::save(debug::colorize(rgb8(),
// 				out_w,
// 				nbasins),
// 		argv[4]);
  

  typedef algebra::vec<3,float> vec3f;

  util::array<vec3f> m_3f = labeling::compute(accu::mean<rgb8>(),
					      input,
					      full2image(w_all),
					      nbasins);
  m_3f[0] = vec3f::zero;

  util::array<rgb8> m(m_3f.nelements());
  // convert::from_to(m_3f, m); // FIXME: Do not work!
  for (unsigned i = 0; i < m_3f.nelements(); ++i)
    m[i] = rgb8(m_3f[i][0], m_3f[i][1], m_3f[i][2]); 


  io::ppm::save( level::transform(out_w,
				  convert::to< fun::i2v::array<rgb8> >(m) ),
		 argv[4] );


//   io::ppm::save( level::transform(w_all,
// 				  convert::to< fun::i2v::array<rgb8> >(m) ),
// 		 argv[4] );


//   debug::println("input_", input_);
//   debug::println("w_squares", w_squares);

// FIXME: The code below does not work!

//   util::array<vec3f> m_3f = labeling::compute(accu::mean<rgb8>(),
// 					      input_,
// 					      w_squares,
// 					      nbasins);

}
