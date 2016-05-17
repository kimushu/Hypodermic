#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Hypodermic/DependencyFactories.h"
#include "Hypodermic/IRegistration.h"
#include "Hypodermic/RegistrationActivator.h"
#include "Hypodermic/TypeInfo.h"


namespace Hypodermic
{

    class Container;


    class Registration : public IRegistration
    {
    public:
        Registration(const TypeInfo& instanceType,
                     const TypeAliases& typeAliases,
                     const InstanceFactory& instanceFactory,
                     const DependencyFactories& dependencyFactories,
                     const std::vector< std::function< void(Container&, const std::shared_ptr< void >&) > >& activationHandlers)
            : m_activator(*this, instanceFactory, activationHandlers)
            , m_instanceType(instanceType)
            , m_typeAliases(typeAliases)
            , m_dependencyFactories(dependencyFactories)
        {
        }

        const TypeInfo& instanceType() const override
        {
            return m_instanceType;
        }

        const TypeAliases& typeAliases() const override
        {
            return m_typeAliases;
        }

        DependencyFactory getDependencyFactory(const TypeInfo& dependencyType) const override
        {
            auto factoryIt = m_dependencyFactories.find(dependencyType);
            if (factoryIt == std::end(m_dependencyFactories))
                return nullptr;

            return factoryIt->second;
        }

        IRegistrationActivator& activator() const override
        {
            return m_activator;
        }

    private:
        mutable RegistrationActivator m_activator;
        TypeInfo m_instanceType;
        TypeAliases m_typeAliases;
        DependencyFactories m_dependencyFactories;
    };

} // namespace Hypodermic