#ifndef PYTHONIC_NUMPY_ISREAL_HPP
#define PYTHONIC_NUMPY_ISREAL_HPP

#include "pythonic/utils/proxy.hpp"
#include "pythonic/types/ndarray.hpp"
#include "pythonic/types/traits.hpp"

namespace pythonic {

    namespace numpy {
        template<class E>
            typename std::enable_if<
            types::is_complex<typename types::numpy_expr_to_ndarray<E>::type::dtype>::value,
            types::ndarray<bool, types::numpy_expr_to_ndarray<E>::N>
                >::type
                isreal(E const& expr) {
                    types::ndarray<bool, types::numpy_expr_to_ndarray<E>::N> out(expr.shape, __builtin__::None);
                    for(long i=0, n=expr.size(); i<n; ++i)
                        out.at(i) = not expr.at(i).imag();
                    return out;
                }

        template<class E>
            typename std::enable_if<
            not types::is_complex<typename types::numpy_expr_to_ndarray<E>::type::dtype>::value,
                types::ndarray<bool, types::numpy_expr_to_ndarray<E>::N>
                    >::type
                    isreal(E const& expr) {
                        return types::ndarray<bool, types::numpy_expr_to_ndarray<E>::N>(expr.shape, true); 
                    }

        PROXY(pythonic::numpy, isreal);

    }

}

#endif

