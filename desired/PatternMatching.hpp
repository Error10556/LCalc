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

template <class... TVariants, class TMatcher>
decltype(auto) operator|(std::variant<TVariants...>&& variant, TMatcher&& vis) {
    return std::visit(std::forward<TMatcher>(vis), std::move(variant));
}

template <class... TVariants, class TMatcher>
decltype(auto) operator|(const std::variant<TVariants...>& variant, TMatcher&& vis) {
    return std::visit(std::forward<TMatcher>(vis), variant);
}

template <class... TVariants, class TMatcher>
decltype(auto) operator|(std::variant<TVariants...>& variant, TMatcher&& vis) {
    return std::visit(std::forward<TMatcher>(vis), variant);
}

template <class... TVariant, class TMatcher>
decltype(auto) operator|(std::tuple<TVariant...>&& vars, TMatcher&& vis) {
    return std::apply(
        [&vis](auto&&... args) {
            return std::visit(std::forward<TMatcher>(vis),
                              std::forward<decltype(args)>(args)...);
        },
        std::move(vars));
}
