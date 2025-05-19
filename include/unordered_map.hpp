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
 */
template <typename Key, typename Value>
class Unordered_Map {
    private:
        /**
         * @brief Estructura interna que almacena una clave, su valor asociado y un puntero al siguiente par clave-valor.
         */
        struct key_value_pair {
            Key key;            ///< Clave del par clave-valor.
            Value *value;        ///< Recibe puntero al valor asociado a la clave.
            key_value_pair* next; ///< Puntero al siguiente par clave-valor en caso de colisión.

            /**
             * @brief Constructor de la estructura key_value_pair.
             * 
             * @param k Clave del par clave-valor.
             * @param v Valor asociado a la clave.
             */
            key_value_pair(const Key& k, Value *v) : key(k), value(v), next(nullptr) {}
        };

        key_value_pair **m_table; ///< Tabla hash que contiene los pares clave-valor.
        size_t m_size; ///< Tamaño actual de la tabla hash.
        size_t m_count; ///< Contador de elementos en la tabla hash.
        /**
         * @brief Función hash que calcula el índice para una clave utilizando el algoritmo djb2.
         * 
         * @param key La clave para la cual se calculará el índice.
         * @return El índice calculado en la tabla hash.
         */
        size_t hash_fuction(const Key& key) const;

    public:
        Unordered_Map(const Unordered_Map&) = delete; ///< Elimina el constructor de copia.
        Unordered_Map& operator=(const Unordered_Map&) = delete; ///< Elimina el operador de asignación.
        /**
         * @brief Constructor de Unordered_Map.
         * Inicializa la tabla hash, estableciendo todos los punteros a nullptr.
         * @param size Tamaño de la tabla hash.
         */
        Unordered_Map(size_t size);
        
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
        void insert(const Key& key, Value *value);

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
         * @return Un puntero al valor eliminado o nullptr si no se encuentra. Será liberado por el llamador.
         */
        Value *erase(const Key& key);

        /**
         * @brief Devuelve un aproximado del tamaño en memoria de la tabla hash.
         * 
         */
        size_t info_map() const;

        /**
         * @brief Limpia la memoria ocupada por los valores de la tabla hash.
         * 
         * Esta función libera la memoria ocupada (dinámica) por los valores almacenados en la tabla hash.
         * 
         */
        void clear_values();

        /**
         * @brief Recorre la tabla hash y aplica una función a cada par clave-valor.
         * 
         * @param callback Función que se aplicará a cada par clave-valor.
         * @param data Datos adicionales que se pasarán a la función callback.
         * @return El número de elementos procesados.
         */
        size_t for_each(void (*callback)(Key, Value*, void*), void *data);
};

#include "unordered_map.tpp"
#endif
