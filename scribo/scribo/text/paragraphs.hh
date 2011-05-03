#include <mln/util/array.hh>
#include <mln/accu/shape/bbox.hh>
#include <mln/core/image/image2d.hh>
#include <mln/core/alias/neighb2d.hh>
#include <mln/draw/box.hh>
#include <mln/data/convert.hh>
#include <mln/value/int_u16.hh>
#include <mln/value/label_16.hh>
#include <mln/value/int_u8.hh>
#include <mln/value/rgb8.hh>
#include <mln/io/ppm/save.hh>
#include <mln/io/pgm/save.hh>
#include <mln/geom/rotate.hh>
#include <mln/literal/colors.hh>

#include <scribo/core/macros.hh>
#include <scribo/core/line_set.hh>
#include <scribo/core/line_info.hh>

using namespace mln;

namespace scribo
{

  namespace internal
  {

//-------------------------------------
// Extracting root of links
//-------------------------------------
      template <typename T>
      inline
      unsigned
      find_root(util::array<T>& parent, unsigned x)
      {
	unsigned tmp_x = x;

	while (parent(tmp_x) != tmp_x)
	  tmp_x = parent(tmp_x);

	while (parent(x) != x)
	{
	  const unsigned tmp = parent(x);
	  x = parent(x);
	  parent(tmp) = tmp_x;
	}

	return x;
      }
  }

  namespace filter
  {

//---------------------------------------------------------------------
// This method aims to cut the links between lines that do not fit the
// different criteria
//---------------------------------------------------------------------

    template <typename L>
    inline
    void paragraph_links(const util::array<value::int_u16>& left,
			 const util::array<value::int_u16>& right,
			 util::array<value::int_u16>& output,
			 const util::array< line_info<L> >& lines,
			 const image2d<bool>& input)
    {
      output = left;

      const unsigned nlines = lines.nelements();

      // image2d<value::rgb8> links = data::convert(value::rgb8(), input);
      // for (unsigned l = 0; l < nlines; ++l)
      // {
      // 	  mln::draw::line(links, lines(l).bbox().pcenter(), lines(left(l)).bbox().pcenter(), literal::red);
      // }
      // mln::io::ppm::save(links, "out_links.ppm");

      // For each line
      for (unsigned l = 0; l < nlines; ++l)
      {
	// Neighbors

	const value::int_u16 left_nbh = output(l);
	const value::int_u16 right_nbh = right(l);
	const value::int_u16 lol_nbh = output(left_nbh);

	// Line features
	const float x_height = lines(l).x_height();
	const float left_x_height = lines(left_nbh).x_height();
	const float right_x_height = lines(right_nbh).x_height();

	const box2d& left_line_bbox = lines(left_nbh).bbox();
	const box2d& current_line_bbox = lines(l).bbox();
	const box2d& right_line_bbox = lines(right_nbh).bbox();
	const box2d& lol_line_bbox = lines(lol_nbh).bbox(); // lol : left neighbor of the left neighbor

	const int lline_col_min = left_line_bbox.pmin().col();
	const int cline_col_min = current_line_bbox.pmin().col();
	const int rline_col_min = right_line_bbox.pmin().col();
	const int lolline_col_min = lol_line_bbox.pmin().col();

	const int lline_col_max = left_line_bbox.pmax().col();
	const int cline_col_max = current_line_bbox.pmax().col();
	const int rline_col_max = right_line_bbox.pmax().col();

	const int lline_cw = lines(left_nbh).char_width();
	const int cline_cw = lines(l).char_width();
	const int rline_cw = lines(right_nbh).char_width();
	// Maximal x variation to consider two lines vertically aligned
	const int delta_alignment = cline_cw;

	// Checks the baseline distances of the two neighbors
	{
	  // Current line baseline
	  const int c_baseline = lines(l).baseline();

	  // Baseline distance with the left and right neighbors
	  const int lc_baseline = lines(left_nbh).baseline() - c_baseline;
	  const int rc_baseline = c_baseline -lines(right_nbh).baseline();

	  // Max baseline distance between the two neighbors
	  // const float delta_baseline_max = std::max(lc_baseline, rc_baseline);
	  // const float delta_baseline_min = std::min(lc_baseline,
	  // rc_baseline);

	  // Only two lines, meaning the current line has only one neighbor
	  bool two_lines = false;

	  // If the current line has no left neighbor
	  if (lc_baseline == 0)
	  {
	    // ror : right neighbor of the right neighbor
	    const value::int_u16 ror_nbh = right(right_nbh);
	    // const box2d& ror_line_bbox = lines(ror_nbh).bbox();

	    // If the current line has a ror
	    if (ror_nbh != right_nbh
		&& output(ror_nbh) == right_nbh)
	    {
	      // Distance between the current line and the right neighbor
	      const float right_distance = lines(l).meanline() - lines(right_nbh).baseline();
	      // Distance between the right neighbor and the ror
	      const float ror_distance = lines(right_nbh).meanline() - lines(ror_nbh).baseline();
	      // ror x_height
	      const float ror_x_height = lines(ror_nbh).x_height();

	      // Conditions to cut the link between the current line
	      // and its right neighbor
	      if (right_distance > 1.4f * ror_distance
		  && std::max(ror_x_height, right_x_height) <
		  1.2f * std::min(ror_x_height, right_x_height)
		  && output(right_nbh) == l)
	      {
		  output(right_nbh) = right_nbh;
		  continue;
	      }
	    }
	    // Otherwise we only have a group of two lines
	    else
	    {
	      // We determine the distance between the two lines
	      const float distance = lines(l).meanline() - lines(right_nbh).baseline();
	      two_lines = true;

	      // If the distance between the two lines is greater than
	      // the minimum x height of the two lines then we cut the
	      // link between them
	      if (distance > 2.0f * std::min(x_height, right_x_height)
		&& output(right_nbh) == l)
	      {
		  output(right_nbh) = right_nbh;
		  continue;
	      }
	    }

	    // Lines features
	    const float min_x_height = std::min(x_height, right_x_height);
	    const float max_x_height = std::max(x_height, right_x_height);
	    const float min_char_width = std::min(rline_cw, cline_cw);
	    const float max_char_width = std::max(rline_cw, cline_cw);

	    // Condition to cut the link between the current line and
	    // its right neighbor
	    if ((max_x_height > min_x_height * 1.2f) &&
		!(max_char_width <= 1.2f * min_char_width))
	    {
	      if (output(right_nbh) == l)
	      {
		output(right_nbh) = right_nbh;
		continue;
	      }
	    }

	    // If we only have two lines we stop the study
	    if (two_lines)
	      continue;
	  }
	  // If the current line has no right neighbor
	  else if (rc_baseline == 0)
	  {
	    // lol : left neighbor of the left neighbor

	    // If the left neighbor of the current line has a left neighbor
	    if (lol_nbh != left_nbh)
	    {
	      // Distance between the current line and its left neighbor
	      const float left_distance = lines(left_nbh).meanline() -
		lines(l).baseline();
	      // Distance between the left neighbor and the left
	      // neighbor of its left neighbor
	      const float lol_distance = lines(lol_nbh).meanline() -
		lines(left_nbh).baseline();
	      // lol x height
	      const float lol_x_height = lines(lol_nbh).x_height();

	      // Conditions to cut the link between the current line
	      // and its left neighbor
	      if (left_distance > 1.4f * lol_distance
		  && std::max(lol_x_height, left_x_height) <
		  1.2f * std::min(lol_x_height, left_x_height))
	      {
		  output(l) = l;
		  continue;
	      }
	    }
	    // Otherwise we only have a group of two lines
	    else
	    {
	      // Distance between the current line and it left neighbor
	      const float distance = lines(left_nbh).meanline() -
		lines(l).baseline();

	      two_lines = true;

	      // If the distance is greater than the min x height
	      // between the two lines
	      if (distance > 2.0f * std::min(x_height, left_x_height))
	      {
		  output(l) = l;
		  continue;
	      }
	    }

	    // Lines features
	    const float min_x_height = std::min(x_height, left_x_height);
	    const float max_x_height = std::max(x_height, left_x_height);
	    const float min_char_width = std::min(lline_cw, cline_cw);
	    const float max_char_width = std::max(lline_cw, cline_cw);

	    // Condition to cut the link between the current line and
	    // its left neighbor
	    if ((max_x_height > min_x_height * 1.2f) &&
		!(max_char_width <= 1.2f * min_char_width))
	    {
	      output(l) = l;
	      continue;
	    }

	    // If we only have two lines we stop the study
	    if (two_lines)
	      continue;
	  }
	  // The current line has at least one left and one right neighbor
	  else // if (delta_baseline_max >= delta_baseline_min)
	  {
	    // Distance between the left and the current line
	    const float left_distance =
	      lines(left_nbh).meanline() - lines(l).baseline();
	    // Distance between the right and the current line
	    const float right_distance =
	      lines(l).meanline() - lines(right_nbh).baseline();

	    // If the left line is too far compared to the right one
	    // we cut the link with it
	    if (left_distance > 1.2f * right_distance
	      && std::max(x_height, left_x_height) > 1.2f * std::min(x_height, left_x_height))
	    {
	      output(l) = l;
	      continue;
	    }
	    // If the right line is too far compared to the left one
	    // we cut the link with it
	    else if (right_distance > 1.2f * left_distance
		     && std::max(x_height, right_x_height) > 1.2f * std::min(x_height, right_x_height)
	    	     && output(right_nbh) == l)
	    {
	      output(right_nbh) = right_nbh;
	      continue;
	    }

	    // If the distance between the baseline of the left
	    // neighbor and the baseline of the current line is
	    // greater than the one between the current line baseline
	    // and the right line baseline we have to study the texte
	    // features of the right and left lines
	    if (lc_baseline > rc_baseline)
	    {
	      const float cw_max = std::max(lline_cw, cline_cw);
	      const float cw_min = std::min(lline_cw, cline_cw);
	      const float min_x_height = std::min(x_height, left_x_height);
	      const float max_x_height = std::max(x_height, left_x_height);

	      if ((max_x_height > min_x_height * 1.2f) &&
		  !(cw_max <= 1.2f * cw_min))
	      {
		output(l) = l;
		continue;
	      }

	      {
		const float min_x_height = std::min(x_height, right_x_height);
		const float max_x_height = std::max(x_height, right_x_height);
		const float cw_max = std::max(rline_cw, cline_cw);
		const float cw_min = std::min(rline_cw, cline_cw);

		if ((max_x_height > min_x_height * 1.2f)
		    && !(cw_max <= 1.2f * cw_min)
		    && output(right_nbh) == l)
		{
		  output(right_nbh) = right_nbh;
		  continue;
		}
	      }
	    }
	    else
	    {
	      const float cw_max = std::max(rline_cw, cline_cw);
	      const float cw_min = std::min(rline_cw, cline_cw);
	      const float min_x_height = std::min(x_height, right_x_height);
	      const float max_x_height = std::max(x_height, right_x_height);

	      if ((max_x_height > min_x_height * 1.2f)
		  && !(cw_max <= 1.2f * cw_min)
		  && output(right_nbh) == l)
	      {
		  output(right_nbh) = right_nbh;
		  continue;
	      }

	      {
		const float min_x_height = std::min(x_height, left_x_height);
		const float max_x_height = std::max(x_height, left_x_height);
		const float cw_max = std::max(lline_cw, cline_cw);
		const float cw_min = std::min(lline_cw, cline_cw);

		if ((max_x_height > min_x_height * 1.2f)
		    && !(cw_max <= 1.2f * cw_min))
		{
		  output(l) = l;
		  continue;
		}
	      }
	    }
	  }
	}

	// If we arrive here, it means than the lines in the
	// neighborhood of the current line are quite similar. We can
	// then begin to study the indentations in order to determine
	// the beginning of new paragraphs

//-----------------------------------------------------------------------------
//   ___________________________
//  |___________________________|
//      ________________________
//     |________________________|
//   ___________________________
//  |___________________________|
//   ___________________________
//  |___________________________|
//
//  Simple case : paragraphs are justified on the left. We try to find any
//  indentation like above.
//
//-----------------------------------------------------------------------------

	{
	  // Check if the current line neighbors are aligned
	  bool left_right_aligned = false;
	  bool left_lol_aligned = false;
	  const int dx_lr = std::abs(lline_col_min - rline_col_min);
	  const int dx_llol = std::abs(lline_col_min - lolline_col_min);

	  if (dx_lr < delta_alignment)
	    left_right_aligned = true;

	  if (dx_llol < delta_alignment)
	    left_lol_aligned = true;

	  if (left_right_aligned && left_lol_aligned)
	  {
	    const int left_right_col_min = std::min(lline_col_min, rline_col_min);
	    const int dx_lrc = std::abs(left_right_col_min - cline_col_min);
	    const float l_char_width = 1.5f * lines(l).char_width();

	    if (dx_lrc > l_char_width &&
		dx_lrc < 3.0f * l_char_width &&
		cline_col_min > rline_col_min &&
		cline_col_min > lline_col_min)
	    {
	      output(right_nbh) = right_nbh;
	      continue;
	    }
	  }
	}

//-----------------------------------------------------------------------------
//   ___________________________
//  |___________________________|
//   ___________________
//  |___________________|           End of the paragraph - Current line
//      ________________________
//     |________________________|   Beginning of a new one
//   ___________________________
//  |___________________________|   Left of left of current line
//
//  End of paragraph case : we try to find an end to the current paragraph
//
//-----------------------------------------------------------------------------

	{
	  // Check if the current line neighbors are aligned
	  bool left_right_max_aligned = false;
	  bool left_current_min_aligned = false;
	  bool lol_current_min_aligned = false;
	  const bool lol_is_left = output(left_nbh) == left_nbh;
	  const int dx_lr_max = std::abs(lline_col_max - rline_col_max);
	  const int dx_lc_min = std::abs(lline_col_min - cline_col_min);
	  const int dx_lolc_min = std::abs(lolline_col_min - cline_col_min);

	  if (dx_lr_max < delta_alignment)
	    left_right_max_aligned = true;

	  if (dx_lc_min < delta_alignment)
	    left_current_min_aligned = true;

	  if (dx_lolc_min < delta_alignment)
	    lol_current_min_aligned = true;

	  if (!left_current_min_aligned && left_right_max_aligned &&
	      (lol_current_min_aligned || lol_is_left))
	  {
	    const int dx_lrc = std::abs(lline_col_max - cline_col_max);
	    const int l_char_width = lines(l).char_width();

	    if (dx_lrc > l_char_width &&
		cline_col_max < lline_col_max &&
		cline_col_min < lline_col_min &&
		(lline_col_min > lolline_col_min || lol_is_left))
	    {
	      output(l) = l;
	      continue;
	    }
	  }
	}


//-----------------------------------------------------------------------------
//   ___________________________
//  |___________________________|
//   ___________________________
//  |___________________________|
//      ________________________
//     |________________________|
//
//  Simple case : paragraphs are justified on the left. We try to find any
//  indentation like above at the end of a column.
//
//-----------------------------------------------------------------------------

	if (left_nbh == l)
	{
	  const value::int_u16 ror_nbh = right(right_nbh);
	  const box2d& ror_line_bbox = lines(ror_nbh).bbox();
	  const int rorline_col_min  = ror_line_bbox.pmin().col();

	  bool right_ror_min_aligned = false;
	  const int dx_rror_min = std::abs(rline_col_min - rorline_col_min);

	  if (dx_rror_min < delta_alignment)
	    right_ror_min_aligned = true;

	  if (right_ror_min_aligned)
	  {
	    const int right_ror_col_min = std::min(rline_col_min, rorline_col_min);
	    const int dx_rrorc = std::abs(right_ror_col_min - cline_col_min);
	    const float l_char_width = 1.5f * lines(l).char_width();

	    if (dx_rrorc > l_char_width &&
		dx_rrorc < 3.0f * l_char_width &&
		cline_col_min > rline_col_min &&
		cline_col_max >= rline_col_max)
	    {
	      output(right_nbh) = right_nbh;
	      continue;
	    }
	  }
	}
      }


      // Only debug

      {
	image2d<value::rgb8> debug = data::convert(value::rgb8(), input);

	for (unsigned i = 0; i < output.nelements(); ++i)
	  output(i) = internal::find_root(output, i);

	mln::util::array<accu::shape::bbox<point2d> > nbbox(output.nelements());
	for (unsigned i = 0; i < nlines; ++i)
	{
	  // if (lines(i).is_textline())
	  // {
	  // mln::draw::box(debug, lines(i).bbox(), literal::red);
	    nbbox(output(i)).take(lines(i).bbox());
	  // }
	}

	for (unsigned i = 0; i < nbbox.nelements(); ++i)
	  if (nbbox(i).is_valid())
	  {
	    box2d b = nbbox(i).to_result();
	    mln::draw::box(debug, b, literal::orange);
	    b.enlarge(1);
	    mln::draw::box(debug, b, literal::orange);
	    b.enlarge(1);
	    mln::draw::box(debug, b, literal::orange);
	  }

	mln::io::ppm::save(debug, "out_paragraph.ppm");
      }

    }
  }

//-------------------------------------------------------------
// Preparation of the lines before linking them.
// For each line we draw the top and the bottom of it.
// Assuming than i is the number of the line. Then the top of the line
// will be affected with the value 2 * i in the block image and the
// bottom with 2 * i + 1.
//
//-------------------------------------------------------------

  template <typename L>
  inline
  void prepare_lines(const box2d& domain,
		     const util::array< line_info<L> >& lines,
		     image2d<value::int_u16>& blocks,
		     util::array<box2d>& rbbox)
  {
    std::map< int, std::vector< const box2d* > > drawn_lines;
    const unsigned nlines = lines.nelements();

    // For each line
    for (unsigned l = 0; l < nlines; ++l)
    {
      // Rotation of the bounding box
      box2d b = geom::rotate(lines(l).bbox(), -90, domain.pcenter());
      rbbox.append(b);

      const unsigned index = l + 1;
      const unsigned even_index = 2 * index;
      const unsigned odd_index = even_index + 1;

      // Top of the line
      {
	bool not_finished = true;
	int col_offset = 0;

	while (not_finished)
	{
	  // Looking for a column in the image to draw the top of the
          // line

	  const int col = b.pmax().col() + col_offset;
	  std::map< int, std::vector< const box2d* > >::iterator it
	    = drawn_lines.find(col);

	  if (it != drawn_lines.end())
	  {
	    const std::vector< const box2d* >& lines = (*it).second;
	    const unsigned nb_lines = lines.size();
	    unsigned i = 0;

	    for (i = 0; i < nb_lines; ++i)
	    {
	      const box2d* box = lines[i];
	      const int min_row = std::max(b.pmin().row(), box->pmin().row());
	      const int max_row = std::min(b.pmax().row(), box->pmax().row());

	      if (min_row - max_row <= 0)
		break;
	    }

	    if (i == nb_lines)
	    {
	      mln::draw::line(blocks, point2d(b.pmin().row(), col),
			      point2d(b.pmax().row(), col), even_index);
	      not_finished = false;
	      drawn_lines[col].push_back(&(rbbox[l]));
	    }
	    else
	      ++col_offset;
	  }
	  else
	  {
	    mln::draw::line(blocks, point2d(b.pmin().row(), col),
			    point2d(b.pmax().row(), col), even_index);
	    not_finished = false;
	    drawn_lines[col].push_back(&(rbbox[l]));
	  }
	}
      }

      // Bottom of the line
      {
	bool not_finished = true;
	int col_offset = 0;

	while (not_finished)
	{
	  // Looking for a column in the image to draw the bottom of
	  // the line

	  const int col = b.pmin().col() - col_offset;
	  std::map< int, std::vector< const box2d* > >::iterator it
	    = drawn_lines.find(col);

	  if (it != drawn_lines.end())
	  {
	    const std::vector< const box2d* >& lines = (*it).second;
	    const unsigned nb_lines = lines.size();
	    unsigned i = 0;

	    for (i = 0; i < nb_lines; ++i)
	    {
	      const box2d* box = lines[i];
	      const int min_row = std::max(b.pmin().row(), box->pmin().row());
	      const int max_row = std::min(b.pmax().row(), box->pmax().row());

	      if (min_row - max_row <= 0)
		break;
	    }

	    if (i == nb_lines)
	    {
	      mln::draw::line(blocks, point2d(b.pmin().row(), col),
			      point2d(b.pmax().row(), col), odd_index);
	      not_finished = false;
	      drawn_lines[col].push_back(&(rbbox[l]));
	    }
	    else
	      ++col_offset;
	  }
	  else
	  {
	    mln::draw::line(blocks, point2d(b.pmin().row(), col),
			    point2d(b.pmax().row(), col), odd_index);
	    not_finished = false;
	    drawn_lines[col].push_back(&(rbbox[l]));
	  }
	}
      }
    }
  }

  template <typename L>
  inline
  void
  process_left_link(image2d<value::int_u16>& blocks,
		    const util::array<box2d>& rbbox,
		    const util::array< line_info<L> >& lines,
		    util::array<value::int_u16>& left)
  {
    typedef value::int_u16 V;

    // At the beginning each line is its own neighbor
    for (unsigned i = 0; i < left.nelements(); ++i)
      left(i) = i;

    const unsigned nlines = lines.nelements();

    // For each line
    for (unsigned i = 0; i < nlines; ++i)
    {
      // Max distance for the line search
      int dmax = 1.5f * lines(i).x_height();

      // Starting points in the current line box
      point2d c = rbbox(i).pcenter();
      point2d q(rbbox(i).pmin().row() + ((c.row() - rbbox(i).pmin().row()) / 4), c.col());

      int
	midcol = (rbbox(i).pmax().col()
		  - rbbox(i).pmin().col()) / 2;

      // Left
      {
	// marge gauche
	int
	  nleftima = c.col() - blocks.domain().pmin().col(),
	  // Distance gauche
	  nleft = std::min(nleftima, midcol + dmax);

	V
	  // Starting points in the box
	  *p = &blocks(c),
	  *p2 = &blocks(q),
	  // End of search
	  *pstop = p - nleft - 1,
	  // Line neighbor
	  *nbh_p = 0;

	// While we haven't found a neighbor or reached the limit
	for (; p != pstop; --p, --p2)
	{
	  if (*p2 != literal::zero  // Not the background
	      && ((*p2 % 2) == 0) // Looking for the bottom of a line
	      && left((*p2 >> 1) - 1) != i) // No loops
	  {
	    // Neightbor found, we stop the research
	    nbh_p = p2;
	    break;
	  }

	  if (*p != literal::zero  // Not the background
	      && ((*p % 2) == 0) // Looking for the bottom of a line
	      && left((*p >> 1) - 1) != i) // No loops
	  {
	    // Neightbor found, we stop the research
	    nbh_p = p;
	    break;
	  }
	}

	// If a neighbor was found, then we have found the top of the
	// line. We are then looking for the bottom of the encountered
	// line. If during the search process we find a complete line
	// included in the touched line, this line is considered as
	// the neighbor under certain conditions (see below)

	//---------------------------------------------------------------
	//   _________________________                        |
	//  |_________________________| => Current line       | Search direction
	//                                                    v
	//  => First encountered top line
	//  __________________________________________________ 2Q
	// |                 Q                                |
	// |   _________________________                      |2P
	// |  |_____________P___________|  => Second top      |2P + 1
	// |                                  line            |
	// |__________________________________________________|2Q + 1
	//
	//
	//---------------------------------------------------------------

	if (nbh_p)
	{
	  std::vector<V> lines_nbh;
	  const V end_p = *nbh_p + 1;
	  const V* nbh_p_copy = nbh_p;

	  for (; *nbh_p != end_p; --nbh_p)
	  {
	    if ((*nbh_p) != literal::zero)  // Not the background
	    {
	      if ((*nbh_p) % 2 == 0)// We have found the top of
		// another line
		lines_nbh.push_back(*nbh_p);
	      else
	      {
		// We have found the bottom of a line. We are looking if
		// we have already encountered the top of this
		// line. If so, we link the current line with this one
		// under certain conditions:

		if (std::find(lines_nbh.begin(), lines_nbh.end(),
			      (*nbh_p) - 1) != lines_nbh.end())
		{
		  // If we can link the complete line with the current line
		  if (// It must be in the search range
		    nbh_p > pstop
		    // Avoid loops
		      && left(((*nbh_p - 1) >> 1) - 1) != i)
		    left(i) = ((*nbh_p - 1) >> 1) - 1;

		  // We have found a complete line so we stop the search
		  break;
		}
	      }
	    }
	  }


	  // If we haven't found any included line in the first
	  // neighbor, then the line is considered as the neighbor of
	  // the current line
	  if (*nbh_p == end_p)
	    left(i) = (*nbh_p_copy >> 1) - 1;
	}
      }
    }
  }


  // We assume that the lines have been rotated
  template <typename L>
  inline
  void
  process_right_link(image2d<value::int_u16>& blocks,
		     const util::array<box2d>& rbbox,
		     const util::array< line_info<L> >& lines,
		     util::array<value::int_u16>& right)
  {
    typedef value::int_u16 V;

    // At the beginning each line is its own neighbor
    for (unsigned i = 0; i < right.nelements(); ++i)
      right(i) = i;

    const unsigned nlines = lines.nelements();

    // For each line
    for (unsigned i = 0; i < nlines; ++i)
    {
      // Max distance for the line search
      int dmax = 1.5f * lines(i).x_height();

      // Starting points in the current line box
      point2d c = rbbox(i).pcenter();
      point2d q(rbbox(i).pmax().row() - ((rbbox(i).pmax().row() - c.row()) / 4), c.col());

      int
	midcol = (rbbox(i).pmax().col()
	  - rbbox(i).pmin().col()) / 2;

      // Right
      {
	int
	  nrightima = geom::ncols(blocks) - c.col() + blocks.domain().pmin().col(),
	  nright = std::min(nrightima, midcol + dmax);

	V
	  // Starting points in the box
	  *p = &blocks(c),
	  *p2 = &blocks(q),
	  // End of search
	  *pstop = p + nright - 1,
	  // Line neighbor
	  *nbh_p = 0;

	// While we haven't found a neighbor or reached the limit
	for (; p != pstop; ++p, ++p2)
	{
	  if (*p2 != literal::zero  // Not the background
	      && ((*p2 % 2) == 1) // Looking for the bottom of a line
	      && right(((*p2 - 1) >> 1) - 1) != i) // No loops
	  {
	    // Neightbor found, we stop the research
	    nbh_p = p2;
	    break;
	  }

	  if (*p != literal::zero  // Not the background
	      && ((*p % 2) == 1) // Looking for the bottom of a line
	      && right(((*p - 1) >> 1) - 1) != i) // No loops
	  {
	    // Neightbor found, we stop the research
	    nbh_p = p;
	    break;
	  }
	}

	// If a neighbor was found, then we have found the bottom of the
	// line. We are then looking for the top of the encountered
	// line. If during the search process we find a complete line
	// included in the touched line, this line is considered as
	// the neighbor under certain conditions (see below)

	//---------------------------------------------------------------
	//
	//
	//  __________________________________________________ 2Q
	// |                 Q                                |
	// |   _________________________                      |2P
	// |  |_____________P___________|  => Second bottom   |2P + 1
	// |                                  line            |
	// |__________________________________________________|2Q + 1
	//  => First encountered bottom line
	//   _________________________                        ^
	//  |_________________________| => Current line       | Search direction
	//                                                    |
	//---------------------------------------------------------------

	if (nbh_p)
	{
	  std::vector<V> lines_nbh;
	  const V end_p = *nbh_p - 1;
	  const V* nbh_p_copy = nbh_p;

	  for (; *nbh_p != end_p; ++nbh_p)
	  {
	    if (*nbh_p != literal::zero)  // Not the background
	    {
	      if (*nbh_p % 2 == 1) // We have found the bottom of
		// another line
		lines_nbh.push_back(*nbh_p);
	      else
	      {
		// We have found the top of a line. We are looking if
		//we have already encountered the bottom of this
		// line. If so, we link the current line with this one
		// under certain conditions:

		if (std::find(lines_nbh.begin(), lines_nbh.end(),
			      *nbh_p + 1) != lines_nbh.end())
		{
		  // If we can link the complete line with the current line
		  if (// It must be in the search range
		    nbh_p < pstop
		    // Avoid loops
		      && right((*nbh_p >> 1) - 1) != i)
		    right(i) = (*nbh_p >> 1) - 1;

		  // We have found a complete line, so we stop the search
		  break;
		}
	      }
	    }
	  }

	  // If we haven't found any included line in the first
	  // neighbor, then the line is considered as the neighbor of
	  // the current line

	  if (*nbh_p == end_p)
	    right(i) = ((*nbh_p_copy - 1) >> 1) - 1;
	}
      }
    }
  }

//-----------------------------------------------------------------------
// Finalizing the links by merging information extracted from the left
// and right links
//-----------------------------------------------------------------------

  template< typename L >
  inline
  void finalize_links(util::array<value::int_u16>& left,
		      util::array<value::int_u16>& right,
		      const util::array< line_info<L> >& lines)
  {
    const unsigned nlines = lines.nelements();

    for (unsigned i = 0; i < nlines; ++i)
    {
      const unsigned left_value = left(i);
      const unsigned right_value = right(i);

      // If the right neighbor of my left neighbor is itself then its
      // right neighbor is me
      {
	value::int_u16& v = right(left_value);

	if (v == left_value)
	  v = i;
      }

      // If the left neighbor of my right neighbor is itself then its
      // left neighbor is me
      {
	value::int_u16& v = left(right_value);

	if (v == right_value)
	  v = i;
      }
    }
  }

  template <typename L>
  inline
  void extract_paragraphs(line_set<L>& lines,
			  const image2d<bool>& input)
  {
    typedef value::int_u16 V;

    image2d<V> blocks(geom::rotate(input.domain(), -90, input.domain().pcenter()));
    data::fill(blocks, 0);

    util::array< line_info<L> > lines_info;

    for_all_lines(l, lines)
    {
      if (lines(l).is_textline())
	lines_info.append(lines(l));
    }

    const unsigned nlines = lines_info.nelements();
    util::array<box2d> rbbox;
    util::array<V> left(nlines);
    util::array<V> right(nlines);
    util::array<V> output;

    rbbox.reserve(nlines);
    output.reserve(nlines);

    std::cout << "Preparing lines" << std::endl;
    prepare_lines(input.domain(), lines_info, blocks, rbbox);
//    io::pgm::save(blocks, "blocks.pgm");
    std::cout << "Linking left" << std::endl;
    process_left_link(blocks, rbbox, lines_info, left);
    std::cout << "Linking right" << std::endl;
    process_right_link(blocks, rbbox, lines_info, right);
    std::cout << "Finalizing links" << std::endl;
    finalize_links(left, right, lines_info);
    // std::cout << "Finalizing merging" << std::endl;
    // finalize_line_merging(left, right, lines);
    std::cout << "Extracting paragraphs" << std::endl;
    filter::paragraph_links(left, right, output, lines_info, input);
  }
}
