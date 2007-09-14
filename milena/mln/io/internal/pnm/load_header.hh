// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006, 2007 EPITA
// Research and Development Laboratory
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

#ifndef MLN_IO_PNM_LOAD_HEADER_HH
# define MLN_IO_PNM_LOAD_HEADER_HH

# include <iostream>
# include <fstream>
# include <string>

namespace mln
{

  namespace io
  {

    namespace internal
    {

      namespace pnm
      {


	bool read_header(std::istream& istr,
			     char& type,
			     int& nrows, int& ncols,
			     bool test = false)
	{
	  // check magic
	  if (istr.get() != 'P' )
	    goto err;
	  type = istr.get();

	  if (type < '1' || type > '6')
	    goto err;
	  if (istr.get() != '\n')
	    goto err;

	  // skip comments
	  while (istr.peek() == '#')
	    {
	      std::string line;
	      std::getline(istr, line);
	    }

	  // get size
	  istr >> ncols >> nrows;
	  if (nrows <= 0 || ncols <= 0)
	    goto err;

	  // skip maxvalue
	  if (istr.get() != '\n')
	    goto err;
	  if (type != '1' && type != '4')
	    {
	      std::string line;
	      std::getline(istr, line);
	    }
	  return true;

	err:
	  if (! test)
	    {
	      std::cerr << "error: badly formed header!";
	      abort();
	    }
	  return false;
	}

	void read_header(char ascii, char raw,
			     std::istream& istr,
			     char& type,
			     int& nrows, int& ncols)
	{
	  read_header(istr, type, nrows, ncols);
	  if (! (type == ascii || type == raw))
	    {
	      std::cerr << "error: bad pnm type; "
			<< "expected P" << ascii
			<< " or P" << raw
			<< ", get P" << type << "!";
	      abort();
	    }
	}

      } // end of namespace mln::io::internal::pnm

    } // end of namespace mln::io::internal

  } // end of namespace mln::io

} // end of namespace mln


#endif // ! MLN_IO_PNM_LOAD_HEADER_HH
