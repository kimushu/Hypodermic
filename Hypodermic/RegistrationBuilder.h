#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "Hypodermic/DependencyFactories.h"
#include "Hypodermic/InstanceLifetime.h"
#include "Hypodermic/Registration.h"
#include "Hypodermic/PersistentInstanceRegistration.h"
#include "Hypodermic/ProvidedInstanceRegistration.h"
#include "Hypodermic/TypeAliases.h"


namespace Hypodermic
{

    template
    <
        class TRegistrationDescriptorInfo,
        class TInstanceLifetime = typename TRegistrationDescriptorInfo::InstanceLifetime
    >
    class RegistrationBuilder;


    template <class TRegistrationDescriptorInfo>
    class RegistrationBuilder< TRegistrationDescriptorInfo, TransientInstance >
    {
    public:
        static std::shared_ptr< IRegistration > build(const TypeInfo& instanceType,
                                                      const TypeAliases& typeAliases,
                                                      const InstanceFactory& instanceFactory,
                                                      const DependencyFactories& dependencyFactories,
                                                      const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
        {
            return std::make_shared< Registration >(instanceType, typeAliases, instanceFactory, dependencyFactories, activationHandlers);
        }
        
        template <class T>
        static std::shared_ptr< IRegistration > buildForProvidedInstance(const std::shared_ptr< T >& instance, const TypeAliases& typeAliases)
        {
            return std::make_shared< ProvidedInstanceRegistration< T > >(instance, typeAliases);
        }
    };


    template <class TRegistrationDescriptorInfo>
    class RegistrationBuilder< TRegistrationDescriptorInfo, PersistentInstance >
    {
    public:
        static std::shared_ptr< IRegistration > build(const TypeInfo& instanceType,
                                                      const TypeAliases& typeAliases,
                                                      const InstanceFactory& instanceFactory,
                                                      const DependencyFactories& dependencyFactories,
                                                      const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
        {
            return std::make_shared< PersistentInstanceRegistration >
            (
                RegistrationBuilder< TRegistrationDescriptorInfo, TransientInstance >::build
                (
                    instanceType,
                    typeAliases,
                    instanceFactory,
                    dependencyFactories,
                    activationHandlers
                )
            );
        }

        template <class T>
        static std::shared_ptr< IRegistration > buildForProvidedInstance(const std::shared_ptr< T >& instance, const TypeAliases& typeAliases)
        {
            return std::make_shared< PersistentInstanceRegistration >
            (
                RegistrationBuilder< TRegistrationDescriptorInfo, TransientInstance >::template buildForProvidedInstance< T >
                (
                    instance,
                    typeAliases
                )
            );
        }
    };

} // namespace Hypodermic