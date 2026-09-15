#ifndef EXOTIC_JSON_VALUE_SERIALIZER_HPP
#define EXOTIC_JSON_VALUE_SERIALIZER_HPP

#include "../json_value.hpp"
#include "../json_object.hpp"
#include "../json_entry.hpp"
#include "../stream/output_stream.hpp"
#include <format>

namespace exotic::json {

    /************************************************/

    template<typename>
    struct value_serializer_behavior {
        template<typename Stream, typename T>
        static void apply(Stream&& stream, T&& v) {
            stream.write("\"Cannot serialize value\"");
        }
    };

    template<>
    struct value_serializer_behavior<json_value<bool_t>> {
        template<typename Stream>
        static void apply(Stream&& stream, json_value<bool_t>& v) {
            stream.write(v.get() ? "true" : "false");
        }
    };

    template<>
    struct value_serializer_behavior<json_value<number_t>> {
        template<typename Stream>
        static void apply(Stream&& stream, json_value<number_t>& v) {
            stream.write(std::format("{}", v.get()));
        }
    };

    template<>
    struct value_serializer_behavior<json_value<string_t>> {
        template<typename Stream>
        static void apply(Stream&& stream, json_value<string_t>& v) {
            stream.write("\"" + v.get() + "\"");
        }
    };

    template<>
    struct value_serializer_behavior<json_value<object_t>> {
        template<typename Stream>
        static void apply(Stream&& stream, json_value<object_t>& v) {
            object_t& obj = v.get();

            stream.write("{\n");

            for (auto it = obj->begin(); it != obj->end(); ++it) { // APRES LE FIX (unique)
                stream.write("\t");

                (void)serialize(std::forward<Stream>(stream), *it);

                if (std::next(it) != obj->end()) stream.write(", ");
                stream.write("\n");
            }

            stream.write("}");
        }
    };

    template<>
    struct value_serializer_behavior<json_value<array_t>> {
        template<typename Stream>
        static void apply(Stream&& stream, json_value<array_t>& v) {
            array_t& arr = v.get();

            stream.write("[\n");

            for (auto it = arr.begin(); it != arr.end(); ++it) {
                stream.write("\t");

                std::visit([&](auto& val) {
                    using sub_t = std::remove_cvref_t<decltype(val)>;
                    (void)serialize(std::forward<Stream>(stream), val);
                }, *it);

                if (std::next(it) != arr.end()) stream.write(", ");
                stream.write("\n");
            }

            stream.write("]");
        }
    };

} // namespace exotic::json

#endif