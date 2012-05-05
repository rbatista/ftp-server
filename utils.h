#ifndef UTILS_H
#define UTILS_H

#include <errno.h>
#include <string.h>
#include <stdlib.h>

#define fatal() \
        do { fprintf (stderr, "%s: %s: %d: %s: %s\n", \
             program_invocation_short_name, __FILE__, __LINE__,\
		      __PRETTY_FUNCTION__, strerror (errno)); exit(EXIT_FAILURE);} while (0)


#endif /* UTILS_h */
