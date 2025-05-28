/**
 * @file anfitrion.cpp
 * @brief Implementación de la clase Anfitrion.
 * 
 * Esta clase representa a un anfitrion con sus datos básicos como documento, 
 * contraseña, antigüedad, puntuación y si es anfitrión o no. También proporciona
 * métodos para acceder a estos datos y calcular el tamaño en memoria de un objeto Anfitrion.
 */

 #include "anfitrion.hpp"
 #include "performance.hpp"
 #include <stdint.h>
 #include <string>
 #include <cstring>
 #include <iostream>
 /**
  * @brief Constructor de la clase Anfitrion.
  * 
  * Inicializa un objeto Anfitrion con los datos proporcionados.
  * 
  * @param documento Documento del anfitrion (ID o similar).
  * @param password Contraseña del anfitrion.
  * @param antiguedad Antigüedad del anfitrion en meses.
  * @param puntuacion Puntuación del anfitrion (0.0 a 5.0).
  */

/**
 * @def LOG_ERROR(fn, msg)
 * @brief Macro para imprimir errores con el contexto de Alojamiento.
 * @param fn Nombre de la función donde ocurre el error.
 * @param msg Mensaje de error a imprimir.
 */
#define LOG_ERROR(fn, msg) std::cerr << "[Anfitrion/" << fn << "]: " << msg << std::endl
#define LOG_SUCCESS(fn, msg) std::cout << "[Anfitrion/" << fn << "]: " << msg << std::endl

 Anfitrion::Anfitrion(uint64_t documento, char *password, uint16_t antiguedad, float puntuacion) 
     : m_documento(documento),  
       m_antiguedad(antiguedad), 
       m_puntuacion(puntuacion),
       m_password(nullptr),
       m_alojamientos(nullptr)
 {
        if (password == nullptr) {
            LOG_ERROR("Anfitrion", "La contraseña es nula");
            m_password = nullptr;
            return;
        }
        size_t len = strlen(password) + 1;
        m_password = new char[len];
        if (m_password == nullptr) {
            LOG_ERROR("Anfitrion", "Error al asignar memoria para la contraseña");
            return;
        }
        memcpy(m_password, password, len);
        m_password[len - 1] = '\0';
        m_alojamientos = new Linked_List<Alojamiento*>();
        g_strlen_cnt++; // Contador de strlen
        g_memcpy_cnt++; // Contador de memcpy
 };
 
 /**
  * @brief Obtiene el documento del anfitrion.
  * 
  * @return Referencia constante al documento del anfitrion.
  */
 uint64_t Anfitrion::get_documento() const
 { 
     return m_documento; 
 }
 
 /**
  * @brief Obtiene la contraseña del anfitrion.
  * 
  * @return Referencia constante a la contraseña del anfitrion.
  */
 const char *Anfitrion::get_password() const
 { 
     return m_password; 
 }
 
 /**
  * @brief Obtiene la antigüedad del anfitrion.
  * 
  * @return La antigüedad del anfitrion en meses.
  */
 uint16_t Anfitrion::get_antiguedad() const 
 { 
     return m_antiguedad; 
 }
 
 /**
  * @brief Obtiene la puntuación del anfitrion.
  * 
  * @return La puntuación del anfitrion.
  */
 float Anfitrion::get_puntuacion() const 
 { 
     return m_puntuacion; 
 }
 

 /**
  * @brief Calcula el tamaño total en memoria de un objeto Anfitrion.
  * 
  * Calcula el tamaño total de un objeto Anfitrion, incluyendo la memoria ocupada
  * por los strings `m_documento` y `m_password`. Esta función no garantiza
  * el tamaño exacto debido a la naturaleza de las implementaciones de std::string.
  * 
  * @return El tamaño total en bytes ocupado por el objeto Anfitrion.
  */
 size_t Anfitrion::get_obj_size() const 
 { 
    size_t total_size = sizeof(*this); 
    total_size += strlen(m_password) + 1;
    g_strlen_cnt++; // Contador de strlen
    return total_size;
 }

/**
 * @brief Agrega un alojamiento a la lista de alojamientos del anfitrion.
 * 
 * @param alojamiento Puntero al objeto Alojamiento a agregar.
 * @return Alojamiento* Puntero al alojamiento agregado, o nullptr si hubo error.
 */

Alojamiento* Anfitrion::set_alojamiento(Alojamiento* alojamiento)
{
    if (alojamiento == nullptr) {
        LOG_ERROR("set_alojamiento", "El alojamiento es nulo");
        return nullptr;
    }

    m_alojamientos->insert_front(alojamiento);
    return alojamiento;
}

/**
 * @brief Muestra los alojamientos del anfitrion.
 * 
 * Elimina una reserva que esté asociada a un alojamiento del anfitrion.
 */

bool Anfitrion::eliminar_reserva(Reserva* reserva)
{
    if (reserva == nullptr) {
        LOG_ERROR("eliminar_reserva", "La reserva es nula");
        return false;
    }
    uint32_t codigo_alojamiento = reserva->get_codigo_alojamiento();
    Node<Alojamiento*>* current = m_alojamientos->get_head();

    while (current != nullptr) {
        g_ciclos++;
        Alojamiento* alojamiento = current->data;
        if (alojamiento->get_id() == codigo_alojamiento) {
            if (alojamiento->eliminar_reserva(reserva->get_codigo_reserva())) {
                LOG_SUCCESS("eliminar_reserva", "Reserva eliminada con éxito");
            } else {
                LOG_ERROR("eliminar_reserva", "No se pudo eliminar la reserva");
            }
            reserva = nullptr;
            break;
        }
        current = m_alojamientos->get_next(current);
    }
    return true;
}
/**
 * @brief Muestra los alojamientos del anfitrion activos en un intervalo de fechas.
 * 
 * Recorre la lista de alojamientos y muestra la información de cada uno.
 * 
 * @param desde Fecha de inicio para filtrar reservas.
 * @param hasta Fecha de fin para filtrar reservas.
 */
void Anfitrion::mostrar_alojamientos(Fecha &desde, Fecha &hasta) const
{
    if (m_alojamientos == nullptr) {
        std::cout << "No hay alojamientos disponibles." << std::endl;
        return;
    }

    Node<Alojamiento*>* current = m_alojamientos->get_head();
    while (current != nullptr) {
        Alojamiento* alojamiento = current->data;
        alojamiento->mostrar_reservas(desde, hasta);
        std::cout << "------------------------" << std::endl;
        current = m_alojamientos->get_next(current);
        g_ciclos++;
    }
}

/**
* @brief Destructor de la clase Anfitrion.
* 
* Libera los recursos ocupados por el objeto Anfitrion.
*/

Anfitrion::~Anfitrion() 
{
    g_tamano -= this->get_obj_size();
    delete[] m_password;
    delete m_alojamientos;
    m_alojamientos = nullptr;
    m_password = nullptr;
}
 