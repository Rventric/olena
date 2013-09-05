// Copyright (C) 2009, 2010, 2011, 2013 EPITA Research and Development
// Laboratory (LRDE)
//
// This file is part of Olena.
//
// Olena is free software: you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the Free
// Software Foundation, version 2 of the License.
//
// Olena is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Olena.  If not, see <http://www.gnu.org/licenses/>.
//
// As a special exception, you may use this file as part of a free
// software project without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to produce
// an executable, this file does not by itself cause the resulting
// executable to be covered by the GNU General Public License.  This
// exception does not however invalidate any other reasons why the
// executable file might be covered by the GNU General Public License.

#ifndef SCRIBO_TEXT_RECOGNITION_HH
# define SCRIBO_TEXT_RECOGNITION_HH

/// \file
///
/// \brief Recognize text using an OCR (Tesseract).
///
/// \todo For each text bbox, we create a new image. We may like to avoid that.
/// \todo Do not store the result in an image?

# if !defined HAVE_TESSERACT_2 && !defined HAVE_TESSERACT_3
#  define HAVE_TESSERACT_2
# endif

# include <clocale>

# include <ostream>

# include <mln/core/image/dmorph/image_if.hh>
# include <mln/core/concept/neighborhood.hh>
# include <mln/core/site_set/box.hh>

# include <mln/util/array.hh>
# include <mln/data/fill.hh>
# include <mln/data/paste.hh>
# include <mln/data/paste_without_localization.hh>
# include <mln/pw/all.hh>

# include <mln/core/alias/w_window2d_int.hh>
# include <mln/make/w_window2d_int.hh>

# include <mln/border/resize.hh>

# include <scribo/core/macros.hh>

# include <scribo/text/clean_inplace.hh>

# include <scribo/core/line_set.hh>


# include <tesseract/baseapi.h>

# if defined HAVE_TESSERACT_3
#  include <tesseract/resultiterator.h>
# endif // ! HAVE_TESSERACT_3




namespace scribo
{

  namespace text
  {

    /*! \brief Passes the text bboxes to Tesseract (OCR).

       \param[in] lines       The lines of text.
       \param[in] language    The language which should be recognized by
			      Tesseract. (fra, en, ...)

       \ingroup grpocr
    */
    template <typename L>
    void
    recognition(line_set<L>& lines, const char *language);


    /*! \brief Recognize text from an image.

      \param[in] line        Image of text line.
      \param[in] language    The language which should be recognized by
			     Tesseract. (fra, en, ...)
      \param[in] output_file If set, store the recognized text in
			     this file.

      \ingroup grpocr
    */
    template <typename I>
    void
    recognition(const Image<I>& line,
		const char *language,
		const std::string& output_file = std::string());



# ifndef MLN_INCLUDE_ONLY

#  ifdef HAVE_TESSERACT_2


    template <typename L>
    void
    recognition(line_set<L>& lines, const char *language)
    {
      using namespace mln;

      mln_trace("scribo::text::recognition");

      /* Tesseract is known to have issues while reading training
	 data, depending on the locale in use.  Training data files
	 contain floating-point values and the decimal separator may
	 either be '.' or ',' which may trigger a fatal error with the
	 following message:

           Error: Illegal min or max specification!

	 The recommended solution is to set the locale of LC_NUMERIC
	 to "C".  For more information, see:

           http://code.google.com/p/tesseract-ocr/wiki/FAQ#Error:_Illegal_min_or_max_specification
      */
      setlocale(LC_NUMERIC, "C");

      // Initialize Tesseract.
      TessBaseAPI::InitWithLanguage(NULL, NULL, language, NULL, false, 0, NULL);

      typedef mln_ch_value(L,bool) I;

      /// Use text bboxes with Tesseract.
      for_all_lines(i, lines)
      {
	if (! lines(i).is_textline())
	  continue;

	mln_domain(I) box = lines(i).bbox();

	// Make sure characters are isolated from the borders.
	// Help Tesseract.
	box.enlarge(2);

	I text_ima(box);
	data::fill(text_ima, false);

	// Careful: background is set to 'false'.
	const component_set<L>& comp_set = lines.components();
	const L& lbl = comp_set.labeled_image();

	// Extract each character component to create the line image.
	const mln::util::array<component_id_t>& comps =
	  lines(i).component_ids();
	for_all_elements(e, lines(i).component_ids())
	{
	  unsigned comp_id = comps(e);
	  data::fill(((text_ima | comp_set(comp_id).bbox()).rw()
		      | (pw::value(lbl) == pw::cst(comp_id))).rw(),
		     true);
	}

	/// Improve text quality.
	text::clean_inplace(lines(i), text_ima);

	// Make sure characters are isolated from the borders.
	// Help Tesseract.
	//
	// FIXME: can be improved! We need a morpher for a constant
	// extension set to false (avoid data::fill), a morpher for
	// translating the domain to (0,0) (avoid the creation of a
	// new image), change the default border::thickness to 0 and a
	// morpher to enlarge the domain to a part of the extension.
	mln_domain(I) lbox = text_ima.domain();
	lbox.enlarge(lines(i).char_space() + 2);
	I line_image(lbox, 0); // Make sure there is no border!
	data::fill(line_image, false);
	data::paste_without_localization(text_ima, line_image);

	// Recognize characters.
	char* s = TessBaseAPI::TesseractRect(
	    (unsigned char*) line_image.buffer(),
	    sizeof (bool),			 // pixel size
	    line_image.ncols() * sizeof (bool),  // row offset
	    0,					 // left
	    0,					 // top
	    line_image.ncols(),		         // n cols
	    line_image.nrows());		 // n rows

	if (s != 0)
	{
	  std::string str(s);
	  str = str.substr(0, str.length() - 2);
	  lines(i).update_text(str);
	}

	// The string has been allocated by Tesseract. It must be released.
	delete[] s;
      }

      // Restore the default locale from the environment.
      setlocale(LC_NUMERIC, "");
    }


    template <typename I>
    void
    recognition(const Image<I>& line_,
		const char *language,
		const std::string& output_file)
    {
      using namespace mln;

      mln_trace("scribo::text::recognition");

      const I& line = exact(line_);
      mln_precondition(line.is_valid());

      // See the above explanations about setlocale.
      setlocale(LC_NUMERIC, "C");

      // Initialize Tesseract.
      TessBaseAPI::InitWithLanguage(NULL, NULL, language, NULL, false, 0, NULL);

      std::ofstream file;
      if (!output_file.empty())
	file.open(output_file.c_str());

      mln_domain(I) box = line.domain();
      // Make sure characters are isolated from the borders.
      // Help Tesseract.
      box.enlarge(2);

      I text_ima(box);
      data::fill(text_ima, false);
      data::paste(line, text_ima);

      // Make sure there is no border.
      border::resize(text_ima, 0);

      // Recognize characters.
      char* s = TessBaseAPI::TesseractRect(
	(unsigned char*) text_ima.buffer(),
	sizeof (bool),			  // pixel size
	text_ima.ncols() * sizeof (bool), // row offset
	0,				  // left
	0,				  // top
	text_ima.ncols(),		  // n cols
	text_ima.nrows());		  // n rows

      if (s != 0)
	{
	  if (!output_file.empty())
	    {
	      std::string str(s);
	      str = str.substr(0, str.length() - 1);
	      file << line.domain().bbox().pmin().row()
		   << " "
		   << line.domain().bbox().pmin().col()
		   << " "
		   << line.domain().bbox().pmax().row()
		   << " "
		   << line.domain().bbox().pmax().col()
		   << " "
		   << str;
	    }
	}

      // The string has been allocated by Tesseract. We must free it.
      delete[] s;

      if (!output_file.empty())
	file.close();

      // See the above explanations about setlocale.
      setlocale(LC_NUMERIC, "");
    }


#  else // HAVE_TESSERACT_3


    template <typename L>
    void
    recognition(line_set<L>& lines, const char *language)
    {
      using namespace mln;

      mln_trace("scribo::text::recognition");

      // See the above explanations about setlocale.
      setlocale(LC_NUMERIC, "C");

      // Initialize Tesseract.
      tesseract::TessBaseAPI tess;
      if (tess.Init(NULL, language, tesseract::OEM_DEFAULT) == -1)
      {
	std::cerr << "Error: cannot initialize tesseract!" << std::endl;
	abort();
      }
      tess.SetPageSegMode(tesseract::PSM_SINGLE_LINE);

      typedef mln_ch_value(L,bool) I;

      // Use text bboxes with Tesseract.
      for_all_lines(i, lines)
      {
	if (! lines(i).is_textline())
	  continue;

	mln_domain(I) box = lines(i).bbox();

	// Make sure characters are isolated from the borders.
	// Help Tesseract.
	// FIXME: not needed anymore in tesseract 3 ?
	//
	box.enlarge(2);

	I text_ima(box);
	data::fill(text_ima, false);

	// Careful: background is set to 'false'.
	const component_set<L>& comp_set = lines.components();
	const L& lbl = comp_set.labeled_image();

	// Extract each character component to create the line image.
	const mln::util::array<component_id_t>& comps =
	  lines(i).component_ids();
	for_all_elements(e, lines(i).component_ids())
	{
	  unsigned comp_id = comps(e);
	  data::fill(((text_ima | comp_set(comp_id).bbox()).rw()
		      | (pw::value(lbl) == pw::cst(comp_id))).rw(),
		     true);
	}

	/// Improve text quality.
	text::clean_inplace(lines(i), text_ima);

	// Recognize characters.
	tess.SetImage(
	  (unsigned char*) &text_ima(text_ima.domain().pmin()),
	  text_ima.ncols(),			// n cols
	  text_ima.nrows(),			// n rows
	  sizeof (bool),			// pixel size
	  text_ima.ncols() * sizeof (bool)
	  + 2 * text_ima.border());		// row offset

	char *s = tess.GetUTF8Text();
	if (s != 0)
	{
	  tesseract::ResultIterator *it = tess.GetIterator();
	  std::string str(s);
	  str = str.substr(0, str.length() - 2);
	  lines(i).update_text(str, it->Confidence(tesseract::RIL_TEXTLINE));
	}

	delete[] s;
      }

      // Restore the default locale from the environment.
      setlocale(LC_NUMERIC, "");
    }


    template <typename I>
    void
    recognition(const Image<I>& line_,
		const char *language,
		const std::string& output_file)
    {
      using namespace mln;

      mln_trace("scribo::text::recognition");

      const I& line = exact(line_);
      mln_precondition(line.is_valid());

      // Restore the default locale from the environment.
      setlocale(LC_NUMERIC, "C");

      // Initialize Tesseract.
      tesseract::TessBaseAPI tess;
      if (tess.Init(NULL, language, tesseract::OEM_DEFAULT) == -1)
      {
	std::cerr << "Error: cannot initialize tesseract!" << std::endl;
	abort();
      }

      std::ofstream file;
      if (!output_file.empty())
	file.open(output_file.c_str());

      // Recognize characters.
      char* s = tess.TesseractRect(
	(unsigned char*) &line(line.domain().pmin()),
	sizeof (bool),					  // pixel size
	line.ncols() * sizeof (bool) + line.border() * 2, // row offset
	0,						  // left
	0,						  // top
	line.ncols(),					  // n cols
	line.nrows());					  // n rows

      if (s != 0)
	{
	  if (!output_file.empty())
	    {
	      std::string str(s);
	      str = str.substr(0, str.length() - 1);
	      file << line.domain().bbox().pmin().row()
		   << " "
		   << line.domain().bbox().pmin().col()
		   << " "
		   << line.domain().bbox().pmax().row()
		   << " "
		   << line.domain().bbox().pmax().col()
		   << " "
		   << str;
	    }
	}

      // The string has been allocated by Tesseract. We must free it.
      delete[] s;

      if (!output_file.empty())
	file.close();

      // Restore the default locale from the environment.
      setlocale(LC_NUMERIC, "");
    }


#  endif // ! HAVE_TESSERACT_2

# endif // ! MLN_INCLUDE_ONLY

  } // end of namespace scribo::text

} // end of namespace scribo

#endif // ! SCRIBO_TEXT_RECOGNITION_HH
