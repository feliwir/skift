#pragma once
#include <assert.h>
#include <libio/Format.h>

namespace Test
{
template <class A, class B>
inline void _assert_lower_than(const A a, const B b, const char *file, const char *function, int line)
{
    if constexpr (!(__CONFIG_IS_RELEASE__))
    {
        if (!(a < static_cast<A>(b)))
        {
            assert_failed(IO::format("{} not lower than {}", a, b).cstring(), file, function, line);
        }
    }
}
} // namespace Test

#ifndef assert_lower_than
#    define assert_lower_than(_a, _b) Test::_assert_lower_than(_a, _b, __FILE__, __FUNCTION__, __LINE__)
#endif