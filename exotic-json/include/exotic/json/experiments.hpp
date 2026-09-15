#ifndef EXOTIC_JSON_EXPERIMENTS_HPP
#define EXOTIC_JSON_EXPERIMENTS_HPP

#include "types.hpp"
#include "json_value.hpp"
#include <string>
#include <variant>
#include "json_entry.hpp"

namespace exotic::json {


//template<typename>
//struct serialize_spec_test {
//    void foo(auto const& v) {
//        std::cout << "UNABLE TO PARSE JSON VALUE";
//        // seule la forme de value change, pas la clé
//    }
//};
//
//template<>
//struct serialize_spec_test<json_value<object_t>> {
//    void foo(json_value<object_t> const& v) {
//        //std::cout << "json_value<object_t> serialize_spec_test";
//
//        const object_t& obj = v.get();
//
//        std::cout << "{\n";
//
//        for (auto it = obj.begin(); it != obj.end(); ++it) {
//            auto& entry = *it;
//
//            std::cout << "\t";
//
//            //call la fonction main qui affiche name
//            std::cout << "\"" << entry.key << "\": ";
//
//            // CALL serialize_spec_test?????????????
//            std::visit([&](EXOTIC_MAYBE_UNUSED auto const& val) {
//                //if constexpr (std::is_same_v<std::remove_cvref_t<decltype(val.get())>, number_t>) {
//                //    std::cout << val.get();
//
//                //    //serialize_spec_test<std::remove_cvref_t<decltype(val)>>{}.foo(val);
//                //}
//
//                serialize_spec_test<std::remove_cvref_t<decltype(val)>>{}.foo(val);
//
//                }, entry.value);
//
//            if (std::next(it) != obj.end()) {
//                std::cout << ", ";
//            }
//
//            //faire un truc de rule + comportement si pas overkill
//
//            std::cout << "\n";
//        }
//
//        std::cout << "}";
//
//
//        //std::cout << std::boolalpha << v.get();
//    }
//};
//
//template<>
//struct serialize_spec_test<json_value<array_t>> {
//    void foo(json_value<array_t> const& v) {
//        const array_t& arr = v.get();
//
//        std::cout << "[\n";
//
//        for (auto it = arr.begin(); it != arr.end(); ++it) {
//            auto& cur_val_variant = *it;
//
//            std::cout << "\t";
//
//            std::visit([&](EXOTIC_MAYBE_UNUSED auto const& val) {
//                //std::cout << "\"" << val << "\": ";
//
//                //std::cout << typeid(std::remove_cvref_t<decltype(val)>).name() << "\n";
//                serialize_spec_test<std::remove_cvref_t<decltype(val)>>{}.foo(val);
//                }, cur_val_variant);
//
//            if (std::next(it) != arr.end()) {
//                std::cout << ", ";
//            }
//
//            std::cout << "\n";
//
//        }
//
//        std::cout << "]";
//    }
//};
//
//template<>
//struct serialize_spec_test<json_value<bool_t>> {
//    void foo(json_value<bool_t> const& v) { // PASSER UNE VALUE + WRITER, PAS ELEMENT COMPLET
//        std::cout << std::boolalpha << v.get();
//    }
//};
//
//template<>
//struct serialize_spec_test<json_value<number_t>> {
//    void foo(json_value<number_t> const& v) {
//        std::cout << v.get();
//    }
//};
//
//template<>
//struct serialize_spec_test<json_value<string_t>> {
//    void foo(json_value<string_t> const& v) {
//        std::cout << "\"" << v.get() << "\"";
//    }
//};
//
//// serialize_behavior<T>
//
//// integrer std::remove_cvref_t dans le public de serialize_behavior
//
//// static_assert
//
//// les envoyer dans un dispatcher ou faire directement partial spec, a voir
//
//// passer le string builder ou whatever au travers de l'arbre
//struct serialize_test {
//    void traverse(json_object& obj) {
//        for (auto it = obj.begin(); it != obj.end(); ++it) {
//            auto& cur_entry = *it;
//            //std::cout << "obj it pass\n";
//
//            //std::cout << "Type: " << typeid(decltype(cur_entry)).name() << "\n";
//            //std::cout << "Key: " << cur_entry.key << "\n";
//
//            std::cout << "\"" << cur_entry.key << "\": ";
//
//            std::visit([&](EXOTIC_MAYBE_UNUSED auto const& val) {
//                serialize_spec_test<std::remove_cvref_t<decltype(val)>>{}.foo(val);
//                }, cur_entry.value);
//
//            std::cout << ",";
//
//            std::cout << "\n";
//        }
//    }
//};

/************************************************************/
//  COMPILE -> exotic::json::json_value<exotic::json::object_t>{{ {"sub_field_0", 10.0} }},
//  COMPILE PAS -> { { "sub_field_0", true } },
// 
//  VOILA LE TOPO, LE COMPILO ESSAYE DE DÉDUIRE RESULTAT EN UN UB LA MOITIÉ DU TEMPS
//  NOTE POUR TOI DE DEMAIN, REGLE CA, FAUT WRAPPER LA CONSTRUCTION
/************************************************************/

#if 0
// constructible_container?
template<typename... Ts>
struct json_generic_constructible {
    // vector pour stocker les args?

    std::tuple<Ts...> tuple;

    json_generic_constructible(Ts&&... init) // JsonValue
        : tuple(std::forward<Ts>(init)...) {

        std::cout << "USING json_generic_constructible\n";

        //vec.assign(il.begin(), il.end());
    }



    /* template<typename... Ts>
        json_generic_constructible(Ts&&... args) : json_generic_constructible({ (JsonValue{args})... }) {
            ((std::cout << "json_generic_constructor -> " << typeid(Ts).name() << "\n"), ...);
        }*/
};

// recuperer en generique
// ensuite, faire un genre de try_build pour entry par exemple
// si ca echoue, assert + message clair

struct json_entry_test {
    template<typename... Ts>
    json_entry_test(json_generic_constructible<Ts...> const& constr_args) {
        std::cout << "json_entry_test received -> "
            << typeid(decltype(constr_args)).name() << "\n";


        std::cout << "KEY: " << std::get<0>(constr_args.tuple) << "\n";

        std::cout << "VAL: "
            << typeid(decltype(std::get<1>(constr_args.tuple))).name()
            << "\n";

        auto val = std::get<1>(constr_args.tuple);

        std::cout << "SUB_KEY: " << val.key << "\n";


        //exotic::json::serialize_test serializer;
        //serializer.traverse(std::get<1>(constr_args.tuple));
    }
    // passer la valeur, pas le truc complet
};

struct json_entry_default_constructible {
    template<typename T>
    json_entry_default_constructible(entry_key_t k, T v) : key(k), value(json_value<T>{ v }) {}

    template<typename T>
    json_entry_default_constructible(entry_key_t k, entry_value_t<T> v) : key(k), value(v) {}

  /*  json_entry_default_constructible(entry_key_t k, entry_value_t<object_t> v)
        : key(k), value(v) {
    }*/

    json_entry_default_constructible(entry_key_t k, entry_value_t<object_t> v) // le nouveau
        : key(k), value(json_value<object_t>{ std::move(v) }) {
    }

    json_entry_default_constructible(entry_key_t k, entry_value_t<array_t> v)
        : key(k), value(v) {
    }

public:
    entry_key_t key;
    JsonValue value;
};

struct json_test_object {
    std::vector<json_entry> entries_;

    //template<typename... Ts>
    //requires (std::constructible_from<json_entry, Ts> && ...)
    //json_test_object(Ts... init) {
    //    std::cout << "json_test_object(Ts... init)\n";

    //}

    // après ca, je peux passer le construcible pack avec
    // static dispatch(étant donné que le pack est typé Ts...)
    // et construire par rapport au contenui

    // TODO: on sait que le json_generic_constructible<Ts...> est typé
    //       il est alors possible de faire du dispatch statique

    json_test_object(std::initializer_list<
        json_generic_constructible<std::string, json_entry_default_constructible>
    > init) {
        std::cout << "json_test_object(std::initializer_list<json_generic_constructible> init)\n";

        // ici, on peux définir le type qu'on veux construire, par exemple, 
        // j'ai construit json_entry_test, uniquement lui peut se faire construire

        // de plus, étant donné que la logique de construction de charge de valider
        // les données (plus l'interception compile-time avec Ts... du constr_pack)
        // je peux délocaliser la logique et créer des erreurs (static_assert) custom
        // pour chaque edge case, ce qui est FUCKING INSANE SÉRIEUX. 
        // REMINDER, C'EST CA QU'IL FAUT PAS QUE TU OUBLIE SI TU PASSE PAR LA ;)

        for (auto const& e : init) {
            std::cout << typeid(decltype(e)).name() << "\n";


            auto entry = json_entry_test{ e };
        }
    }

    /* json_object(std::initializer_list<json_entry> init) {
            std::cout << "json_object(std::initializer_list<json_entry> init)\n";

            entries_.reserve(init.size());
            for (auto const& e : init) {
                std::cout << "+1\n";

                entries_.push_back(e);

            }
        }*/
};
#endif
}

#endif