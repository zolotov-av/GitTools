#ifndef GT_STRING_H
#define GT_STRING_H

#include <QString>
#include <string>

namespace git
{

    using string = QString;

    inline string anystr(const char *s)
    {
        return s;
    }

    inline string anystr(const std::string &s)
    {
        return QString::fromStdString(s);
    }

    inline string anystr(const QString &s)
    {
        return s;
    }

    inline std::string std_string(const char *s)
    {
        return s;
    }

    inline std::string std_string(const std::string &str)
    {
        return str;
    }

    inline std::string std_string(const QString &str)
    {
        return str.toStdString();
    }

    inline QString qt_string(const char *s)
    {
        return s; // QString::fromStdString(s);
    }

    inline QString qt_string(const std::string &s)
    {
        return QString::fromStdString(s);
    }

    inline QString qt_string(const QString &s)
    {
        return s;
    }

    inline int strpos(const std::string &str, const std::string &needle, int offset = 0)
    {
        auto pos = str.find(needle, offset);
        if ( pos == str.npos ) return -1;
        return pos;
    }

    inline int strpos(const QString &str, const QString &needle, int offset = 0)
    {
        return str.indexOf(needle, offset, Qt::CaseSensitive);
    }

    inline std::string substr(const std::string &str, int start, int length)
    {
        return str.substr(start, length);
    }

    inline QString substr(const QString &str, int start, int length)
    {
        return str.mid(start, length);
    }

}

#endif // GT_STRING_H
