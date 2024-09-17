#pragma once


namespace Hypodermic
{

    template <class... T>
    struct MetaPack
    {
        static const int count = static_cast< int >(sizeof...(T));
    };

} // namespace Hypodermic
