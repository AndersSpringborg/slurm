/*****************************************************************************\
 *  job_submit_logging.c - Log job submit request specifications.
 *****************************************************************************
 *  Copyright (C) 2010 Lawrence Livermore National Security.
 *  Produced at Lawrence Livermore National Laboratory (cf, DISCLAIMER).
 *  Written by Morris Jette <jette1@llnl.gov>
 *  CODE-OCEC-09-009. All rights reserved.
 *
 *  This file is part of Slurm, a resource management program.
 *  For details, see <https://slurm.schedmd.com/>.
 *  Please also read the included file: DISCLAIMER.
 *
 *  Slurm is free software; you can redistribute it and/or modify it under
 *  the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 2 of the License, or (at your option)
 *  any later version.
 *
 *  In addition, as a special exception, the copyright holders give permission
 *  to link the code of portions of this program with the OpenSSL library under
 *  certain conditions as described in each individual source file, and
 *  distribute linked combinations including the two. You must obey the GNU
 *  General Public License in all respects for all of the code used other than
 *  OpenSSL. If you modify file(s) with this exception, you may extend this
 *  exception to your version of the file(s), but you are not obligated to do
 *  so. If you do not wish to do so, delete this exception statement from your
 *  version.  If you delete this exception statement from all source files in
 *  the program, then also delete it here.
 *
 *  Slurm is distributed in the hope that it will be useful, but WITHOUT ANY
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 *  FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 *  details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with Slurm; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA.
\*****************************************************************************/

#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "slurm/slurm.h"
#include "slurm/slurm_errno.h"

#include "src/common/slurm_xlator.h"
#include "src/slurmctld/slurmctld.h"

/*
 * These variables are required by the generic plugin interface.  If they
 * are not found in the plugin, the plugin loader will ignore it.
 *
 * plugin_name - a string giving a human-readable description of the
 * plugin.  There is no maximum length, but the symbol must refer to
 * a valid string.
 *
 * plugin_type - a string suggesting the type of the plugin or its
 * applicability to a particular form of data or method of data handling.
 * If the low-level plugin API is used, the contents of this string are
 * unimportant and may be anything.  Slurm uses the higher-level plugin
 * interface which requires this string to be of the form
 *
 *	<application>/<method>
 *
 * where <application> is a description of the intended application of
 * the plugin (e.g., "auth" for Slurm authentication) and <method> is a
 * description of how this plugin satisfies that application.  Slurm will
 * only load authentication plugins if the plugin_type string has a prefix
 * of "auth/".
 *
 * plugin_version - an unsigned 32-bit integer containing the Slurm version
 * (major.minor.micro combined into a single number).
 */
const char plugin_name[]       	= "Job submit ECO plugin";
const char plugin_type[]       	= "job_submit/eco";
const uint32_t plugin_version   = SLURM_VERSION_NUMBER;

/*****************************************************************************\
 * We've provided a simple example of the type of things you can do with this
 * plugin. If you develop another plugin that may be of interest to others
 * please post it to slurm-dev@schedmd.com  Thanks!
\*****************************************************************************/

typedef struct job_submit_eco_config {
	uint32_t num_cores;
	uint32_t cpu_freq;
} job_submit_eco_config;

static int load_config(struct job_submit_eco_config *config)
{
	// Open a pipe to the "echo" command
    FILE *pipe = popen("echo '{\"cores\": 10, \"frequency\": 150000}'", "r");
	if (!pipe) {
		error("cannot find chronus in path");
		return SLURM_ERROR;
	}

    // Read the output of the command
    char output[512];
    fgets(output, sizeof(output), pipe);

	// Close the pipe
	pclose(pipe);

	// Parse the output
	char *cores = strstr(output, "cores");
	char *frequency = strstr(output, "frequency");

	// Get the number of cores
	char *cores_value = strtok(cores, ":");
	cores_value = strtok(NULL, ",");
	config->num_cores = atoi(cores_value);

	// Get the frequency
	char *frequency_value = strtok(frequency, ":");
	frequency_value = strtok(NULL, "}");
	config->cpu_freq = atoi(frequency_value);

	return SLURM_SUCCESS;
}

extern int job_submit(job_desc_msg_t *job_desc, uint32_t submit_uid,
		      char **err_msg)
{
	job_submit_eco_config config;
	load_config(&config);

	info("config->num_cores: %d", config.num_cores);
	info("config->cpu_freq: %d", config.cpu_freq);


	uint32_t num_tasks = 15;
	info("Settings n tasks: %d | %d", job_desc->num_tasks, num_tasks);
	job_desc->num_tasks = num_tasks;


	uint32_t cpu_freq = 2200000;
	info("Settings cpu freq: %d | %d", job_desc->cpu_freq_min, cpu_freq);
	job_desc->cpu_freq_min = cpu_freq;
	job_desc->cpu_freq_max = cpu_freq;

	info("job->cpu_freq_min: %d", job_desc->cpu_freq_min);
	info("job->cpu_freq_max: %d", job_desc->cpu_freq_max);
	info("job->cpu_freq_gov: %d", job_desc->cpu_freq_gov);
	info("job->num_tasks: %d", job_desc->num_tasks);
	info("job->cpu_per_task: %d", job_desc->cpus_per_task);
	info("job->min_cpus: %d", job_desc->min_cpus);


	return SLURM_SUCCESS;
}

extern int job_modify(job_desc_msg_t *job_desc, job_record_t *job_ptr,
		      uint32_t submit_uid)
{
	info("Eco modify request: %s", "test");


	return SLURM_SUCCESS;
}
