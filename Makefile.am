confdir = $(sysconfdir)
conf_DATA = ytalkrc
man_MANS = docs/ytalk.1
EXTRA_DIST = config.rpath mkinstalldirs ChangeLog README INSTALL $(man_MANS) $(conf_DATA)
AM_CFLAGS = -Wall -pedantic
AM_CPPFLAGS = @SYSTEM_DEFS@
SUBDIRS = docs src test
