/**
 * @file linked_list.tpp
 * @brief Implementación de la clase Linked_List y Node.
 */
#ifndef __LINKED_LIST_TPP__
#define __LINKED_LIST_TPP__
#include "linked_list.hpp"

template <typename T>
Node<T>::Node(T value) : data(value), next(nullptr) {}

template <typename T>
Linked_List<T>::Linked_List() : head(nullptr), size(0) {}

template <typename T>
Linked_List<T>::~Linked_List() {
    Node<T>* current = head;
    while (current) {
        Node<T>* next = current->next;
        delete current;  // Solo elimina los nodos, no los datos apuntados
        current = next;
    }
}

template <typename T>
void Linked_List<T>::insert_front(T value) {
    Node<T>* new_node = new Node<T>(value);
    new_node->next = head;
    head = new_node;
    size++;
}

template <typename T>
bool Linked_List<T>::remove(T value) {
    /**
     * @brief Elimina el primer nodo que contiene el dato especificado.
     * @param value Dato a eliminar (comparación por puntero).
     */
    if (!head) return true;

    if (head->data == value) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        size--;
        return true;
    }

    Node<T>* current = head;
    while (current->next && current->next->data != value) {
        current = current->next;
    }

    if (current->next) {
        Node<T>* temp = current->next;
        current->next = current->next->next;
        size--;
        delete temp;
        return true;
    }
    return false;  // No se encontró el valor
}

template <typename T>
bool Linked_List<T>::is_empty() const {
    return head == nullptr;
}

template <typename T>
void Linked_List<T>::clear_data() {
    Node<T>* current = head;
    while (current) {
        // Elimina el objeto apuntado por data
        delete current->data;
        current->data = nullptr;    
        current = current->next;
    }
}

template <typename T>
Node<T>* Linked_List<T>::get_head() const {
    return head;
}

template <typename T>
Node<T>* Linked_List<T>::get_next(Node<T>* node) const {
    return node ? node->next : nullptr;
}

template <typename T>
uint32_t Linked_List<T>::get_size() const {
    return size;
}


template <typename T>
void Linked_List<T>::insert_sorted(T nuevo_elemento, bool (*comparador)(T, T)) 
{
    /**
    * @brief Inserta un nuevo elemento en la lista de forma ordenada.
    */
    Node<T>* nuevo = new Node<T>(nuevo_elemento);

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
    }

    nuevo->next = actual->next;
    actual->next = nuevo;
    size++;
}

#endif