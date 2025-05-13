#ifndef __UNORDERED_MAP_HPP__
#define __UNORDERED_MAP_HPP__

#include <cstddef>
#include <stdint.h>

/**
 * @brief Implementación de un mapa no ordenado (hash map).
 * 
 * Este mapa utiliza una tabla hash con resolución de colisiones por encadenamiento.
 * Los elementos se almacenan en listas enlazadas dentro de cada índice de la tabla.
 * 
 * @tparam Key Tipo de la clave.
 * @tparam Value Tipo del valor asociado a cada clave.
 * @tparam TABLE_SIZE Tamaño de la tabla hash (por defecto 101).
 */
template <typename Key, typename Value, size_t TABLE_SIZE = 101>
class Unordered_Map {
    private:
        /**
         * @brief Estructura interna que almacena una clave, su valor asociado y un puntero al siguiente par clave-valor.
         */
        struct key_value_pair {
            Key key;            ///< Clave del par clave-valor.
            Value value;        ///< Valor asociado a la clave.
            key_value_pair* next; ///< Puntero al siguiente par clave-valor en caso de colisión.

            /**
             * @brief Constructor de la estructura key_value_pair.
             * 
             * @param k Clave del par clave-valor.
             * @param v Valor asociado a la clave.
             */
            key_value_pair(const Key& k, const Value& v) : key(k), value(v), next(nullptr) {}
        };

        key_value_pair *TABLE[TABLE_SIZE]; ///< Tabla hash que contiene los pares clave-valor.

        /**
         * @brief Función hash que calcula el índice para una clave utilizando el algoritmo djb2.
         * 
         * @param key La clave para la cual se calculará el índice.
         * @return El índice calculado en la tabla hash.
         */
        size_t hash_fuction(const Key& key) const {
            size_t hash_value = 5381; ///< Valor inicial del hash (un número primo utilizado como semilla).
            const uint8_t* key_bytes = reinterpret_cast<const uint8_t*>(&key);

            for (size_t i = 0; i < sizeof(Key); ++i) {
                hash_value = ((hash_value << 5) + hash_value) + key_bytes[i]; // Fórmula djb2: hash * 33 + c
            }

            return hash_value % TABLE_SIZE; // Retorna el índice en la tabla
        }

    public:
        /**
         * @brief Constructor de Unordered_Map.
         * Inicializa la tabla hash, estableciendo todos los punteros a nullptr.
         */
        Unordered_Map();

        /**
         * @brief Destructor de Unordered_Map.
         * Libera la memoria ocupada por los elementos almacenados en la tabla hash.
         */
        ~Unordered_Map();

        /**
         * @brief Inserta un nuevo par clave-valor en la tabla hash.
         * Si la clave ya existe, actualiza su valor.
         * 
         * @param key Clave que se desea insertar.
         * @param value Valor asociado a la clave.
         */
        void insert(const Key& key, const Value& value);

        /**
         * @brief Busca un valor asociado a una clave en la tabla hash.
         * 
         * @param key La clave para la cual se desea encontrar el valor.
         * @return Un puntero al valor asociado a la clave, o nullptr si no se encuentra.
         */
        Value* find(const Key& key);

        /**
         * @brief Elimina un par clave-valor de la tabla hash.
         * 
         * @param key Clave del elemento que se desea eliminar.
         */
        void erase(const Key& key);

};

#include "unordered_map.tpp"
#endif
