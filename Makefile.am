## Makefile.am -- Process this file with automake to produce Makefile.in
## Copyright (C) 2000 Gary V. Vaughan
##
## This program is free software; you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 2, or (at your option)
## any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program; if not, write to the Free Software
## Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

EXTRA_DIST		= bootstrap ReleaseNote.txt license.html readme.html \
			  README.config \
			  ./DefaultDefinitions/PmtDefaultDefinitions.xsd \
			  ./test/Common/*


AUTOMAKE_OPTIONS 	= foreign
SUBDIRS 		= doc src test examples

##MAINTAINERCLEANFILES 	= Makefile.in aclocal.m4 configure sic/config-h.in \
##			sic/stamp-h.in $(AUX_DIST)

##ACLOCAL			= aclocal -I $(auxdir)

##dist-hook:
##	(cd $(distdir) && mkdir $(auxdir))
##	for file in $(AUX_DIST) $(AUX_DIST_EXTRA); do \
##	  cp $$file $(distdir)/$$file; \
##	done
