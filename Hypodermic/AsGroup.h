#pragma once

#include "Hypodermic/DefaultBaseResolver.h"
#include "Hypodermic/EnforceBaseOf.h"
#include "Hypodermic/MetaPack.h"
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
            typedef typename TDescriptorInfo::template RegisterGroup< TGroup, TBaseResolver >::PackType PackType;
            return asGroupItems< TGroup, TBaseResolver, TDelayedDescriptor >(PackType{});
        }

    private:
        template <template <class...> class TGroup, template <class> class TBaseResolver, class TDelayedDescriptor, class... TBases>
        typename TDelayedDescriptor::template UpdateDescriptor
        <
            typename TDescriptorInfo::template RegisterGroup< TGroup, TBaseResolver >::Type
        >
        ::Type& asGroupItems(MetaPack< TBases... >)
        {
            auto descriptor = static_cast< TDescriptor* >(this);
            addGroupItem< TBases... >(descriptor);

            auto updatedDescriptor = descriptor->template createUpdate< typename TDescriptorInfo::template RegisterGroup< TGroup, TBaseResolver >::Type >();
            descriptor->registrationDescriptorUpdated()(updatedDescriptor);

            return *updatedDescriptor;
        }

        template <int = 0>
        void addGroupItem(TDescriptor*)
        {
        }

        template <class TBase, class... TOthers>
        void addGroupItem(TDescriptor* descriptor)
        {
            Extensions::EnforceBaseOf< TDescriptorInfo, TBase, InstanceType >::act();

            descriptor->addTypeIfMissing(createKeyForType< TBase >(), [](const std::shared_ptr< void >& x)
            {
                auto instanceDynamicType = std::static_pointer_cast< InstanceType >(x);
                auto instanceStaticType = std::static_pointer_cast< TBase >(instanceDynamicType);
                return instanceStaticType;
            });

            addGroupItem< TOthers... >(descriptor);
        }

    protected:
        virtual ~AsGroup() = default;
    };

} // namespace RegistrationDescriptorOperations
} // namespace Hypodermic
