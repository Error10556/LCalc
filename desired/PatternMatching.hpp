#pragma once
#include <utility>
#include <variant>
#include <tuple>

template <class... TCase>
class PatternMatch : TCase... {
public:
    PatternMatch(TCase&&... closure)
        : TCase(std::forward<TCase>(closure))... {}
    using TCase::operator()...;
};

template <class TVariant, class... TCase>
decltype(auto) operator|(TVariant&& variant, const PatternMatch<TCase...>& vis) {
    return std::visit(vis, variant);
}

template <class... TVariant, class... TCase>
decltype(auto) operator|(std::tuple<TVariant...>&& vars, const PatternMatch<TCase...>& vis) {
    return std::apply(
        [&vis](auto&&... args) {
            return std::visit(
                vis, std::forward<decltype(args)>(args)...);
        },
        std::move(vars));
}
