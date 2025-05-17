#ifndef __LINKED_LIST_TPP__
#define __LINKED_LIST_TPP__
#include "linked_list.hpp"

template <typename T>
Node<T>::Node(T value) : data(value), next(nullptr) {}

template <typename T>
Linked_List<T>::Linked_List() : head(nullptr) {}

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
}

template <typename T>
void Linked_List<T>::insert_back(T value) {
    Node<T>* new_node = new Node<T>(value);
    if (!head) {
        head = new_node;
        return;
    }

    Node<T>* current = head;
    while (current->next) {
        current = current->next;
    }
    current->next = new_node;
}

template <typename T>
void Linked_List<T>::remove(T value) {
    if (!head) return;

    if (head->data == value) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
        return;
    }

    Node<T>* current = head;
    while (current->next && current->next->data != value) {
        current = current->next;
    }

    if (current->next) {
        Node<T>* temp = current->next;
        current->next = current->next->next;
        delete temp;
    }
}

template <typename T>
bool Linked_List<T>::is_empty() const {
    return head == nullptr;
}

template <typename T>
void Linked_List<T>::clear_data() {
    Node<T>* current = head;
    while (current) {
        delete current->data;       // Elimina el objeto apuntado
        current->data = nullptr;    // Limpieza defensiva
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
#endif