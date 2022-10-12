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

AUTOMAKE_OPTIONS 	= foreign

EXTRA_DIST = LicenseInfo.txt PmtC++StyleGuide.doc \
        PmtC++StyleGuide.pdf PmtUserGuide.pdf \
	PmtUserGuide.doc footer.html header.html \
	doxygen.cfg doxygen_html.cfg

doc: html/index.html

## Only produce the html version for now.
html/index.html: ../src/PmtCore/PmtMetadata.h
	doxygen doxygen_html.cfg

##uninstall-local:
##	rm -rf $(pkgdatadir)/html
##	rm -rf $(pkgdatadir)/latex
##	rm -rf $(mandir)/man3/Pmt*
##	rm -rf $(mandir)/man3/Ek*
##	rm -rf $(mandir)/man3/InstanceFilter.3
##	rm -rf $(mandir)/man3/DOM_NodeFilter.3
##	rm -rf $(mandir)/man3/VEkRefCount.3
##	rm -rf $(mandir)/man3/binary_function.3
##	rm -rf $(mandir)/man3/exception.3
##	rm -rf $(phgdatadir)/rtf

##install-data-hook: html/index.html
##	cp -pR html $(pkgdatadir)
##	cp -pR latex $(pkgdatadir)
##	cp -pR man/man3 $(mandir)
##	cp -pR rtf $(phgdatadir)

##dist-hook: html/index.html
##	cp -pR html $(distdir)
##	cp -pR latex $(distdir)
##	cp -pR man $(distdir)
##	cp -pR rtf $(distdir)

clean-local:
	rm -rf html
##	rm -rf latex
##	rm -rf man
##	rm -rf rtf
