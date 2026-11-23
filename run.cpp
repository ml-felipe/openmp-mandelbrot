#include <cerrno>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <vector>
#define HEADLESS_TEST
#include SOURCE

int main(int argc, char **argv) {
    int limit = 100;
    if (argc > 2) { std::fprintf(stderr,"usage: %s [iterations 1..10000]\n",argv[0]); return 2; }
    if (argc == 2) {
        char *end = nullptr;
        errno = 0;
        long value = std::strtol(argv[1], &end, 10);
        if (errno || end == argv[1] || *end || value < 1 || value > MAXITER) {
            std::fprintf(stderr,"iterations must be an integer from 1 to 10000\n"); return 2;
        }
        limit = static_cast<int>(value);
    }
    std::vector<int> pixels(ROWS*COLS), bins(limit);
    histogram = bins.data(); output2histogram = 1;
    mandel_simple(reinterpret_cast<int (*)[COLS]>(pixels.data()),
                  -2, -2, 2, 2, 4.0/COLS, 4.0/ROWS, limit);
    std::uint64_t sum = 0;
    for (int dwell : pixels) sum += dwell;
    std::printf("%dx%d, iterations=%d, dwell_sum=%llu\n",COLS,ROWS,limit,
                static_cast<unsigned long long>(sum));
}
