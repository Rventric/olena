// Copyright 2001, 2002  EPITA Research and Development Laboratory
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

#ifndef OLENA_VALUE_OPS_DEFS_HH
# define OLENA_VALUE_OPS_DEFS_HH

//
//  Macros to factorize global_ops definition
//
//////////////////////////////////////////////

// --------------------------------------------------------------------//

// 
//  Define global assignements for a builtin
//  Internal use only.
//
////////////////////////////////////////////

# define GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, Builtin)		\
template <class T2>						\
inline Builtin& Op(Builtin& lhs, const rec_value<T2>& rhs)	\
{ return optraits<Builtin>::Name(lhs, rhs.self()); }


// 
//  Define global assignements for both rec_value and builtins
//
///////////////////////////////////////////////////////////////


# define GLOBAL_ASSIGN_OP(Op, Name)			\
template <class T1, class T2>				\
inline T1& Op(rec_value<T1>& lhs, const T2& rhs)	\
{ return optraits<T1>::Name(lhs.self(), rhs); }		\
							\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, signed   int);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, unsigned int);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, signed   short);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, unsigned short);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, signed   char);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, unsigned char);	\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, float);		\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, double);		\
GLOBAL_ASSIGN_OP_BUILTIN(Op, Name, bool);

// --------------------------------------------------------------------//

// 
//  Define global arithmetic operators for a builtin
//  Internal use only.
//
/////////////////////////////////////////////////////

#define GLOBAL_ARITH_OP_BUILTIN(Op, Name, Builtin)			\
template <class T2>							\
inline typename								\
internal::deduce_from_traits<internal::operator_##Name##_traits, 	\
                            Builtin, 					\
                            T2>::ret					\
Op(Builtin lhs, const rec_value<T2>& rhs)				\
{									\
  typedef								\
    internal::deduce_from_traits<internal::operator_##Name##_traits,	\
    Builtin, T2> deduced_type;						\
									\
  typedef typename deduced_type::impl impl;				\
  typedef typename deduced_type::lhs_type lhs_type;			\
  typedef typename deduced_type::rhs_type rhs_type;			\
									\
  return optraits<impl>::Name(static_cast<lhs_type>(lhs),		\
	    	              static_cast<rhs_type>(rhs.self()));	\
}

// 
//  Define global arithmetic operators for both rec_values and builtins
//
////////////////////////////////////////////////////////////////////////

#define GLOBAL_ARITH_OP(Op, Name)						\
template <class T1, class T2>							\
inline typename									\
internal::deduce_from_traits<internal::operator_##Name##_traits, T1, T2>::ret	\
Op(const rec_value<T1>& lhs, const T2& rhs)					\
{										\
  typedef									\
    internal::deduce_from_traits<internal::operator_##Name##_traits,		\
    T1, T2> deduced_type;							\
										\
  typedef typename deduced_type::impl impl;					\
  typedef typename deduced_type::lhs_type lhs_type;				\
  typedef typename deduced_type::rhs_type rhs_type;				\
										\
  return optraits<impl>::Name(static_cast<lhs_type>(lhs.self()),		\
			      static_cast<rhs_type>(rhs));			\
}										\
										\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, signed   int);				\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, unsigned int);				\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, signed   short);				\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, unsigned short);				\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, signed   char);				\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, unsigned char);				\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, float);					\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, double);					\
GLOBAL_ARITH_OP_BUILTIN(Op, Name, bool);


// --------------------------------------------------------------------//

// 
//  Define global logical operators for a builtin
//  Internal use only.
//
//////////////////////////////////////////////////

# define GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, Builtin)			\
template <class T2>							\
inline typename								\
internal::deduce_from_traits<internal::operator_logical_traits, 	\
			     Builtin, 					\
			     T2>::ret					\
Op(const Builtin& lhs, const rec_value<T2>& rhs)			\
{									\
  typedef								\
    internal::deduce_from_traits<internal::operator_logical_traits,	\
    Builtin, T2> deduced_type;						\
									\
  typedef typename deduced_type::impl impl;				\
  typedef typename deduced_type::lhs_type lhs_type;			\
  typedef typename deduced_type::rhs_type rhs_type;			\
									\
  return optraits<impl>::Name(static_cast<lhs_type>(lhs),		\
			      static_cast<rhs_type>(rhs.self()));	\
}

// 
//  Define global logical operators for both rec_values and builtins
//
////////////////////////////////////////////////////////////////////

# define GLOBAL_LOGICAL_OP(Op, Name)						\
template <class T1, class T2>							\
inline typename									\
internal::deduce_from_traits<internal::operator_logical_traits, T1, T2>::ret	\
Op(const rec_value<T1>& lhs, const T2& rhs)					\
{										\
  typedef									\
    internal::deduce_from_traits<internal::operator_logical_traits,		\
    T1, T2> deduced_type;							\
										\
  typedef typename deduced_type::impl impl;					\
  typedef typename deduced_type::lhs_type lhs_type;				\
  typedef typename deduced_type::rhs_type rhs_type;				\
										\
  return optraits<impl>::Name(static_cast<lhs_type>(lhs.self()),		\
			      static_cast<rhs_type>(rhs));			\
}										\
										\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, signed   int);				\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, unsigned int);				\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, signed   short);				\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, unsigned short);				\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, signed   char);				\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, unsigned char);				\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, float);					\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, double);					\
GLOBAL_LOGICAL_OP_BUILTIN(Op, Name, bool);


// --------------------------------------------------------------------//

# define GLOBAL_CMP_OP_BUILTIN(Op, Name, Builtin)			\
template <class T2>							\
inline bool Op(const Builtin& lhs, const rec_value<T2>& rhs)		\
{									\
  typedef								\
    internal::deduce_from_traits<internal::operator_cmp_traits,		\
                                 Builtin, T2> deduced_type;		\
									\
  typedef typename deduced_type::impl impl;				\
  typedef typename deduced_type::lhs_type lhs_type;			\
  typedef typename deduced_type::rhs_type rhs_type;			\
									\
  return optraits<impl>::Name(static_cast<lhs_type>(lhs),		\
			      static_cast<rhs_type>(rhs.self()));	\
}


# define GLOBAL_CMP_OP(Op, Name)					\
template <class T1, class T2>						\
inline bool Op(const rec_value<T1>& lhs, const T2& rhs)			\
{									\
  typedef								\
    internal::deduce_from_traits<internal::operator_cmp_traits,		\
                                 T1, T2> deduced_type;			\
									\
  typedef typename deduced_type::impl impl;				\
  typedef typename deduced_type::lhs_type lhs_type;			\
  typedef typename deduced_type::rhs_type rhs_type;			\
									\
 return optraits<impl>::Name(static_cast<lhs_type>(lhs.self()),		\
			      static_cast<rhs_type>(rhs));		\
}									\
									\
GLOBAL_CMP_OP_BUILTIN(Op, Name, signed   int);				\
GLOBAL_CMP_OP_BUILTIN(Op, Name, unsigned int);				\
GLOBAL_CMP_OP_BUILTIN(Op, Name, signed   short);			\
GLOBAL_CMP_OP_BUILTIN(Op, Name, unsigned short);			\
GLOBAL_CMP_OP_BUILTIN(Op, Name, signed   char);				\
GLOBAL_CMP_OP_BUILTIN(Op, Name, unsigned char);				\
GLOBAL_CMP_OP_BUILTIN(Op, Name, float);					\
GLOBAL_CMP_OP_BUILTIN(Op, Name, double);				\
GLOBAL_CMP_OP_BUILTIN(Op, Name, bool);


#endif // ndef OLENA_VALUE_OPS_DEFS_HH
