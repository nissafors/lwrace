#include <stddef.h>
#include <sys/time.h>

/* Return current time of day as secs.millisecs */
double getnow() {
  struct timeval now;

  gettimeofday(&now, NULL);
  return now.tv_sec+(now.tv_usec/1000000.0);
}
