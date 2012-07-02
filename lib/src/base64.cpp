#include <ec_base64.h>


namespace ecCore {

// == == == == == == == == == == == == == == == == ==

uchar* Base64::base64_str = (uchar*)
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

uchar* Base64::base64_url_safe_str = (uchar*)
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789-_";

uchar Base64::base64_str_back[] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,  62,   0,  62,   0,  63,
   52,  53,  54,  55,  56,  57,  58,  59,  60,  61,   0,   0,   0,   0,   0,   0,
    0,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
   15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,   0,   0,   0,   0,  63,
    0,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,
   41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,   0,   0,   0,   0,   0};

// == == == == == == == == == == == == == == == == ==

bool Base64::isValid(const QString& str, bool robust){

    if(!str.length())
        return false;

    uchar mask = (robust) ? 0x3F : 0xFF;

    QString tempstr;
    QRegExp re("^([A-Z]|[a-z]|[0-9]|\\+|\\/|\\-|\\_)+(\\=\\=|\\=)?$");

    for(QString::const_iterator iter = str.begin();iter != str.end();iter++){

        if(((*iter).cell() & mask) == '\r' || ((*iter).cell() & mask) == '\n')
            continue;

        tempstr += (*iter).cell() & mask;
    }

    return tempstr.contains(re);
}

// == == == == == == == == == == == == == == == == ==

QString Base64::encode(const QString& str, bool url_safe, bool add_crlf){

    if(!str.length())
        throw Exception("Base64::encode; invalid domain");

    uchar* encode_str = (url_safe) ? base64_url_safe_str : base64_str;

    QString retstr;
    QString::const_iterator iter = str.begin();

    for(uint i = 0;i < (uint)str.length();i += 3){

        if(add_crlf && i && i % 76 == 0)
            retstr += "\r\n";

        retstr += encode_str[iter[i].cell() >> 2];

        if(i == (uint)str.length() - 1){

            retstr += encode_str[(iter[i].cell() << 4) & 0x30] + "==";
            break;
        }

        retstr += encode_str[(iter[i].cell() << 4 | iter[i + 1].cell() >> 4) & 0x3F];

        if(i + 1 == (uint)str.length() - 1){

            retstr += encode_str[(iter[i + 1].cell() << 2) & 0x3C] + "=";
            break;
        }

        retstr += encode_str[(iter[i + 1].cell() << 2 | iter[i + 2].cell() >> 6) & 0x3F];
        retstr += encode_str[iter[i + 2].cell() & 0x3F];
    }

    return retstr;
}

// == == == == == == == == == == == == == == == == ==

QString Base64::decode(const QString& str, bool robust){

    if(!str.length())
        throw Exception("Base64::decode; invalid domain");

    uchar mask = (robust) ? 0x3F : 0xFF;

    QString retstr;
    QString tempstr = str;

    while(tempstr.length() % 4)
        tempstr += '\0';

    QString::iterator iter = tempstr.begin();

    for(uint i = 0;i < (uint)str.length();i += 4){

        if((iter[i].cell() & mask) == '\r' || (iter[i].cell() & mask) == '\n')
            continue;

        retstr += base64_str_back[iter[i].cell() & mask] << 2 | base64_str_back[iter[i + 1].cell() & mask] >> 4;
        retstr += base64_str_back[iter[i + 1].cell() & mask] << 4 | base64_str_back[iter[i + 2].cell() & mask] >> 2;
        retstr += base64_str_back[iter[i + 2].cell() & mask] << 6 | base64_str_back[iter[i + 3].cell() & mask];
    }

    return retstr;
}

// == == == == == == == == == == == == == == == == ==

} // ecCore
