#include "config.h"

#define _GNU_SOURCE

#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "src/common/slurm_xlator.h"
#include "src/common/slurm_opt.h"
#include "src/common/slurm_resource_info.h"
#include "src/api/pmi_server.h"
#include "src/srun/libsrun/allocate.h"
#include "src/srun/libsrun/fname.h"
#include "src/srun/libsrun/launch.h"
#include "src/srun/libsrun/multi_prog.h"

#include "src/plugins/launch/slurm/task_state.h"

#ifndef OPEN_MPI_PORT_ERROR
/* This exit code indicates the launched Open MPI tasks could
 *	not open the reserved port. It was already open by some
 *	other process. */
#define OPEN_MPI_PORT_ERROR 108
#endif

#define MAX_STEP_RETRIES 4

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
 *      <application>/<method>
 *
 * where <application> is a description of the intended application of
 * the plugin (e.g., "task" for task control) and <method> is a description
 * of how this plugin satisfies that application.  Slurm will only load
 * a task plugin if the plugin_type string has a prefix of "task/".
 *
 * plugin_version - an unsigned 32-bit integer containing the Slurm version
 * (major.minor.micro combined into a single number).
 */
const char plugin_name[]        = "launch Slurm plugin";
const char plugin_type[]        = "launch/slurm";
const uint32_t plugin_version   = SLURM_VERSION_NUMBER;



/*
 * init() is called when the plugin is loaded, before any other functions
 *	are called.  Put global initialization here.
 */
extern int init(void)
{
	verbose("%s loaded", plugin_name);
	return SLURM_SUCCESS;
}