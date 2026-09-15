#ifndef EXOTIC_JSON_OBJECT_SERIALIZER_HPP
#define EXOTIC_JSON_OBJECT_SERIALIZER_HPP

#include "../json_object.hpp"

namespace exotic::json {

    /************************************************/

    struct object_serializer_behavior {
        template<typename Stream>
        static void apply(Stream&& stream, json_object& obj) {
            stream.write("{\n");

            for (auto it = obj.begin(); it != obj.end(); ++it) {
                (void)serialize(std::forward<Stream>(stream), *it);

                if (std::next(it) != obj.end()) stream.write(", ");
                stream.write("\n");
            }

            stream.write("}");
        }
    };

} // namespace exotic::json

#endif