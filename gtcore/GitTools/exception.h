#ifndef GT_EXCEPTION_H
#define GT_EXCEPTION_H

#include <string>
#include <exception>
#include <GitTools/string.h>

namespace git
{

    class exception: public std::exception
    {
    private:

        std::string m_message;

    public:

        exception(const char *message): m_message(message)
        {
        }

        template <class string_t>
        exception(const string_t &message): m_message(std_string(message))
        {
        }

        string message() const { return anystr(m_message); }

        const char * what() const noexcept override
        {
            return m_message.c_str();
        }

    };

}

#endif // GT_EXCEPTION_H
