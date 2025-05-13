#ifndef __UNORDERED_MAP_TPP__
#define __UNORDERED_MAP_TPP__

#include <iostream>
#include "unordered_map.hpp"
/**
 * @brief Constructor de Unordered_Map.
 * Inicializa la tabla hash a nullptr en todas sus posiciones.
 */
template <typename Key, typename Value, size_t TABLE_SIZE>
Unordered_Map<Key, Value, TABLE_SIZE>::Unordered_Map() 
{
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        TABLE[i] = nullptr;
    }
}

/**
 * @brief Destructor de Unordered_Map.
 * Libera la memoria de todos los elementos almacenados en la tabla hash.
 */
template <typename Key, typename Value, size_t TABLE_SIZE>
Unordered_Map<Key, Value, TABLE_SIZE>::~Unordered_Map() 
{
    for (size_t i = 0; i < TABLE_SIZE; ++i) {
        key_value_pair* current = TABLE[i];
        while (current != nullptr) {
            key_value_pair* to_delete = current;
            current = current->next;
            delete to_delete;
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
template <typename Key, typename Value, size_t TABLE_SIZE>
void Unordered_Map<Key, Value, TABLE_SIZE>::insert(const Key& key, const Value& value)
{
    size_t index = hash_fuction(key);
    key_value_pair* current = TABLE[index];

    // Verifica si la clave ya existe
    while (current != nullptr) {
        if (current->key == key) {
            current->value = value; // Actualiza el valor
            return;
        }
        current = current->next;
    }

    // Si no se encontró, agrega un nuevo nodo al principio de la lista
    key_value_pair* new_pair = new key_value_pair(key, value);
    new_pair->next = TABLE[index];
    TABLE[index] = new_pair;
}

/**
 * @brief Busca un valor asociado a una clave en la tabla hash.
 * 
 * @param key Clave para la cual se desea encontrar el valor.
 * @return Valor asociado a la clave o nullptr si no se encuentra.
 */
template <typename Key, typename Value, size_t TABLE_SIZE>
Value* Unordered_Map<Key, Value, TABLE_SIZE>::find(const Key& key) 
{
    size_t index = hash_fuction(key);
    key_value_pair* current = TABLE[index];
    
    while (current != nullptr) {
        if (current->key == key) {
            return &(current->value);
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
template <typename Key, typename Value, size_t TABLE_SIZE>
void Unordered_Map<Key, Value, TABLE_SIZE>::erase(const Key& key) 
{
    size_t index = hash_fuction(key);
    key_value_pair* current = TABLE[index];
    key_value_pair* previous = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (previous == nullptr) {
                // Si no tiene un elemento anterior, significa que es el primer elemento de la lista
                TABLE[index] = current->next;
            } else {
                // Eliminar un elemento intermedio o final
                previous->next = current->next;
            }
            delete current;
            return;
        }
        previous = current;
        current = current->next;
    }
}
#endif
