#ifndef __UNORDERED_MAP_TPP__
#define __UNORDERED_MAP_TPP__

#include <iostream>
#include "unordered_map.hpp"
#define RESIZE(size) ((size/(0.75)) + 1) //Ya que no hacemos rehashing, se redimensiona la tabla hash al 75% de su tamaño original.

/**
 * @brief Constructor de Unordered_Map.
 * Inicializa la tabla hash a nullptr en todas sus posiciones.
 */
template <typename Key, typename Value>
Unordered_Map<Key, Value>::Unordered_Map(size_t size)
    : m_size(RESIZE(size)),
      m_count(0)
{
    m_table = new key_value_pair*[m_size];
    if (m_table == nullptr) {
        throw std::bad_alloc();
        return;
    }

    // Inicializa la tabla hash a nullptr
    for (size_t i = 0; i < m_size; ++i) {
        m_table[i] = nullptr;
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
            delete to_delete->value; // Libera el valor
            delete to_delete;
        }
    }
    delete[] m_table;
    m_table = nullptr;
    std::cout << "Mapa destruido" << std::endl;
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
            return;
        }
        current = current->next;
    }

    // Si no se encontró, agrega un nuevo nodo al principio de la lista
    key_value_pair* new_pair = new key_value_pair(key, value);
    new_pair->next = m_table[index];
    m_table[index] = new_pair;
    m_count++;
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
            return current->value;
        }
        current = current->next;
    }
    
    return nullptr;
}

/**
 * @brief Elimina un par clave-valor de la tabla hash.
 * Si la clave no existe, no realiza ninguna acción.
 * 
 * @param key Clave del elemento que se desea eliminar.
 */
template <typename Key, typename Value>
void Unordered_Map<Key, Value>::erase(const Key& key) 
{
    size_t index = hash_fuction(key);
    key_value_pair* current = m_table[index];
    key_value_pair* previous = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (previous == nullptr) {
                // Si no tiene un elemento anterior, significa que es el primer elemento de la lista
                m_table[index] = current->next;
            } else {
                // Eliminar un elemento intermedio o final
                previous->next = current->next;
            }
            delete current->value; // Libera el valor
            delete current;
            m_count--;
            return;
        }
        previous = current;
        current = current->next;
    }
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
    for (size_t i = 0; i < sizeof(Key); ++i)
        hash_value = ((hash_value << 5) + hash_value) + key_bytes[i]; // Fórmula djb2: hash * 33 + c
    
    return hash_value % m_size; // Retorna el índice en la tabla
}

template <typename Key, typename Value>

size_t Unordered_Map<Key, Value>::info_map() const
{
    size_t total = 0;

    // Tabla de punteros
    total += sizeof(key_value_pair*) * m_size;

    // Nodos key_value_pair + Objetos Value
    for (size_t i = 0; i < m_size; ++i) {
        key_value_pair* current = m_table[i];
        while (current != nullptr) {
            total += sizeof(key_value_pair); // Nodo
            total += sizeof(Value);          // Objeto apuntado
            current = current->next;
        }
    }

    total += sizeof(m_size);
    total += sizeof(m_count);

    return total;
}

#endif
