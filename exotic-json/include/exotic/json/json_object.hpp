#ifndef EXOTIC_JSON_OBJECT_HPP
#define EXOTIC_JSON_OBJECT_HPP

#include "types.hpp"
#include "macros.hpp"
#include "json_entry.hpp"
#include <vector>
#include <unordered_map>
#include <initializer_list>
#include <iostream>

namespace exotic::json {

    struct json_object_2 {
        explicit json_object_2(json_entry entry) {}

        // MAYBE LUI SELON LES DIRES DE CHATGPT, je vais peter un cable sérieux, C'EST IMPOSSIBLE
        // externaliser les constructeurs si besoin en strategy
        json_object_2(std::initializer_list<json_entry> init) {
            std::cout << "json_object(std::initializer_list<json_entry> init)\n";

            entries_.reserve(init.size());
            for (auto const& e : init) {
                std::cout << "+1\n";

                entries_.push_back(e);

            }
        }



        //template<typename... Ts>
        //json_object(Ts&&... ts) {
        //    (entries_.emplace_back(std::forward<Ts>(ts)), ...);
        //}

        template<typename T>
        using is_json_entry_compatible = std::disjunction<
            std::is_same<std::remove_cvref_t<T>, json_entry>,
            std::is_constructible<json_entry, T>
        >;

        // variadic constructor avec SFINAE
        template<typename... Ts>
        json_object_2(Ts&&... ts)
            requires (is_json_entry_compatible<Ts>::value && ...)
        {
            (entries_.emplace_back(std::forward<Ts>(ts)), ...);
        }

        EXOTIC_NODISCARD auto begin() {
            return entries_.begin(); // std::_Vector_iterator<...>
        } // temporaire
        EXOTIC_NODISCARD auto end() {
            return entries_.end(); // std::_Vector_iterator<...>
        }

    private:
        std::vector<json_entry> entries_; //reserve?
    };

    struct json_object {
        using index_t = std::size_t;
        static constexpr std::size_t DEFAULT_RESERVE = 24;

        json_object() = default;

        json_object(std::initializer_list<json_entry> init) {
            std::cout << "json_object_2(std::initializer_list<json_entry> init)\n";

            entries_.reserve(DEFAULT_RESERVE);
            map_index_.reserve(DEFAULT_RESERVE);

            for (auto const& e : init) {
                entries_.push_back(e);
                map_index_.emplace(e.key, entries_.size() - 1);
            }
        }

        std::size_t size() { return entries_.size(); }

        auto& operator[](entry_key_t key) { // voir si passer ref ou copie, 
            std::cout << "accessing via operator[]\n";
            if (!map_index_.contains(key)) {
                entries_.push_back(json_entry{ key, json_value<nul_t>{} });
                map_index_.emplace(key, entries_.size() - 1);
            }
            return entries_[map_index_[key]].value;
        }

        auto& operator[](index_t idx) {
            return entries_[idx].value;
        }

        auto& at(entry_key_t key) {
            if (!map_index_.contains(key)) 
                throw std::out_of_range("Index out of bounds!");
            return entries_[map_index_[key]].value;
        }

        decltype(auto) find(entry_key_t key) {
            decltype(auto) it = entries_.begin();
            for (; it != entries_.end(); ++it)
                if (it->key == key) break;
            return it;
        }


        bool contains(entry_key_t key) const {
            return map_index_.contains(key);
        }


        void test() {
            for (auto& e : entries_) {
                std::visit([&](auto const& val) {
                    using T = std::remove_cvref_t<decltype(val)>;
                    if constexpr (std::is_same_v<T, json_value<number_t>>) {
                        std::cout << "Name: " << e.key
                            << " Value: " << val.get() << "\n";
                    }
                    }, e.value);
            }
        }

    public: // temporaire
        EXOTIC_NODISCARD decltype(auto) begin() { return entries_.begin(); }
        EXOTIC_NODISCARD decltype(auto) end() { return entries_.end(); }

    private:
        std::vector<json_entry> entries_; //reserve?
        std::unordered_map<entry_key_t, std::size_t> map_index_;
    };

} // namespace exotic::json

#endif