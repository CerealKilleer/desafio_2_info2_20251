#ifndef __UNORDERED_MAP_TPP__
#define __UNORDERED_MAP_TPP__

#include <iostream>
#include "unordered_map.hpp"
#include "performance.hpp"
#define RESIZE(size) ((size/(0.75)) + 1) //Ya que no hacemos rehashing, se redimensiona la tabla hash al 75% de su tamaño original.

/**
 * @brief Constructor de Unordered_Map.
 * Inicializa la tabla hash a nullptr en todas sus posiciones.
 */
template <typename Key, typename Value>
Unordered_Map<Key, Value>::Unordered_Map(size_t size)
    : m_size(RESIZE(size))
{
    m_table = new key_value_pair*[m_size];
    g_tamano += sizeof(key_value_pair*) * m_size;
    if (m_table == nullptr) {
        throw std::bad_alloc();
        return;
    }

    // Inicializa la tabla hash a nullptr
    for (size_t i = 0; i < m_size; ++i) {
        m_table[i] = nullptr;
        g_ciclos++;
    }
}

/**
 * @brief Destructor de Unordered_Map.
 * Libera la memoria de todos los elementos almacenados en la tabla hash.
 */
template <typename Key, typename Value>
Unordered_Map<Key, Value>::~Unordered_Map() 
{
    for (size_t i = 0; i < m_size; ++i) {
        key_value_pair* current = m_table[i];
        while (current != nullptr) {
            key_value_pair* to_delete = current;
            current = current->next;
            g_ciclos++;
            delete to_delete;
        }
    }
    delete[] m_table;
    m_table = nullptr;
    std::cout << "Mapa destruido" << std::endl;
}

/**
 * @brief Libera la memoria ocupada (si la hay) por los valores de la tabla hash.
 * Es responsabilidad del llamador liberar la memoria ocupada por los valores.
 */
template <typename Key, typename Value>
void Unordered_Map<Key, Value>::clear_values() 
{
    for (size_t i = 0; i < m_size; ++i) {
        key_value_pair* current = m_table[i];
        while (current != nullptr) {
            delete current->value;
            current->value = nullptr;
            current = current->next;
            g_ciclos++;
        }
    }
}

/**
 * @brief Inserta un nuevo par clave-valor en la tabla hash.
 * Si la clave ya existe, actualiza su valor, si hay colisión, se maneja mediante encadenamiento.
 * 
 * @param key Clave que se desea insertar.
 * @param value Valor asociado a la clave.
 */
template <typename Key, typename Value>
void Unordered_Map<Key, Value>::insert(const Key& key, Value *value)
{
    size_t index = hash_fuction(key);
    key_value_pair* current = m_table[index];

    // Verifica si la clave ya existe
    while (current != nullptr) {
        if (current->key == key) {
            delete current->value; // Libera el valor anterior
            current->value = value; // Asigna el nuevo valor
            g_ciclos++;
            return;
        }
        current = current->next;
        g_ciclos++;
    }

    // Si no se encontró, agrega un nuevo nodo al principio de la lista
    key_value_pair* new_pair = new key_value_pair(key, value);
    new_pair->next = m_table[index];
    m_table[index] = new_pair;
}

/**
 * @brief Busca un valor asociado a una clave en la tabla hash.
 * 
 * @param key Clave para la cual se desea encontrar el valor.
 * @return Valor asociado a la clave o nullptr si no se encuentra.
 */
template <typename Key, typename Value>
Value* Unordered_Map<Key, Value>::find(const Key& key) 
{
    size_t index = hash_fuction(key);
    key_value_pair* current = m_table[index];
    
    while (current != nullptr) {
        if (current->key == key) {
            g_ciclos++;
            return current->value;
        }
        current = current->next;
        g_ciclos++;
    }
    
    return nullptr;
}

/**
 * @brief Elimina un par clave-valor de la tabla hash.
 * Si la clave no existe, no realiza ninguna acción.
 * 
 * @param key Clave del elemento que se desea eliminar.
 * @return Un puntero al valor eliminado o nullptr si no se encuentra. Será liberado por el llamador.
 */
 
template <typename Key, typename Value>
Value *Unordered_Map<Key, Value>::erase(const Key& key) 
{
    size_t index = hash_fuction(key);
    key_value_pair* current = m_table[index];
    key_value_pair* previous = nullptr;
    Value *value = nullptr; // Inicializa el puntero a nullptr
    while (current != nullptr) {
        g_ciclos++;
        if (current->key == key) {
            if (previous == nullptr) {
                // Si no tiene un elemento anterior, significa que es el primer elemento de la lista
                m_table[index] = current->next;
            } else {
                // Eliminar un elemento intermedio o final
                previous->next = current->next;
            }
            value = current->value; // Guarda el valor antes de eliminar
            g_tamano -= sizeof(value);
            delete current;
            return value;
        }
        previous = current;
        current = current->next;
    }
    return nullptr; // Si no se encontró la clave, retorna nullptr
}

/**
* @brief Función hash que calcula el índice para una clave utilizando el algoritmo djb2.
* 
* @param key La clave para la cual se calculará el índice.
* @return El índice calculado en la tabla hash.
*/
template <typename Key, typename Value>
size_t Unordered_Map<Key, Value>::hash_fuction(const Key& key) const 
{
    size_t hash_value = 5381; ///< Valor inicial del hash (un número primo utilizado como semilla).

    const uint8_t* key_bytes = reinterpret_cast<const uint8_t*>(&key);
    for (size_t i = 0; i < sizeof(Key); ++i, g_ciclos++)
        hash_value = ((hash_value << 5) + hash_value) + key_bytes[i]; // Fórmula djb2: hash * 33 + c
    
    return hash_value % m_size; // Retorna el índice en la tabla
}

template <typename Key, typename Value>

size_t Unordered_Map<Key, Value>::info_map() const
{
    return sizeof(*this);
}

template <typename Key, typename Value>
void Unordered_Map<Key, Value>::for_each(void (*callback)(Key, Value*, void*), void *data) 
{
    for (size_t i = 0; i < m_size; ++i) {
        key_value_pair* current = m_table[i];
        while (current != nullptr) {
            callback(current->key, current->value, data);
            current = current->next;
            g_ciclos++;
        }
    }
}
#endif
