// Copyright (C) 2004  EPITA Research and Development Laboratory
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


#ifndef OLENA_OLN_UTILS_BUFFER_HH
# define OLENA_OLN_UTILS_BUFFER_HH

# include <ntg/int.hh>
# include <vector>

namespace oln {
  namespace utils {
    /*!
    ** \brief Buffer used for MD5 data type abstraction.
    */
    class buffer
    {
    public:
      typedef ntg::int_u32		value_type; ///< Used data type.

      /*!
      ** \brief Initialization of data.
      */
      buffer();

      /*!
      ** \brief Add every bits of e in data.
      **
      ** \arg e Element to work on.
      ** \arg count Tell if you want to count e size in the buffer size.
      */
      template <class E>
      void add(const E &e, bool count = true);

      /*!
      ** \brief Push back a bit at the end of the buffer.
      **
      ** true -> push back a 1\n
      ** false -> push back a 0\n
      **
      ** \arg bit Bit to push.
      ** \arg is_padding Are you adding padding bytes ?
      */
      void push_back(bool bit, bool is_padding = false);

      /*!
      ** \brief Return the nth word.
      */
      ntg::int_u32 operator[] (unsigned n) const;

      /*!
      ** \brief Append padding bits.
      */
      void append_padding();

      /*!
      ** \brief Append length.
      */
      void append_length();

      /*!
      ** \brief Return the number of words.
      */
      unsigned size() const;

      /*!
      ** \brief Return the length in bits.
      */
      unsigned bit_len() const;

      /*!
      ** \brief Tell if the buffer has already been padded.
      */
      bool is_append_padding() const;

      /*!
      ** \brief Tell if the length has already been appended.
      */
      bool is_append_length() const;

    protected:
      /*!
      ** \brief Change order of data in a word.
      **
      ** \arg x Data to reorder.
      */
      value_type reorder(value_type x) const;

      static const unsigned		length_ = 100; ///< Capacity chunk.
      /// Number of the current bit in the current word.
      unsigned char			current_bit_;
      std::vector<value_type>	data_; ///< The buffer data.
      value_type			len1_; ///< first word length
      value_type			len2_; ///< second word length
      bool				padding_appended_; ///< Status of padding.
      bool				length_appended_; ///< Status of length appending.
    };

# include <oln/utils/buffer.hxx>

  } // !utils
} // !oln
#endif // !OLENA_OLN_UTILS_BUFFER_HH
