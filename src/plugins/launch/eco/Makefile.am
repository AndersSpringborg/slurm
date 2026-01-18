# Makefile for launch/eco plugin

AUTOMAKE_OPTIONS = foreign

PLUGIN_FLAGS = -module -avoid-version --export-dynamic

AM_CPPFLAGS = -DSLURM_PLUGIN_DEBUG -I$(top_srcdir) -I$(top_srcdir)/src/common

pkglib_LTLIBRARIES = launch_eco.la

launch_eco_la_SOURCES = launch_eco.c

launch_eco_la_LDFLAGS = $(PLUGIN_FLAGS)
