# Makefile for job_submit/eco plugin

AUTOMAKE_OPTIONS = foreign

PLUGIN_FLAGS = -module -avoid-version --export-dynamic

AM_CPPFLAGS = -DSLURM_PLUGIN_DEBUG -I$(top_srcdir) -I$(top_srcdir)/src/common

pkglib_LTLIBRARIES = job_submit_eco.la

# Job submit eco plugin.
job_submit_eco_la_SOURCES = job_submit_eco.c
job_submit_eco_la_LDFLAGS = $(PLUGIN_FLAGS)
