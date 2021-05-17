#if defined(_POSIX_C_SOURCE) && _POSIX_C_SOURCE < 199309L
#error Expected _POSIX_C_SOURCE >= 199309L
#endif

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L
#endif

#include <time.h>
#include <stdio.h>

/* bench.h uses a simple memory layout using one simple C array:
 *
 * 1st struct timespec: Start of measuring
 * 2nd struct timespec: End of measuring
 * 3rd struct timespec: Difference between 1st and 2nd struct's
 */
/**
 * bench.h uses a simple memory layout, which uses no custom structs, but a
 * custom typedef, which defines an array type. It contains three
 * struct timespec, with several meanings:
 *
 * 1st   start of measurement
 * 2nd   end of measurement
 * 3rd   difference between 1st and 2nd time
 */
typedef struct timespec bench_t[3];

/**
 * Emit lvalue for first struct timespec in bench_t
 */
#define bench_begin(bm) ((bm)[0])
/**
 * Emit lvalue for second struct timespec in bench_t
 */
#define bench_end(bm) ((bm)[1])
/**
 * Emit lvalue for third struct timespec in bench_t
 */
#define bench_diff(bm) ((bm)[2])

/**
 * Convert struct timespec into double
 */
#define bench_diffdbl(bm) ((double)((bm)[2].tv_sec + (bm)[2].tv_nsec / \
		1000000000.0))

/**
 * Calculate difference between start and end and write it to res
 */
static inline void bench_timediff(const struct timespec *restrict start,
        const struct timespec *restrict end, struct timespec *restrict res)
{
	if ((start->tv_sec > end->tv_sec) || (start->tv_sec == end->tv_sec &&
		start->tv_nsec > end->tv_nsec))
	{
		const struct timespec *restrict __swap = start;
		start = end;
		end = __swap;
	}

	res->tv_sec = end->tv_sec - start->tv_sec;
	if (end->tv_nsec < start->tv_nsec)
	{
		--res->tv_sec;
		res->tv_nsec = start->tv_nsec - end->tv_nsec;
	}
	else
		res->tv_nsec = end->tv_nsec - start->tv_nsec;
}

/**
 * Start measurement for bm. bm shall be a bench_t.
 */
#define BENCH_START(bm) do {\
		bench_t *__bench = &(bm);\
		clock_gettime(CLOCK_MONOTONIC, *__bench + 0);\
		{
/**
 * End last measurement and calculate time difference for it
 */
#define BENCH_END() }\
		clock_gettime(CLOCK_MONOTONIC, *__bench + 1);\
		bench_timediff((*__bench) + 0, (*__bench) + 1, (*__bench) + 2);\
	} while (0)

/**
 * Format string for printf-family functions for benchmark results
 */
#define bench_fmt_str "%.10fs [%zu sec %lu nsec]"
/**
 * Format arguments for printf-family functions
 */
#define bench_fmt(bench) bench_diffdbl(bench), (bench)[2].tv_sec, (bench)[2].tv_nsec

/**
 * Report benchmark result to stream using stdio
 */
#define bench_report(stream, bench) ({fprintf((stream), bench_fmt_str "\n",\
		bench_fmt(bench));})
