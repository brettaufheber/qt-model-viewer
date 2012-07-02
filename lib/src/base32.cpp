#include <ec_base32.h>


namespace ecCore {

// == == == == == == == == == == == == == == == == ==

uchar* Base32::base32_str = (uchar*)
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "234567";

uchar Base32::base32_str_back[] = {
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,  26,  27,  28,  29,  30,  31,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
   15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
    0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0};

// == == == == == == == == == == == == == == == == ==

bool Base32::isValid(const QString& str, bool robust){

    if(!str.length())
        return false;

    uchar mask = (robust) ? 0x1F : 0xFF;

    QString tempstr;
    QRegExp re("^([A-Z]|[2-7])+(\\=\\=\\=\\=|\\=\\=\\=|\\=\\=|\\=)?$");

    for(QString::const_iterator iter = str.begin();iter != str.end();iter++){

        if(((*iter).cell() & mask) == '\r' || ((*iter).cell() & mask) == '\n')
            continue;

        tempstr += (*iter).cell() & mask;
    }

    return tempstr.contains(re);
}

// == == == == == == == == == == == == == == == == ==

QString Base32::encode(const QString& str, bool add_crlf){

    if(!str.length())
        throw Exception("Base32::encode; invalid domain");

    QString retstr;
    QString::const_iterator iter = str.begin();

    for(uint i = 0;i < (uint)str.length();i += 5){

        if(add_crlf && i && i % 76 == 0)
            retstr += "\r\n";

        retstr += base32_str[iter[i].cell() >> 3];

        if(i == (uint)str.length() - 1){

            retstr += base32_str[(iter[i].cell() << 2) & 0x1C] + "====";
            break;
        }

        retstr += base32_str[(iter[i].cell() << 2 | iter[i + 1].cell() >> 6) & 0x1F];
        retstr += base32_str[(iter[i + 1].cell() >> 1) & 0x1F];

        if(i + 1 == (uint)str.length() - 1){

            retstr += base32_str[(iter[i + 1].cell() << 4) & 0x10] + "===";
            break;
        }

        retstr += base32_str[(iter[i + 1].cell() << 4 | iter[i + 2].cell() >> 4) & 0x1F];

        if(i + 2 == (uint)str.length() - 1){

            retstr += base32_str[(iter[i + 2].cell() << 1) & 0x1E] + "==";
            break;
        }

        retstr += base32_str[(iter[i + 2].cell() << 1 | iter[i + 3].cell() >> 7) & 0x1F];
        retstr += base32_str[(iter[i + 3].cell() >> 2) & 0x1F];

        if(i + 3 == (uint)str.length() - 1){

            retstr += base32_str[(iter[i + 3].cell() << 3) & 0x18] + "=";
            break;
        }

        retstr += base32_str[(iter[i + 3].cell() << 3 | iter[i + 4].cell() >> 5) & 0x1F];
        retstr += base32_str[iter[i + 4].cell() & 0x1F];
    }

    return retstr;
}

// == == == == == == == == == == == == == == == == ==

QString Base32::decode(const QString& str, bool robust){

    if(!str.length())
        throw Exception("Base32::decode; invalid domain");

    uchar mask = (robust) ? 0x1F : 0xFF;

    QString retstr;
    QString tempstr = str;

    while(tempstr.length() % 8)
        tempstr += '\0';

    QString::iterator iter = tempstr.begin();

    for(uint i = 0;i < (uint)str.length();i += 8){

        if((iter[i].cell() & mask) == '\r' || (iter[i].cell() & mask) == '\n')
            continue;

        retstr += base32_str_back[iter[i].cell() & mask] << 3 | base32_str_back[iter[i + 1].cell() & mask] >> 2;
        retstr += base32_str_back[iter[i + 1].cell() & mask] << 6 | base32_str_back[iter[i + 2].cell() & mask] << 1 | base32_str_back[iter[i + 3].cell() & mask] >> 4;
        retstr += base32_str_back[iter[i + 3].cell() & mask] << 4 | base32_str_back[iter[i + 4].cell() & mask] >> 1;
        retstr += base32_str_back[iter[i + 4].cell() & mask] << 7 | base32_str_back[iter[i + 5].cell() & mask] << 2 | base32_str_back[iter[i + 6].cell() & mask] >> 3;
        retstr += base32_str_back[iter[i + 6].cell() & mask] << 5 | base32_str_back[iter[i + 7].cell() & mask];
    }

    return retstr;
}

// == == == == == == == == == == == == == == == == ==

} // ecCore
