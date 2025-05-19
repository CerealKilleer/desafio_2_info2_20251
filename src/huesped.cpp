/**
 * @file usuario.cpp
 * @brief Implementación de la clase Huesped.
 * 
 * Esta clase representa a un usuario con sus datos básicos como documento, 
 * contraseña, antigüedad, puntuación y si es anfitrión o no. También proporciona
 * métodos para acceder a estos datos y calcular el tamaño en memoria de un objeto Huesped.
 */

 #include "huesped.hpp"
 #include <stdint.h>
 #include <string>
 #include <cstring>
 #include <iostream>

 #define LOG_SUCCESS(fn, msg) std::cout << "[Huesped/" << fn << "]: " << msg << std::endl
 #define LOG_ERROR(fn, msg) std::cerr << "[Huesped/" << fn << "]: " << msg << std::endl
 /**
  * @brief Constructor de la clase Huesped.
  * 
  * Inicializa un objeto Huesped con los datos proporcionados.
  * 
  * @param documento Documento del usuario (ID o similar).
  * @param password Contraseña del usuario.
  * @param antiguedad Antigüedad del usuario en meses.
  * @param puntuacion Puntuación del usuario (0.0 a 5.0).
  */

/**
 * @def LOG_ERROR(fn, msg)
 * @brief Macro para imprimir errores con el contexto de Alojamiento.
 * @param fn Nombre de la función donde ocurre el error.
 * @param msg Mensaje de error a imprimir.
 */
#define LOG_ERROR(fn, msg) std::cerr << "[Huesped/" << fn << "]: " << msg << std::endl

 Huesped::Huesped(uint64_t documento, char *password, uint8_t antiguedad, float puntuacion) 
     : m_documento(documento),  
       m_antiguedad(antiguedad), 
       m_puntuacion(puntuacion),
       m_password(nullptr),
       m_reservas(nullptr)
 {
        if (password == nullptr) {
            LOG_ERROR("Huesped", "La contraseña es nula");
            m_password = nullptr;
            return;
        }
        size_t len = strlen(password) + 1;
        m_password = new char[len];
        if (m_password == nullptr) {
            LOG_ERROR("Huesped", "Error al asignar memoria para la contraseña");
            return;
        }
        strncpy(m_password, password, len);
        m_password[len - 1] = '\0';
        m_reservas = new Linked_List<Reserva*>();
 };
 
 /**
  * @brief Obtiene el documento del usuario.
  * 
  * @return Referencia constante al documento del usuario.
  */
 uint64_t Huesped::get_documento() const
 { 
     return m_documento; 
 }
 
 /**
  * @brief Obtiene la contraseña del usuario.
  * 
  * @return Referencia constante a la contraseña del usuario.
  */
 const char *Huesped::get_password() const
 { 
     return m_password; 
 }
 
 /**
  * @brief Obtiene la antigüedad del usuario.
  * 
  * @return La antigüedad del usuario en meses.
  */
 uint8_t Huesped::get_antiguedad() const 
 { 
     return m_antiguedad; 
 }
 
 /**
  * @brief Obtiene la puntuación del usuario.
  * 
  * @return La puntuación del usuario.
  */
 float Huesped::get_puntuacion() const 
 { 
     return m_puntuacion; 
 }
 

 /**
  * @brief Calcula el tamaño total en memoria de un objeto Huesped.
  * 
  * Calcula el tamaño total de un objeto Huesped, incluyendo la memoria ocupada
  * por los strings `m_documento` y `m_password`. Esta función no garantiza
  * el tamaño exacto debido a la naturaleza de las implementaciones de std::string.
  * 
  * @return El tamaño total en bytes ocupado por el objeto Huesped.
  */
 size_t Huesped::get_obj_size() const 
 { 
    size_t total_size = sizeof(Huesped); 
    total_size += strlen(m_password) + 1;
 
    return total_size;
 }


/**
 * @brief Muestra los alojamientos del anfitrion.
 * 
 * Elimina una reserva que esté asociada a un alojamiento del anfitrion.
 */

bool Huesped::eliminar_reserva(Reserva* reserva)
{
    size_t cnt = 0;
    if (reserva == nullptr) {
        LOG_ERROR("eliminar_reserva", "La reserva es nula");
        return false;
    }
    if (m_reservas->remove(reserva)) {
        LOG_SUCCESS("eliminar_reserva", "Reserva eliminada con éxito");
        return true;
    } else {
        LOG_ERROR("eliminar_reserva", "No se pudo eliminar la reserva");
        return false;
    }
}


/**
 * @brief Establece una reserva para el huesped.
 * 
 * @param reserva Reserva a establecer.
 * @return Puntero a la reserva establecida.
 */
void Huesped::set_reserva(Reserva* reserva) 
{
    if (reserva == nullptr) {
        LOG_ERROR("set_reserva", "La reserva es nula");
        return;
    }
    m_reservas->insert_front(reserva);
}


/**
* @brief Destructor de la clase Huesped.
* 
* Libera los recursos ocupados por el objeto Huesped.
*/
Huesped::~Huesped() 
{
    delete[] m_password;
    delete m_reservas;
    std::cout << "Huesped: " << m_documento << " destruido" << std::endl;
}
 