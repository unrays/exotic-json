#ifndef EXOTIC_JSON_ENTRY_HPP
#define EXOTIC_JSON_ENTRY_HPP

#include "types.hpp"
#include "json_value.hpp"
#include <string>
#include <variant>

namespace exotic::json {

    using entry_key_t = std::string;
    template<typename T> using entry_value_t = json_value<T>;

    //peut etre faire isJsonValueType concept pour remplacer T
    //héritage pour composition des données + héritage constructeur?
    struct json_entry {
        template<typename T>
        json_entry(entry_key_t k, T v) : key(k), value(json_value<T>{ v }) {}

        template<typename T>
        json_entry(entry_key_t k, entry_value_t<T> v) : key(k), value(v) {}

        //json_entry(entry_key_t k, entry_value_t<object_t> v) : key(k), value(v) {}

        //json_entry(entry_key_t k, entry_value_t<object_t> v) : key(k), value(v) {}

        /*json_entry(entry_key_t k, entry_value_t<object_t> v) OLD POUR OBJECT
            : key(k), value(v) {
        }*/

        json_entry(entry_key_t k, object_t v) // le nouveau
            : key(k), value(json_value<object_t>{ std::move(v) }) {
        }

        json_entry(entry_key_t k, entry_value_t<array_t> v)
            : key(k), value(v) {
        }

        //en théoeir, ya pas vriametn de constructeurs ici

        //json_entry(entry_key_t k, entry_value_t<array_t> v) : key(k), value(v) {}

    public:
        entry_key_t key;
        JsonValue value;
    };

} // namespace exotic::json

#endif