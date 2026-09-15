#ifndef EXOTIC_JSON_OUTPUT_STREAM_HPP
#define EXOTIC_JSON_OUTPUT_STREAM_HPP

#include "stream_buffer.hpp"

namespace exotic::json {

    /************************************************/

    //template<typename Derived>
    //struct string_writer_strategy {
    //    // FAIRE CRTP POLYMORPHISM ET AJOUTER UN CONCEPT
    //};

    template<typename Derived>
    struct string_writer_strategy {
        template<convertible_to_string_t Stringable>
        void write(Stringable&& input) {
            static_cast<Derived*>
                (this)->append(std::forward<Stringable>(input));
        }

        // end?
    };

    /************************************************/

    //AJOUTER CONCEPT POUR WRITER
    template<IsStreamBuffer Buffer, template<typename> class Writer>
    struct output_stream : // peut etre tout foutr dans serializer directement
        public Buffer,
        public Writer<output_stream<Buffer, Writer>>
    {
    };

} // namespace exotic::json

#endif