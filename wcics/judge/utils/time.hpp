#include <time.h>

#define bil 1000 * 1000 * 1000

inline void timespec_sub(const struct timespec& a, const struct timespec& b, struct timespec& c) {
	c.tv_nsec = a.tv_nsec - b.tv_nsec;
	c.tv_sec = a.tv_sec - b.tv_sec;
	if(c.tv_nsec < 0) {
		c.tv_nsec += bil;
		c.tv_sec--;
	}
}

inline void timespec_add(const struct timespec& a, const struct timespec& b, struct timespec& c) {
	c.tv_nsec = a.tv_nsec + b.tv_nsec;
	c.tv_sec = a.tv_sec + b.tv_sec;
	if(c.tv_nsec >= bil) {
		c.tv_nsec -= bil;
		c.tv_sec++;
	}
}
