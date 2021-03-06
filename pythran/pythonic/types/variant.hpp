#ifndef PYTHONIC_TYPES_VARIANT_HPP
#define PYTHONIC_TYPES_VARIANT_HPP

#include <utility>

namespace pythonic {

    namespace types {
        template <class... Types>
            struct variant {};

        template <class Type>
            struct variant<Type> {
                char mem[sizeof(Type)];
                Type* t;

                variant() : t(nullptr) {}
                variant(Type const& t) : t(new (mem) Type(t)) {}
                variant(variant<Type> const& t) : t(t.t?new (mem) Type(*t.t):t.t) {}
                template<class OtherType>
                    variant(variant<OtherType> const& t) : t(nullptr) {}

                template <class... Args>
                    auto operator()(Args&&... args) -> decltype( std::declval<Type>()(std::forward<Args>(args)...) ) {
                        return (*t)(std::forward<Args>(args)...);
                    }

            };

        template<class Type, class... Types>
            struct variant<Type, Types...> {
                typedef void callable;
                char mem[sizeof(Type)];
                Type* t;
                variant<Types...> ot;

                variant() : t(nullptr), ot() {}
                variant(Type const& t) : t(new (mem) Type(t)), ot() {}
                template<class T>
                    variant(T const& t) : t(nullptr), ot(t) {}

                // the below constructor handle case when Types and OtherTypes are not exactly the same
                // or are in a different order
                template<class OtherType, class... OtherTypes>
                    variant(variant<OtherType, OtherTypes...> const& t) :
                        t(nullptr),
                        ot(t.ot) {}

                template<class... OtherTypes>
                    variant(variant<Type, OtherTypes...> const& t) :
                        t(t.t?new (mem) Type(*t.t):t.t),
                        ot(t.ot) {}

                template <class... Args>
                    auto operator()(Args&&... args) -> decltype( std::declval<Type>()(args...)){
                        if(t) return (*t)(args...);
                        else return ot(args...);
                    }
            };

    }
}
#endif
