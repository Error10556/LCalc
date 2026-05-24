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

template <class TVariant, class TMatcher>
decltype(auto) operator|(TVariant&& variant, TMatcher&& vis) {
    return std::visit(std::forward<TMatcher>(vis),
                      std::forward<TVariant>(variant));
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
