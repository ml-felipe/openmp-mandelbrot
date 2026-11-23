#include <algorithm>
#include <cassert>
#include <cstdio>
#include <vector>
#define HEADLESS_TEST
#include SOURCE

// Independent scalar oracle; retain the coursework convention z starts at c.
static int reference(int x, int y, int limit) {
    double cr = -2.0 + x * (4.0 / COLS);
    double ci = -2.0 + (ROWS - 1 - y) * (4.0 / ROWS);
    double r = cr, i = ci;
    int n = 0;
    do {
        double next = r*r - i*i + cr;
        i = 2*r*i + ci;
        r = next;
        ++n;
    } while (n < limit && r*r+i*i < 4.0);
    return n;
}
int main() {
    omp_set_dynamic(0);
    for (int threads : {1, 2, 3, 4, 8, 20}) {
        omp_set_num_threads(threads);
        for (int limit : {1, 17, 100}) {
            std::vector<int> pixels(ROWS*COLS, -1), counts(limit, 0), expected(limit, 0);
            histogram = counts.data();
            output2histogram = 1;
            mandel_simple(reinterpret_cast<int (*)[COLS]>(pixels.data()),
                          -2, -2, 2, 2, 4.0/COLS, 4.0/ROWS, limit);
            for (int y=0; y<ROWS; ++y) for (int x=0; x<COLS; ++x) {
                int want = reference(x,y,limit);
                if (pixels[y*COLS+x] != want) {
                    std::fprintf(stderr,"pixel mismatch (%d,%d), threads=%d\n",x,y,threads);
                    return 1;
                }
                ++expected[want-1];
            }
            if (counts != expected) {
                std::fprintf(stderr,"histogram mismatch, threads=%d\n",threads);
                return 1;
            }
        }
    }
    std::printf("PASS %dx%d: 6 thread counts, 3 iteration limits, pixels and histograms\n",COLS,ROWS);
}
