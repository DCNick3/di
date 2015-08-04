//
// Copyright (c) 2012-2015 Krzysztof Jusiak (krzysztof at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef BOOST_DI_WRAPPERS_SHARED_HPP
#define BOOST_DI_WRAPPERS_SHARED_HPP

#include "boost/di/aux_/compiler.hpp"
#include "boost/di/aux_/utility.hpp"
#include "boost/di/aux_/type_traits.hpp"
#include "boost/di/fwd.hpp"

namespace boost { namespace di { inline namespace v1 { namespace wrappers {

template<class T>
struct shared {
    template<class>
    struct is_referable
        : std::true_type
    { };

    template<class I>
    struct is_referable<std::shared_ptr<I>>
        : std::false_type
    { };

    template<class I>
    inline operator std::shared_ptr<I>() const noexcept {
        return object;
    }

    template<class I>
    inline operator boost::shared_ptr<I>() const noexcept {
        struct sp_holder {
            std::shared_ptr<T> object;
            void operator()(...) noexcept { object.reset(); }
        };
        return {object.get(), sp_holder{object}};
    }

    template<class I>
    inline operator std::weak_ptr<I>() const noexcept {
        return object;
    }

    inline operator T&() noexcept {
        return *object;
    }

    inline operator const T&() const noexcept {
        return *object;
    }

    std::shared_ptr<T> object;
};

template<class T>
struct shared<T*> {
    template<class>
    struct is_referable
        : std::true_type
    { };

    template<class I>
    struct is_referable<std::shared_ptr<I>>
        : std::false_type
    { };

    inline operator T&() noexcept {
        return *object;
    }

    inline operator const T&() const noexcept {
        return *object;
    }

    #if defined(BOOST_DI_MSVC) // __pph__
        explicit shared(T* object)
            : object(object)
        { }
    #endif // __pph__

    T* object = nullptr;
};

template<class T>
struct shared<T&> {
    shared(T& object) // non explicit
        : object(&object)
    { }

    template<class I, BOOST_DI_REQUIRES(std::is_convertible<T&, I>::value)>
    inline operator I() const noexcept {
        return *object;
    }

    inline operator T&() const noexcept {
        return *object;
    }

    T* object = nullptr;
};

}}}} // boost::di::v1::wrappers

#endif

