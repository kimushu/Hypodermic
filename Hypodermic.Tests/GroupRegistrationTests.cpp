#include "stdafx.h"

#include "Hypodermic/ContainerBuilder.h"

#include "TestingTypes.h"


namespace Hypodermic
{
namespace Testing
{

    template <class TItem>
    struct FilterBaseResolver
    {
        using Type = IFilter<TItem>;
    };

    struct GroupRegistrationFixture
    {
        ~GroupRegistrationFixture()
        {
            Behavior::configureRuntimeRegistration(true);
        }
    };

    BOOST_FIXTURE_TEST_SUITE(GroupRegistrationTests, GroupRegistrationFixture)

    BOOST_AUTO_TEST_CASE(should_register_all_types_in_a_group)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< TypeWithGroups >()
            .asGroup< IGroup >()
            .singleInstance();

        auto container = builder.build();

        // Assert
        auto instance1 = container->resolve< BaseType1 >();
        BOOST_CHECK(instance1 != nullptr);

        auto derived1 = std::dynamic_pointer_cast< TypeWithGroups >(instance1);
        BOOST_CHECK(derived1 != nullptr);

        auto instance2 = container->resolve< BaseType2 >();
        BOOST_CHECK(instance2 != nullptr);

        auto derived2 = std::dynamic_pointer_cast< TypeWithGroups >(instance1);
        BOOST_CHECK(derived2 != nullptr);

        BOOST_CHECK(derived1 == derived2);

        Behavior::configureRuntimeRegistration(false);
        BOOST_CHECK_THROW(container->resolve< TypeWithGroups >(), ResolutionException);
        BOOST_CHECK_THROW(container->resolve< IFilter<int> >(), ResolutionException);
        BOOST_CHECK_THROW(container->resolve< IFilter<char> >(), ResolutionException);
    }

    BOOST_AUTO_TEST_CASE(should_register_all_types_in_a_group_and_self)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< TypeWithGroups >()
            .asGroup< IGroup >()
            .asSelf();

        auto container = builder.build();

        // Assert
        auto instance1 = container->resolve< BaseType1 >();
        BOOST_CHECK(instance1 != nullptr);

        auto derived1 = std::dynamic_pointer_cast< TypeWithGroups >(instance1);
        BOOST_CHECK(derived1 != nullptr);

        auto instance2 = container->resolve< BaseType2 >();
        BOOST_CHECK(instance2 != nullptr);

        auto derived2 = std::dynamic_pointer_cast< TypeWithGroups >(instance1);
        BOOST_CHECK(derived2 != nullptr);

        auto self = container->resolve< TypeWithGroups >();
        BOOST_CHECK(derived1 == derived2);
        BOOST_CHECK(derived1 != self);

        Behavior::configureRuntimeRegistration(false);
        BOOST_CHECK_THROW(container->resolve< IFilter<int> >(), ResolutionException);
        BOOST_CHECK_THROW(container->resolve< IFilter<char> >(), ResolutionException);
    }

    BOOST_AUTO_TEST_CASE(should_register_all_types_in_two_groups)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< TypeWithGroups >()
            .asGroup< IGroup >()
            .asGroup< IGroupWithFilter, FilterBaseResolver >();

        auto container = builder.build();

        // Assert
        auto instance1 = container->resolve< BaseType1 >();
        BOOST_CHECK(instance1 != nullptr);

        auto derived1 = std::dynamic_pointer_cast< TypeWithGroups >(instance1);
        BOOST_CHECK(derived1 != nullptr);

        auto instance2 = container->resolve< BaseType2 >();
        BOOST_CHECK(instance2 != nullptr);

        auto derived2 = std::dynamic_pointer_cast< TypeWithGroups >(instance1);
        BOOST_CHECK(derived2 != nullptr);

        auto instance3 = container->resolve< IFilter<int> >();
        BOOST_CHECK(instance3 != nullptr);

        auto derived3 = std::dynamic_pointer_cast< TypeWithGroups >(instance1);
        BOOST_CHECK(derived3 != nullptr);

        auto instance4 = container->resolve< IFilter<char> >();
        BOOST_CHECK(instance4 != nullptr);

        auto derived4 = std::dynamic_pointer_cast< TypeWithGroups >(instance1);
        BOOST_CHECK(derived4 != nullptr);

        BOOST_CHECK(derived1 == derived2);
        BOOST_CHECK(derived1 == derived3);
        BOOST_CHECK(derived1 == derived4);

        Behavior::configureRuntimeRegistration(false);
        BOOST_CHECK_THROW(container->resolve< TypeWithGroups >(), ResolutionException);
    }

    BOOST_AUTO_TEST_CASE(should_ignore_empty_group)
    {
        // Arrange
        ContainerBuilder builder;

        // Act
        builder.registerType< TypeWithEmptyGroup >()
            .asGroup< IGroup >();

        auto container = builder.build();

        // Assert
        auto instance = container->resolve< TypeWithEmptyGroup >();
        BOOST_CHECK(instance != nullptr);
    }

    BOOST_AUTO_TEST_CASE(should_be_functional_for_registerInstance)
    {
        // Arrange
        ContainerBuilder builder;
        auto instance = std::make_shared< TypeWithGroups >();

        // Act
        builder.registerInstance(instance).asGroup< IGroup >();

        auto container = builder.build();

        // Assert
        auto instance1 = container->resolve< BaseType1 >();
        BOOST_CHECK(instance1 == instance);

        auto instance2 = container->resolve< BaseType2 >();
        BOOST_CHECK(instance2 == instance);

        Behavior::configureRuntimeRegistration(false);
        BOOST_CHECK_THROW(container->resolve< TypeWithGroups >(), ResolutionException);
    }

    BOOST_AUTO_TEST_CASE(should_be_functional_for_registerInstanceFactory)
    {
        // Arrange
        ContainerBuilder builder;
        std::shared_ptr< TypeWithGroups > instance;

        // Act
        builder.registerInstanceFactory([&instance](ComponentContext&)
        {
            instance = std::make_shared< TypeWithGroups >();
            return instance;
        }).asGroup< IGroup >();

        auto container = builder.build();

        // Assert
        auto instance1 = container->resolve< BaseType1 >();
        BOOST_CHECK(instance1 == instance);

        auto instance2 = container->resolve< BaseType2 >();
        BOOST_CHECK(instance2 == instance);

        Behavior::configureRuntimeRegistration(false);
        BOOST_CHECK_THROW(container->resolve< TypeWithGroups >(), ResolutionException);
    }

    BOOST_AUTO_TEST_SUITE_END()

} // namespace Testing
} // namespace Hypodermic
