#pragma once

#include "Hypodermic/DefaultBaseResolver.h"
#include "Hypodermic/EnforceBaseOf.h"
#include "Hypodermic/TypeAliasKey.h"


namespace Hypodermic
{
namespace RegistrationDescriptorOperations
{

    template
    <
        class TDescriptor,
        class TDescriptorInfo
    >
    class AsGroup
    {
    private:
        typedef typename TDescriptorInfo::InstanceType InstanceType;

    public:
        template <template <class...> class TGroup, template <class> class TBaseResolver = DefaultBaseResolver, class TDelayedDescriptor = TDescriptor>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::template RegisterGroup< TGroup, TBaseResolver >::Type
        >
        ::Type& asGroup()
        {
            return asGroupItems< TGroup, TBaseResolver, TDelayedDescriptor >(static_cast<InstanceType*>(nullptr));
        }

    private:
        template <template <class...> class TGroup, template <class> class TBaseResolver, class TDelayedDescriptor, class... TItems>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::template RegisterGroup< TGroup, TBaseResolver >::Type
        >
        ::Type& asGroupItems(TGroup<TItems...>*)
        {
            auto descriptor = static_cast< TDescriptor* >(this);
            addGroupItem< TBaseResolver, TItems... >(descriptor);

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::template RegisterGroup< TGroup, TBaseResolver >::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

        template <template <class> class TBaseResolver>
        void addGroupItem(TDescriptor*)
        {
        }

        template <template <class> class TBaseResolver, class TFirst, class... TOthers>
        void addGroupItem(TDescriptor* descriptor)
        {
            typedef typename TBaseResolver<TFirst>::Type TBase;

            Extensions::EnforceBaseOf< TDescriptorInfo, TBase, InstanceType >::act();

            descriptor->addTypeIfMissing(createKeyForType< TBase >(), [](const std::shared_ptr< void >& x)
            {
                auto instanceDynamicType = std::static_pointer_cast< InstanceType >(x);
                auto instanceStaticType = std::static_pointer_cast< TBase >(instanceDynamicType);
                return instanceStaticType;
            });

            addGroupItem< TBaseResolver, TOthers... >(descriptor);
        }

    protected:
        virtual ~AsGroup() = default;
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic
