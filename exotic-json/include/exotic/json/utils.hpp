#ifndef EXOTIC_JSON_UTILS_HPP
#define EXOTIC_JSON_UTILS_HPP

#include "types.hpp"
#include <concepts>
#include <variant>

namespace exotic::json {

    template<typename T>
    concept convertible_to_string_t = requires(T t) { string_t(t); };

    template<typename T>
    concept convertible_to_size_t = requires(T t) { size_t(t); };

    template<typename... Ts>
    TODO_FIX_THIS decltype(auto) utility_get_variant_type(const std::variant<Ts...>& v) {
        return std::visit([](EXOTIC_MAYBE_UNUSED auto const& val) -> decltype(auto) {
            return val;
            }, v);
    }

} // namespace exotic::json

#endif