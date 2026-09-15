#ifndef EXOTIC_JSON_TRAITS_HPP
#define EXOTIC_JSON_TRAITS_HPP

#include "types.hpp"
#include "macros.hpp"
#include <tuple>
#include <type_traits>

namespace exotic::json {

    template<typename T>
    constexpr bool check_valid_json_type_impl() {
        return std::apply([]<typename... Ts>(EXOTIC_MAYBE_UNUSED Ts&...) -> bool {
            return (... || std::is_same_v<T, std::remove_cvref_t<typename Ts::type>>);
        }, valid_json_types);
    };

    template<typename T>
    concept valid_json_type = check_valid_json_type_impl<std::remove_cvref_t<T>>();

    template<typename T>
    concept is_json_value_object = std::same_as<T, json_value<object_t>>;

    template<typename T>
    concept is_json_value_array = std::same_as<T, json_value<array_t>>;

} // namespace exotic::json

#endif