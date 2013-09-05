#! /bin/sh

# Copyright (C) 2011, 2013 EPITA Research and Development Laboratory (LRDE).
#
# This file is part of Olena.
#
# Olena is free software: you can redistribute it and/or modify it under
# the terms of the GNU General Public License as published by the Free
# Software Foundation, version 2 of the License.
#
# Olena is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Olena.  If not, see <http://www.gnu.org/licenses/>.


# This file is run by the LRDE autobuilder before the build.
# It is not meant to be distributed with Olena.

set -ex

# Buildbot will tell us the name of the branch being compiled using $1.
branch=$1

# Filter stats for specific branches.
case "$branch" in
  # Consider these branches only.
  master|next|swilena|mesh-segm-skel|stable/scribo|unstable/scribo) ;;
  # Don't compute statistics for other branches.
  *) exit ;;
esac

subdir=$branch
DEST=/lrde/dload/olena/stats/$subdir

# Compute statistics.
gitstats . $DEST

# Anonymize some names.
find "$DEST" -name "*.html" -exec perl -pi -e 's/J.r.me Darbon/JD/g' {} \;

# Expose uploaded files.
chmod -R a+rX $DEST
