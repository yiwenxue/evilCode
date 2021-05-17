#include "bench.h"
#include <unistd.h>

int main() {
  bench_t benchmark;

  BENCH_START(benchmark);
  sleep(1);
  BENCH_END();

  bench_report(stdout, benchmark);
  return 0;
}
