/**
 * @file linked_list.tpp
 * @brief Implementación de la clase Linked_List y Node.
 */
#ifndef __LINKED_LIST_TPP__
#define __LINKED_LIST_TPP__
#include "linked_list.hpp"
#include "performance.hpp"

template <typename T>
Node<T>::Node(T value) : data(value), next(nullptr) {}

template <typename T>
Linked_List<T>::Linked_List() : head(nullptr), size(0) {}

/**
 * @brief Destructor de la lista enlazada.
 * Elimina todos los nodos, pero no los datos apuntados por ellos.
 */
template <typename T>
Linked_List<T>::~Linked_List() {
    Node<T>* current = head;
    while (current) {
        Node<T>* next = current->next;
        g_ciclos++;
        g_tamano -= sizeof(Node<T>);
        delete current;  // Solo elimina los nodos, no los datos apuntados
        current = next;
    }
}

/**
 * @brief Inserta un nuevo nodo al frente de la lista.
 * @param value Dato a insertar.
 */
template <typename T>
void Linked_List<T>::insert_front(T value) {
    Node<T>* new_node = new Node<T>(value);
    g_tamano += sizeof(Node<T>);
    new_node->next = head;
    head = new_node;
    size++;
}

/**
 * @brief Remueve el primer nodo que contiene el dato especificado.
 * @param value Dato a eliminar (comparación por puntero).
 */
template <typename T>
bool Linked_List<T>::remove(T value) {
    /**
     * @brief Elimina el primer nodo que contiene el dato especificado.
     * @param value Dato a eliminar (comparación por puntero).
     */
    if (!head) 
        return true;

    if (head->data == value) {
        Node<T>* temp = head;
        head = head->next;
        g_tamano -= sizeof(Node<T>);
        delete temp;
        size--;
        return true;
    }

    Node<T>* current = head;
    while (current->next && current->next->data != value) {
        current = current->next;
        g_ciclos++;
    }

    if (current->next) {
        Node<T>* temp = current->next;
        current->next = current->next->next;
        size--;
        g_tamano -= sizeof(Node<T>);
        delete temp;
        return true;
    }
    return false;  // No se encontró el valor
}

/**
 * @brief Verifica si la lista está vacía.
 */
template <typename T>
bool Linked_List<T>::is_empty() const {
    return head == nullptr;
}

/**
 * @brief Elimina los objetos apuntados por los datos de cada nodo.
 * 
 * @warning Solo debe usarse si T es un puntero.
 */
template <typename T>
void Linked_List<T>::clear_data() {
    Node<T>* current = head;
    while (current) {
        // Elimina el objeto apuntado por data
        g_ciclos++;
        delete current->data;
        current->data = nullptr;    
        current = current->next;
    }
}

/**
 * @brief retorna el primer nodo de la lista.
 */
template <typename T>
Node<T>* Linked_List<T>::get_head() const {
    return head;
}

/**
 * @brief retorna el siguiente nodo de un nodo dado.
 * @param node Nodo actual.
 * @return Puntero al siguiente nodo, o nullptr si no hay más.
 */
template <typename T>
Node<T>* Linked_List<T>::get_next(Node<T>* node) const {
    return node ? node->next : nullptr;
}

/**
 * @brief Obtiene el tamaño de la lista.
 */
template <typename T>
uint32_t Linked_List<T>::get_size() const {
    return size;
}

/**
 * @brief Inserta un nuevo elemento en la lista de forma ordenada.
 */
template <typename T>
void Linked_List<T>::insert_sorted(T nuevo_elemento, bool (*comparador)(T, T)) 
{
    /**
    * @brief Inserta un nuevo elemento en la lista de forma ordenada.
    */
    Node<T>* nuevo = new Node<T>(nuevo_elemento);
    g_tamano += sizeof(nuevo->data) + sizeof(nuevo->next);
    // Si la lista está vacía o el nuevo elemento es menor que el primero
    if (!head || comparador(nuevo_elemento, head->data)) {
        nuevo->next = head;
        head = nuevo;
        size++;
        return;
    }

    // Busca la posición correcta para insertar el nuevo elemento
    Node<T>* actual = head;
    while (actual->next && !comparador(nuevo_elemento, actual->next->data)) {
        actual = actual->next;
        g_ciclos++;
    }

    nuevo->next = actual->next;
    actual->next = nuevo;
    size++;
}

#endif