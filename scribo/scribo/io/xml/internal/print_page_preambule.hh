// Copyright (C) 2011 EPITA Research and Development Laboratory (LRDE)
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

#ifndef SCRIBO_IO_XML_INTERNAL_PRINT_PAGE_PREAMBULE_HH
# define SCRIBO_IO_XML_INTERNAL_PRINT_PAGE_PREAMBULE_HH

/// \file
///
/// \brief Print PAGE XML format preambule.

# include <fstream>
# include <mln/core/alias/box2d.hh>
# include <scribo/core/document.hh>
# include <scribo/io/xml/internal/time_info.hh>

namespace scribo
{

  namespace io
  {

    namespace xml
    {

      namespace internal
      {

	using namespace mln;


        /// \brief Print PAGE XML format preambule.
	template <typename L>
	void print_PAGE_preambule(std::ofstream& output,
				  const document<L>& doc,
				  bool with_validation);


# ifndef MLN_INCLUDE_ONLY

	template <typename L>
	void print_PAGE_preambule(std::ofstream& output,
				  const document<L>& doc,
				  bool with_validation)
	{
	  output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << std::endl;

	  if (with_validation)
	    output << "<PcGts xmlns=\"http://schema.primaresearch.org/PAGE/gts/pagecontent/2010-03-19\" "
		   << "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" "
		   << "xsi:schemaLocation=\"http://schema.primaresearch.org/PAGE/gts/pagecontent/2010-03-19 "
		   << "http://schema.primaresearch.org/PAGE/gts/pagecontent/2010-03-19/pagecontent.xsd\">"
		   << std::endl;
	  else
	    output << "<PcGts>" << std::endl;

	  output << "  <Metadata>" << std::endl;
	  output << "    <Creator>LRDE</Creator>" << std::endl;
	  output << "    <Created>" << time_info() << "</Created>" << std::endl;
	  output << "    <LastChange>" << time_info() << "</LastChange>" << std::endl;
	  output << "    <Comments>Generated by Scribo from Olena.</Comments>" << std::endl;
	  output << "  </Metadata>" << std::endl;

	  output << "  <Page imageFilename=\"" << doc.filename()
		 << "\" imageWidth=\"" << doc.width()
		 << "\" imageHeight=\"" << doc.height()
		 << "\">" << std::endl;
	}

# endif // ! MLN_INCLUDE_ONLY

      } // end of namespace scribo::io::xml::internal

    } // end of namespace scribo::io::xml

  } // end of namespace scribo::io

} // end of namespace scribo

#endif // ! SCRIBO_IO_XML_INTERNAL_PRINT_PAGE_PREAMBULE_HH
