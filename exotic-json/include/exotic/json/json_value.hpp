#ifndef EXOTIC_JSON_VALUE_HPP
#define EXOTIC_JSON_VALUE_HPP

#include "macros.hpp"
#include "types.hpp"
#include "traits.hpp"
#include <iostream>
#include <utility>

namespace exotic::json {

    template<typename Derived, typename Type>
    struct json_data_constructor_impl { // constructeur de base
        constexpr json_data_constructor_impl() = default;
        constexpr json_data_constructor_impl(Type v) {
            static_cast<Derived*>(this)->value = std::move(v);
        }
    };

    template<typename Derived>
    struct json_data_constructor_impl<Derived, object_t> {
        json_data_constructor_impl(object_t v) {
            static_cast<Derived*>(this)->value = std::move(v);
        }

        /*json_data_constructor_impl(std::initializer_list<json_entry> init) { // AVANT FIX
            static_cast<Derived*>(this)->value = init;

            std::cout << "json_data_constructor_impl init_list CONSTRUCTOR\n";
        }*/

        //json_data_constructor_impl(std::initializer_list<json_entry> init) { // après fix
        //    static_cast<Derived*>(this)->value =
        //        std::make_unique<json_object>(std::vector<json_entry>(init));
        //}

        json_data_constructor_impl(std::initializer_list<json_entry> init) {
            static_cast<Derived*>(this)->value = object_t{ init };
        }
    };

    template<typename Derived>
    struct json_data_constructor_impl<Derived, array_t> {
        json_data_constructor_impl(std::initializer_list<JsonValue> init) {
            static_cast<Derived*>(this)->value = init;
        }

        json_data_constructor_impl(array_t v) { // sus?
            static_cast<Derived*>(this)->value = std::move(v);
        }
    };

    template<typename, typename>
    struct json_data_constructor {
        static_assert(always_false_v<sentinel_t>,
            "json_data_constructor<T, U> requires a valid json type");
    };

    template<typename Derived, valid_json_type T>
    struct json_data_constructor<Derived, T> : json_data_constructor_impl<Derived, T> {
        using json_data_constructor_impl<Derived, T>::json_data_constructor_impl;
    };

    template<typename T>
    struct json_data_holder_impl {
    private:
        T value;
    public:
        template<typename, typename> friend struct json_data_constructor_impl;

        EXOTIC_NODISCARD inline constexpr T& get()& { return value; }
        EXOTIC_NODISCARD inline constexpr const T& get() const& { return value; }
        EXOTIC_NODISCARD inline constexpr T&& get()&& { return std::move(value); }
        EXOTIC_NODISCARD inline constexpr const T&& get() const&& { return std::move(value); }
    };

    template<typename> struct json_data_holder {
        static_assert(always_false_v<sentinel_t>,
            "json_data_holder<T> requires a valid json type");
    };

    template<valid_json_type T>
    struct json_data_holder<T> : json_data_holder_impl<T> {
        using json_data_holder_impl<T>::json_data_holder_impl;
    };

    template<typename Container, typename Constructor>
    struct json_data_wrapper : Container, Constructor {
        using Constructor::Constructor;
    };

    template<typename T>
    struct json_type_traits_impl {
        static constexpr std::size_t size = sizeof(T);
        using type = T;
    };

    template<typename> struct json_type_traits {
        static_assert(always_false_v<sentinel_t>,
            "json_type_traits<T> requires a valid json type");
    };

    template<valid_json_type T>
    struct json_type_traits<T> : json_type_traits_impl<T> {};

    template<typename> struct json_value_base;

    template<typename T>
    struct json_value_base :
        json_data_holder<T>, json_data_constructor<json_value_base<T>, T>,
        json_type_traits<T>
    {
        using json_data_constructor<json_value_base<T>, T>::json_data_constructor;
    };

    template<typename T>
    struct json_value : json_value_base<std::remove_cvref_t<T>> {
        using json_value_base<std::remove_cvref_t<T>>::json_value_base;
    };

    struct JsonValue : std::variant<
        json_value<std::remove_cvref_t<nul_t>>,
        json_value<std::remove_cvref_t<bool_t>>,
        json_value<std::remove_cvref_t<array_t>>,
        json_value<std::remove_cvref_t<number_t>>,
        json_value<std::remove_cvref_t<string_t>>,
        json_value<std::remove_cvref_t<object_t>>
    > {
        using variant::variant;
        using Base = std::variant<
            json_value<std::remove_cvref_t<nul_t>>,
            json_value<std::remove_cvref_t<bool_t>>,
            json_value<std::remove_cvref_t<array_t>>,
            json_value<std::remove_cvref_t<number_t>>,
            json_value<std::remove_cvref_t<string_t>>,
            json_value<std::remove_cvref_t<object_t>>
        >;
        using Base::operator=;
        using Base::emplace;

        JsonValue(bool v) : Base(json_value<bool_t>(v)) {}
        JsonValue(double v) : Base(json_value<number_t>(v)) {}
        JsonValue(const char* v) : Base(json_value<string_t>(v)) {}
        JsonValue(std::string v) : Base(json_value<string_t>(std::move(v))) {}
        JsonValue(array_t v) : Base(json_value<array_t>(std::move(v))) {}

        //JsonValue(object_t v) : Base(json_value<object_t>(std::move(v))) {} // AVANT FIX

        //JsonValue(std::initializer_list<json_entry> init) // POST FIX
        //    : Base(json_value<object_t>(std::make_unique<json_object>(
        //        std::vector<json_entry>(init)
        //    ))) {
        //}

        JsonValue(std::initializer_list<json_entry> init)  // APRÈS FIX
            : Base(json_value<object_t>(object_t{ init })) {
        }

        //JsonValue& operator=(std::vector<JsonValue> v) {
        //    Base::template emplace<json_value<array_t>>(std::move(v));
        //    return *this;
        //}

        JsonValue& operator=(array_t& v) {
            std::cout << "USING JSONVALUE ARRAY OPERATOR=\n";

            std::cout << "arr size: " << v.size() << "\n";

            for (auto e : v) {
                std::cout << "+++++++++\n";
                std::visit([&](auto& sub) {
                    using sub_t = std::remove_cvref_t<decltype(sub)>;

                    std::cout << typeid(sub_t).name() << "\n";

                    if constexpr (std::same_as<sub_t, exotic::json::json_value<std::string>>) {
                        std::cout << sub.get() << " +++++\n";
                    }
                    }, e);
            }
            std::cout << "end of output\n";

            Base::template emplace<json_value<array_t>>(std::move(v));


            return *this;
        }

        JsonValue& operator=(object_t v) { // APRES LE FIX
            Base::template emplace<json_value<object_t>>(std::move(v));
            return *this;
        }
    };

} // namespace exotic::json

#endif