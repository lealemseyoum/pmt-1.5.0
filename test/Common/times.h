
// <sstream> may not be present in GCC-2.95.2 compilers.
// You'll need to use GCC >= 2.95.3 for timing support
#include <sstream>

#ifdef _WIN32

#define CPUDEFS LARGE_INTEGER ruse,rfreq;QueryPerformanceFrequency(&rfreq);
#define CPUTIME (QueryPerformanceCounter(&ruse), (double)ruse.QuadPart / (double)rfreq.QuadPart)
#define CHILDCPUTIME 0

#ifdef BUILD_WITH_TIMING_SUPPORT
struct TimeVal
{
    LARGE_INTEGER time;
};

ostream & operator<<(ostream & os, const TimeVal & t)
{
    os << t.time.QuadPart << endl;
    return os;
}

void GetTime(TimeVal & t)
{
    QueryPerformanceCounter(&t.time);
}

void GetFrequency(TimeVal & t)
{
    QueryPerformanceFrequency(&t.time);
}

// returns differencs in seconds
double GetDifference(TimeVal & begin, TimeVal & end, TimeVal & freq)
{
    double t =
        ((end.time.QuadPart -
          begin.time.QuadPart) / (double) freq.time.QuadPart);
    return t;
}
#endif                          /* def BUILD_WITH_TIMING_SUPPORT */

#else                           /* def _WIN32 */

#include <sys/time.h>
#include <sys/resource.h>
extern int getrusage();
#define CPUDEFS struct rusage ruse;
#define CPUTIME (getrusage(RUSAGE_SELF,&ruse), ruse.ru_utime.tv_sec + ruse.ru_stime.tv_sec + 1e-6 * (ruse.ru_utime.tv_usec + ruse.ru_stime.tv_usec))
#define CHILDCPUTIME (getrusage(RUSAGE_CHILDREN,&ruse), ruse.ru_utime.tv_sec + ruse.ru_stime.tv_sec + 1e-6 * (ruse.ru_utime.tv_usec + ruse.ru_stime.tv_usec))

#ifdef BUILD_WITH_TIMING_SUPPORT
struct TimeVal
{
    timeval tv;
};

ostream & operator<<(ostream & os, const TimeVal & t)
{
    os << "(sec=" << t.tv.tv_sec << ",usec=" << t.tv.tv_usec << ")";
    return os;
}

void GetTime(TimeVal & t)
{
    t.tv.tv_sec = t.tv.tv_usec = 0;
    if (gettimeofday(&t.tv, 0) != 0)
        cout << "Error getting time!" << endl;
}

void GetFrequency(TimeVal & t)
{
}

const long MILLION = 1000000;

//inline 
long long _convert(const TimeVal & t)
{
    long long v = t.tv.tv_sec;
    v *= MILLION;
    v += t.tv.tv_usec;
    return v;
}

// returns differencs in seconds
double GetDifference(TimeVal & begin, TimeVal & end, TimeVal & freq)
{
    long long b = _convert(begin);
    long long e = _convert(end);
    return (e - b) / double (MILLION);
}
#endif                          /* def BUILD_WITH_TIMING_SUPPORT */

#endif                          /* def _WIN32 */

