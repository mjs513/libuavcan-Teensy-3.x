/*
 * Copyright (C) 2014 Pavel Kirienko <pavel.kirienko@gmail.com>
 */

#ifndef UAVCAN_HELPERS_OSTREAM_HPP_INCLUDED
#define UAVCAN_HELPERS_OSTREAM_HPP_INCLUDED

#include <uavcan/util/templates.hpp>
#include <cstdio>


namespace uavcan
{
/**
 * Compact replacement for ostream for use on embedded systems.
 * Can be used for printing UAVCAN messages to stdout.
 *
 * Relevant discussion: https://groups.google.com/forum/#!topic/px4users/6c1CLNutN90
 *
 * Usage:
 *      OStream::instance() << "Hello world!" << OStream::endl;
 */
class UAVCAN_EXPORT OStream : uavcan::Noncopyable
{
    OStream() { }

public:
    static OStream& instance()
    {
        static OStream s;
        return s;
    }

    static OStream& endl(OStream& stream)
    {
        printf("\n");
        return stream;
    }
};

inline OStream& operator<<(OStream& s, long long x)          { printf("%lld", x);  return s; }
inline OStream& operator<<(OStream& s, unsigned long long x) { printf("%llu", x); return s; }

inline OStream& operator<<(OStream& s, long x)           { printf("%ld", x); return s; }
inline OStream& operator<<(OStream& s, unsigned long x)  { printf("%lu", x); return s; }

inline OStream& operator<<(OStream& s, int x)            { printf("%d", x);  return s; }
inline OStream& operator<<(OStream& s, unsigned int x)   { printf("%u", x);  return s; }

inline OStream& operator<<(OStream& s, short x)          { return operator<<(s, static_cast<int>(x)); }
inline OStream& operator<<(OStream& s, unsigned short x) { return operator<<(s, static_cast<unsigned>(x)); }

inline OStream& operator<<(OStream& s, long double x) { printf("%Lg", x); return s; }
inline OStream& operator<<(OStream& s, double x)      { printf("%g", x);  return s; }
inline OStream& operator<<(OStream& s, float x)       { return operator<<(s, static_cast<double>(x)); }

inline OStream& operator<<(OStream& s, char x)        { printf("%c", x); return s; }
inline OStream& operator<<(OStream& s, const char* x) { printf("%s", x); return s; }

inline OStream& operator<<(OStream& s, OStream&(*manip)(OStream&)) { return manip(s); }

}

#endif // UAVCAN_HELPERS_OSTREAM_HPP_INCLUDED
