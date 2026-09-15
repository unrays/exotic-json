#ifndef EXOTIC_JSON_STREAM_BUFFER_HPP
#define EXOTIC_JSON_STREAM_BUFFER_HPP

#include <string>
#include <utility>
#include <concepts>
#include "../types.hpp"
#include "../utils.hpp"

namespace exotic::json {

    template<typename Derived>
    struct stream_buffer_base {
    public:
        decltype(auto) length() const {
            return static_cast<Derived*>(this)->length();
        }

        decltype(auto) size() const {
            return static_cast<Derived*>(this)->size();
        }

        void clear() noexcept { static_cast<Derived*>(this)->clear(); }

    protected:
        template<typename T>
        void append(T&& s) {
            static_cast<Derived*>(this)->append(std::forward<T>(s));
        }
    };

    template<typename T>
    concept IsStreamBuffer = std::derived_from<T, stream_buffer_base<T>>;

    /************************************************/

    struct string_buffer : stream_buffer_base<string_buffer> {
        friend stream_buffer_base<string_buffer>;
        using size_type = std::size_t;
    public:
        size_type length() const noexcept { return buffer_.length(); }
        size_type size()   const noexcept { return buffer_.size(); }

        void clear() noexcept { buffer_.clear(); }

    protected:
        template<convertible_to_size_t Sizable>
        void reserve(Sizable&& s) {
            buffer_.reserve(std::forward<Sizable>(s));
        }

    public: //TEMPORAIRE EN ATTENDANT DE FIX WRITER
        template<convertible_to_string_t Stringable>
        void append(Stringable&& s) {
            buffer_.append(std::forward<Stringable>(s));
        }

        EXOTIC_NODISCARD std::string flush() {
            //return std::move(buffer_);
            return buffer_;
        }

    protected:
        EXOTIC_NODISCARD std::string get_buffer_copy() noexcept { return buffer_; }
        EXOTIC_NODISCARD std::string& get_buffer() noexcept { return buffer_; }
        EXOTIC_NODISCARD const std::string& get_buffer() const noexcept { return buffer_; }

    private:
        std::string buffer_;
    };

} // namespace exotic::json

#endif