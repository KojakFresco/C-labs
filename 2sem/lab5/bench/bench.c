#include "../graph.h"
#include "../dijkstra.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifdef _WIN32
#include <windows.h>
#include <sysinfoapi.h>
#else
#include <unistd.h>
#include <time.h>
#endif

/* High-resolution timer */
static double now_ms(void) {
#ifdef _WIN32
    static LARGE_INTEGER freq = {0};
    LARGE_INTEGER t;
    if (freq.QuadPart == 0) QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&t);
    return (double)t.QuadPart * 1000.0 / (double)freq.QuadPart;
#else
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) == 0) {
        return (double)ts.tv_sec * 1000.0 + (double)ts.tv_nsec / 1e6;
    }
    return (double)clock() * 1000.0 / (double)CLOCKS_PER_SEC;
#endif
}

static void print_system_info(void) {
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    MEMORYSTATUSEX ms;
    ms.dwLength = sizeof(ms);
    GlobalMemoryStatusEx(&ms);
    const char* cpu = getenv("PROCESSOR_IDENTIFIER");
    if (!cpu) cpu = "(unknown)";
    printf("# Platform: Windows\n");
    printf("# CPU: %s\n", cpu);
    printf("# LogicalProcessors: %u\n", si.dwNumberOfProcessors);
    printf("# RAM_bytes: %llu\n", (unsigned long long)ms.ullTotalPhys);
#else
    long proc = sysconf(_SC_NPROCESSORS_ONLN);
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGESIZE);
    printf("# Platform: POSIX\n");
    printf("# LogicalProcessors: %ld\n", proc);
    printf("# RAM_bytes: %lld\n", (long long)pages * (long long)page_size);
#endif
}

static double median_of_copy(const double *a, int n) {
    double *b = malloc(sizeof(double) * n);
    if (!b) return 0.0;
    for (int i = 0; i < n; ++i) b[i] = a[i];
    // simple insertion sort
    for (int i = 1; i < n; ++i) {
        double key = b[i];
        int j = i - 1;
        while (j >= 0 && b[j] > key) { b[j+1] = b[j]; j--; }
        b[j+1] = key;
    }
    double med = (n % 2 == 1) ? b[n/2] : 0.5 * (b[n/2 - 1] + b[n/2]);
    free(b);
    return med;
}

int main(int argc, char **argv) {
    const int sizes[][2] = { {1000, 5000}, {2000, 10000}, {5000, 25000} };
    const int count = sizeof(sizes) / sizeof(sizes[0]);

    int runs = 5;
    int warmups = 1;
    unsigned int seed = 12345;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--runs") == 0 && i + 1 < argc) runs = atoi(argv[++i]);
        else if (strcmp(argv[i], "--warmups") == 0 && i + 1 < argc) warmups = atoi(argv[++i]);
        else if (strcmp(argv[i], "--seed") == 0 && i + 1 < argc) seed = (unsigned)atoi(argv[++i]);
    }

    srand(seed);
    print_system_info();
    printf("n,m,mean_ms,median_ms,stddev_ms,raw_ms\n");

    for (int s = 0; s < count; ++s) {
        int n = sizes[s][0];
        int m = sizes[s][1];

        // warmups
        for (int w = 0; w < warmups; ++w) {
            Graph *g = init_graph();
            if (!g) return 1;
            for (int i = 0; i < m; ++i) {
                int u = rand() % n;
                int v = rand() % n;
                int wt = (rand() % 1000) + 1;
                if (add_edge(g, u, v, wt) != 0) { destroy_graph(g); return 1; }
            }
            long long *d = dijkstra(g, 0);
            if (d) free(d);
            destroy_graph(g);
        }

        double *times = malloc(sizeof(double) * runs);
        if (!times) return 1;
        for (int r = 0; r < runs; ++r) {
            Graph *g = init_graph();
            if (!g) { free(times); return 1; }
            for (int i = 0; i < m; ++i) {
                int u = rand() % n;
                int v = rand() % n;
                int wt = (rand() % 1000) + 1;
                if (add_edge(g, u, v, wt) != 0) { destroy_graph(g); free(times); return 1; }
            }
            double t1 = now_ms();
            long long *d = dijkstra(g, 0);
            double t2 = now_ms();
            if (d) free(d);
            destroy_graph(g);
            times[r] = t2 - t1;
        }

        double sum = 0.0;
        for (int i = 0; i < runs; ++i) sum += times[i];
        double mean = sum / runs;
        double var = 0.0;
        for (int i = 0; i < runs; ++i) var += (times[i] - mean) * (times[i] - mean);
        double stddev = sqrt(var / runs);
        double med = median_of_copy(times, runs);

        printf("%d,%d,%.3f,%.3f,%.3f,\"", n, m, mean, med, stddev);
        for (int i = 0; i < runs; ++i) {
            if (i) printf(";");
            printf("%.3f", times[i]);
        }
        printf("\"\n");

        free(times);
    }

    return 0;
}

