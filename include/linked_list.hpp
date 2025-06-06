#ifndef LINKED_LIST_HPP
#define LINKED_LIST_HPP

#include <iostream>
#include <stdint.h>
/**
 * @brief Nodo de la lista enlazada.
 * 
 * @tparam T Tipo de dato almacenado.
 */
template <typename T>
class Node {
public:
    T data;        ///< Dato almacenado.
    Node* next;    ///< Puntero al siguiente nodo.

    /**
     * @brief Constructor del nodo.
     * @param value Dato a almacenar.
     */
    Node(T value);
};

/**
 * @brief Lista enlazada simple que almacena punteros.
 * 
 * @tparam T Tipo de dato almacenado (puntero).
 */
template <typename T>
class Linked_List {
private:
    uint32_t size;
    Node<T>* head;  ///< Puntero al primer nodo.
public:
    /**
     * @brief Constructor por defecto.
     */
    Linked_List();

    /**
     * @brief Destructor. Elimina los nodos, pero no los datos apuntados.
     */
    ~Linked_List();

    /**
     * @brief Inserta un nuevo nodo al frente.
     * @param value Dato a insertar.
     */
    void insert_front(T value);

    /**
     * @brief Elimina el primer nodo que contiene el dato especificado.
     * @param value Dato a eliminar (comparación por puntero).
     */
    bool remove(T value);

    /**
     * @brief Verifica si la lista está vacía.
     * @return true si está vacía, false en caso contrario.
     */
    bool is_empty() const;

    /**
     * @brief Elimina los objetos apuntados por los datos de cada nodo.
     * 
     * @warning Solo debe usarse si T es un puntero.
     */
    void clear_data();

    /**
     * @brief Devuelve el primer nodo de la lista.
     * @return Puntero al nodo inicial, o nullptr si la lista está vacía.
     */
    Node<T>* get_head() const;

    /**
     * @brief Devuelve el siguiente nodo de un nodo dado.
     * @param node Nodo actual.
     * @return Puntero al siguiente nodo, o nullptr si no hay más.
     */
    Node<T>* get_next(Node<T>* node) const;

    /**
     * @brief Ordena la lista usando el algoritmo merge sort.
     */
    void sort();

    /**
     * @brief muestra el tamaño de la lista
     * @return uint32_t tamaño de la lista
     */
    uint32_t get_size() const;

    /**
     * @brief Inserta un nuevo elemento en la lista de forma ordenada.
     * @param nuevo_elemento Elemento a insertar.
     * @param comparador Función de comparación para determinar el orden.
     */
    void insert_sorted(T nuevo_elemento, bool (*comparador)(T, T));
};

#include "linked_list.tpp"

#endif
