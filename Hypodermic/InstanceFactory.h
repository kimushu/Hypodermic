#pragma once

#include <functional>
#include <memory>


namespace Hypodermic
{

    class Container;
    class IRegistration;

    typedef std::function< std::shared_ptr< void >(const IRegistration&, Container&) > InstanceFactory;

} // namespace Hypodermic