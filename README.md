# OpenMP Mandelbrot decomposition

UPC-FIB Parallel Computing coursework comparing contiguous column blocks,
cyclic rows, and cyclic blocks of 16 columns. Each version computes the same
escape-iteration grid and uses atomic updates for its histogram.

The original files build on the Mandelbrot example credited to Wilkinson and
Allen. This is a coursework adaptation, not an original Mandelbrot renderer.
See [ATTRIBUTION.md](ATTRIBUTION.md).

## Build and run

Requires a C++17 compiler with OpenMP (tested with GCC) and Make.

```sh
make
OMP_NUM_THREADS=4 ./build/block 100
OMP_NUM_THREADS=4 ./build/cyclic 100
OMP_NUM_THREADS=4 ./build/block_cyclic 100
make test
```

The headless runner computes a 512×512 grid over [-2, 2) and prints the sum of
pixel dwell counts. Its optional argument is the iteration limit, from 1 to
10000. The coursework convention starts the recurrence at `z = c`.

## Verification and changes

Tests compare every pixel and every histogram bin with an independent scalar
implementation: three rectangular grid sizes, six thread counts (including
more threads than columns), and three iteration limits per strategy: 162 cases.
The same suite passes AddressSanitizer and UndefinedBehaviorSanitizer
with `ASAN_OPTIONS=detect_leaks=0` (LeakSanitizer cannot run under this
environment's process tracing).

The original block-cyclic loop used thread-dependent block sizes and did not
clip the final block. Tests reproduced histogram errors and heap corruption.
It now uses a uniform 16-column block size and clips the final block. The other
two decomposition strategies are unchanged. The legacy image palette also
reserves space for its `maxiter` entry.

A `HEADLESS_TEST` compile path and a small runner avoid unavailable GUI and
tracing headers. The original GUI, image-writing and huge-page paths remain
in the source, but are excluded from the tested build. Their dependencies
(`fake.h`, `mandelbrot-gui.h`, and the two stb headers) were absent from the
inspected project folders. Those paths have not been built or verified here.

No speedup or scaling results are claimed. Sanitizer success does not establish
race freedom; the tests check output agreement, not exhaustive concurrency behavior.
