#include <ec_core.h>

#ifdef Q_WS_WIN
#include <windows.h>
#else
#include <sys/time.h>
#endif


namespace ecCore {

// == == == == == == == == == == == == == == == == ==

Clock::Clock() {

    memset((void*) &ti, 0, sizeof(TimeInfo));
    frequency = 0;

#ifdef Q_WS_WIN

    LARGE_INTEGER frequency_parts;

    if(!QueryPerformanceFrequency(&frequency_parts)) // get frequency
        throw Exception("Clock::Clock; Performance Counter unavailable");

    frequency = frequency_parts.QuadPart;

#endif

}

// == == == == == == == == == == == == == == == == ==

Clock::~Clock() {}

// == == == == == == == == == == == == == == == == ==

void Clock::setCounter() {

#ifdef Q_WS_WIN

    LARGE_INTEGER time_now;
    static LARGE_INTEGER time_old;

    if(!QueryPerformanceCounter(&time_now)) // get time
        throw Exception("Clock::SetCounter; Performance Counter unavailable");

    qulonglong time_diff = (((double) (time_old.QuadPart - time_now.QuadPart)) / ((double) frequency));

    ti.secs = time_diff / 1000000;
    ti.msecs = (time_diff % 1000000) / 1000;
    ti.usecs = (time_diff % 1000000) % 1000;

#else

    timeval time_now;
    static timeval time_old;

    gettimeofday(&time_now, NULL); // get time

    ti.secs = time_old.tv_sec - time_now.tv_sec;
    ti.msecs = (time_old.tv_usec - time_now.tv_usec) / 1000;
    ti.usecs = (time_old.tv_usec - time_now.tv_usec) % 1000;

#endif

    time_old = time_now;
}

// == == == == == == == == == == == == == == == == ==

TimeInfo Clock::getCounter() {

    return ti;
}

// == == == == == == == == == == == == == == == == ==

} // ecCore
