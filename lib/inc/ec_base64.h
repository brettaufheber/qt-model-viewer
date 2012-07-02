
#ifndef __BASE_64_H__
#define __BASE_64_H__

#include <ec_core.h>


namespace ecCore {

// == == == == == == == == == == == == == == == == ==
/*

Base64: (RFC 3548 / RFC 4648)

    1               2               3
  +---------------+---------------+---------------+
B |7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|7 6 5 4 3 2 1 0|  8 Bit
  +-----------+---+-------+-------+---+-----------+
  |           |           |           |           |
  +-----------+-----------+-----------+-----------+
A |5 4 3 2 1 0|5 4 3 2 1 0|5 4 3 2 1 0|5 4 3 2 1 0|  6 Bit
  +-----------+-----------+-----------+-----------+
    1           2           3           4

  MASK = 00111111B = 0x3F

encode:
 A1 = B1 >> 2
   [ A2 = (B1 << 4) & 00110000B (0x30) ]
 A2 = (B1 << 4 | B2 >> 4) & MASK
   [ A3 = (B2 << 2) & 00111100B (0x3C) ]
 A3 = (B2 << 2 | B3 >> 6) & MASK
 A4 = B3 & MASK

decode:
 B1 = A1 << 2 | A2 >> 4
 B2 = A2 << 4 | A3 >> 2
 B3 = A3 << 6 | A4

*/
// == == == == == == == == == == == == == == == == ==

class Base64 {

private:

//////////////////////////////////
// Data

    static uchar* base64_str;
    static uchar* base64_url_safe_str;
    static uchar base64_str_back[128];

//////////////////////////////////

public:

//////////////////////////////////
// Procedures

    static bool isValid(const QString& str, bool robust);
    static QString encode(const QString& str, bool url_safe, bool add_crlf);
    static QString decode(const QString& str, bool robust);

//////////////////////////////////

};

// == == == == == == == == == == == == == == == == ==

} // ecCore

#endif
