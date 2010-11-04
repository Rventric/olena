## tests.mk					-*- Makefile-Automake -*-

## Shared configuration for tests.

## Copyright (C) 2007, 2008, 2009, 2010 EPITA Research and Development
## Laboratory (LRDE)
##
## This file is part of Olena.
##
## Olena is free software: you can redistribute it and/or modify it under
## the terms of the GNU General Public License as published by the Free
## Software Foundation, version 2 of the License.
##
## Olena is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
## General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with Olena.  If not, see <http://www.gnu.org/licenses/>.


# FIXME: Revamp? (See Olena 0.11's test suite).

# Look for Milena header in srcdir and for (generated) test headers
# (like data.hh) in builddir.
AM_CPPFLAGS = -I$(top_srcdir) -I$(top_builddir)

# FIXME: Add
#
#   AM_CXXFLAGS = $(CXXFLAGS_STRICT) $(CXXFLAGS_DEBUG)
#
# when oln.m4 (or just a part of it) is available in the distribution,
# instead of using the hand-made TESTS_CXXFLAGS.
TESTS_CXXFLAGS = @TESTS_CXXFLAGS@
AM_CXXFLAGS = $(TESTS_CXXFLAGS)

TESTS_CXXFLAGS_SPEED = @TESTS_CXXFLAGS_SPEED@
TESTS_CXXFLAGS_DEBUG = @TESTS_CXXFLAGS_DEBUG@


# Flags for optional dependencies.

GDCM_CPPFLAGS = @GDCM_CPPFLAGS@
GDCM_LDFLAGS = @GDCM_LDFLAGS@

CFITSIO_CPPFLAGS = @CFITSIO_CPPFLAGS@
CFITSIO_LDFLAGS = @CFITSIO_LDFLAGS@

MAGICKXX_CPPFLAGS = @MAGICKXX_CPPFLAGS@
MAGICKXX_LDFLAGS = @MAGICKXX_LDFLAGS@

TIFF_CPPFLAGS = @TIFF_CPPFLAGS@
TIFF_LDFLAGS = @TIFF_LDFLAGS@


# Inject rules to compile tests (recursively), without running them.
include $(top_srcdir)/tests/tests-recursive.mk
