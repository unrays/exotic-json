#ifndef EXOTIC_JSON_SERIALIZER_HPP
#define EXOTIC_JSON_SERIALIZER_HPP

#include "value_serializer.hpp"
#include "entry_serializer.hpp"
#include "object_serializer.hpp"
#include "../macros.hpp"

namespace exotic::json {

    /************************************************/

     //FAUT RESERVE SUR LE BUFFER
    template<typename Stream, typename Behavior>
    struct serializer {
        template<typename... Ts>
        EXOTIC_NODISCARD static decltype(auto) serialize(Stream& stream, Ts&&... args) {
            Behavior::apply(stream, std::forward<Ts>(args)...);
            return stream.flush();
        }

        template<typename... Ts>
        EXOTIC_NODISCARD decltype(auto) serialize(Ts&&... args) {
            auto static_stream = Stream{};
            return this->serialize(static_stream, std::forward<Ts>(args)...);
        }
    };

    /****************************************************************/

    template<typename...> struct serialization_resolver {
        static_assert(always_false_v<sentinel_t>,
            "serialization_resolver<T> requires valid arguments...");
    };

    /****************************************************************/

    template<typename Stream, typename T>
    struct serialization_resolver<Stream, json_value<T>>
        : serializer<Stream, value_serializer_behavior<json_value<T>>> {
    };

    template<typename T>
    struct serialization_resolver<json_value<T>>
        : serializer<
            output_stream<string_buffer, string_writer_strategy>,
            value_serializer_behavior<json_value<T>>
        > {
    };

    /****************************************************************/

    template<typename Stream>
    struct serialization_resolver<Stream, json_entry>
        : serializer<Stream, entry_serializer_behavior> {
    };

    template<>
    struct serialization_resolver<json_entry>
        : serializer<
            output_stream<string_buffer, string_writer_strategy>,
            entry_serializer_behavior
        > {
    };

    /****************************************************************/

    template<typename Stream>
    struct serialization_resolver<Stream, json_object>
        : serializer<Stream, object_serializer_behavior> {
    };

    template<>
    struct serialization_resolver<json_object>
        : serializer<
            output_stream<string_buffer, string_writer_strategy>,
            object_serializer_behavior
        > {
    };

    /****************************************************************/

    template<typename... Ts>
    EXOTIC_NODISCARD decltype(auto) serialize(Ts&&... args) {
        static serialization_resolver<std::remove_cvref_t<Ts>...> static_resolver;
        return static_resolver.serialize(std::forward<Ts>(args)...);
    }

} // namespace exotic::json

#endif