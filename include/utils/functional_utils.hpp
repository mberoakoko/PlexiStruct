//
// Created by mbero on 03/07/2025.
//

#ifndef FUNCTIONAL_UTILS_HPP
#define FUNCTIONAL_UTILS_HPP
#include <utility>


namespace Plexistruct::functional {
    template<typename F, typename G>
    auto compose(F&& f, G&& g)  {
        return [=](auto x){return f(g(x));};
    }

    template<typename F, typename ... F_Rest>
    auto compose(F&& f, F_Rest... r) {
        return [=](auto x) { return f(compose(r...)(x)); };
    }
}


#endif //FUNCTIONAL_UTILS_HPP
