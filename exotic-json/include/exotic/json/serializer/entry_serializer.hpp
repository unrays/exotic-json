#ifndef EXOTIC_JSON_ENTRY_SERIALIZER_HPP
#define EXOTIC_JSON_ENTRY_SERIALIZER_HPP

#include "../json_entry.hpp"

namespace exotic::json {

    /************************************************/

    struct entry_serializer_behavior {
        template<typename Stream>
        static void apply(Stream&& stream, json_entry& entry) {
            std::visit([&](auto& val) {
                stream.write("\"" + entry.key + "\": ");
                (void)serialize(std::forward<Stream>(stream), val);
            }, entry.value);
        }
    };

} // namespace exotic::json

#endif