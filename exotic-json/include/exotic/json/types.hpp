#ifndef EXOTIC_JSON_TYPES_HPP
#define EXOTIC_JSON_TYPES_HPP

#include "macros.hpp"
#include <string>
#include <vector>
#include <tuple>
#include <variant>
#include <type_traits>
#include <iostream>

namespace exotic::json {

    template<typename>
    inline constexpr bool always_false_v = false;

    struct sentinel_t {};
    inline constexpr sentinel_t sentinel{};

    template<typename T> struct json_value;
    struct json_entry;
    struct JsonValue;
    struct json_object;

    using nul_t = sentinel_t;
    using bool_t = bool;
    using number_t = double;
    using string_t = std::string;
    using array_t = std::vector<JsonValue>;

    //using array_t = std::unique_ptr<std::vector<JsonValue>>;
    //using object_t = std::unique_ptr<json_object>;

    //using object_t = std::vector<json_entry>; // probablement avoir a fix

    //using object_t = std::unique_ptr<json_object>;

    struct object_t {
        std::shared_ptr<json_object> ptr;

        object_t() = default;


        /*object_t(std::initializer_list<json_entry> init)
            : ptr(std::make_unique<json_object>(std::vector<json_entry>(init))) {
        }*/

        object_t(json_object&& obj)
            : ptr(std::make_shared<json_object>(std::move(obj))) {
        }

        object_t(json_object& obj)
            : ptr(std::make_shared<json_object>(obj)) {
        }

        object_t(std::initializer_list<json_entry> init)
            : ptr(std::make_shared<json_object>(init)) {
        }

        json_object* operator->() { return ptr.get(); }
        const json_object* operator->() const { return ptr.get(); }

        json_object& operator*() { return *ptr; }
        const json_object& operator*() const { return *ptr; }
    };

    using all_json_types = std::tuple<
        std::type_identity<nul_t>,
        std::type_identity<bool_t>,
        std::type_identity<number_t>,
        std::type_identity<string_t>,
        std::type_identity<array_t>,
        std::type_identity<object_t>
    >;

    inline static constexpr all_json_types valid_json_types;

} // namespace exotic::json

#endif