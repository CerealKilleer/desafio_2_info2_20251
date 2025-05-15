/**
 * @file usuario.cpp
 * @brief Implementación de la clase Usuario.
 * 
 * Esta clase representa a un usuario con sus datos básicos como documento, 
 * contraseña, antigüedad, puntuación y si es anfitrión o no. También proporciona
 * métodos para acceder a estos datos y calcular el tamaño en memoria de un objeto Usuario.
 */

 #include "usuario.hpp"
 #include <stdint.h>
 #include <string>
 #include <cstring>
 #include <iostream>
 /**
  * @brief Constructor de la clase Usuario.
  * 
  * Inicializa un objeto Usuario con los datos proporcionados.
  * 
  * @param documento Documento del usuario (ID o similar).
  * @param password Contraseña del usuario.
  * @param antiguedad Antigüedad del usuario en meses.
  * @param puntuacion Puntuación del usuario (0.0 a 5.0).
  * @param anfitrion Indica si el usuario es un anfitrión o no.
  */

/**
 * @def LOG_ERROR(fn, msg)
 * @brief Macro para imprimir errores con el contexto de Alojamiento.
 * @param fn Nombre de la función donde ocurre el error.
 * @param msg Mensaje de error a imprimir.
 */
#define LOG_ERROR(fn, msg) std::cerr << "[Usuario/" << fn << "]: " << msg << std::endl

 Usuario::Usuario(uint64_t documento, char *password, uint16_t antiguedad, float puntuacion, bool anfitrion) 
     : m_documento(documento),  
       m_antiguedad(antiguedad), 
       m_puntuacion(puntuacion), 
       m_anfitrion(anfitrion),
       m_password(nullptr)
 {
        if (password == nullptr) {
            LOG_ERROR("Usuario", "La contraseña es nula");
            m_password = nullptr;
            return;
        }
        size_t len = strlen(password) + 1;
        m_password = new char[len];
        if (m_password == nullptr) {
            LOG_ERROR("Usuario", "Error al asignar memoria para la contraseña");
            return;
        }
        strncpy(m_password, password, len);
        m_password[len - 1] = '\0';
 };
 
 /**
  * @brief Obtiene el documento del usuario.
  * 
  * @return Referencia constante al documento del usuario.
  */
 uint64_t Usuario::get_documento() const
 { 
     return m_documento; 
 }
 
 /**
  * @brief Obtiene la contraseña del usuario.
  * 
  * @return Referencia constante a la contraseña del usuario.
  */
 const char *Usuario::get_password() const
 { 
     return m_password; 
 }
 
 /**
  * @brief Obtiene la antigüedad del usuario.
  * 
  * @return La antigüedad del usuario en meses.
  */
 uint16_t Usuario::get_antiguedad() const 
 { 
     return m_antiguedad; 
 }
 
 /**
  * @brief Obtiene la puntuación del usuario.
  * 
  * @return La puntuación del usuario.
  */
 float Usuario::get_puntuacion() const 
 { 
     return m_puntuacion; 
 }
 
 /**
  * @brief Obtiene si el usuario es anfitrión.
  * 
  * @return `true` si el usuario es anfitrión, `false` en caso contrario.
  */
 bool Usuario::get_anfitrion() const 
 { 
     return m_anfitrion; 
 }
 
 /**
  * @brief Calcula el tamaño total en memoria de un objeto Usuario.
  * 
  * Calcula el tamaño total de un objeto Usuario, incluyendo la memoria ocupada
  * por los strings `m_documento` y `m_password`. Esta función no garantiza
  * el tamaño exacto debido a la naturaleza de las implementaciones de std::string.
  * 
  * @return El tamaño total en bytes ocupado por el objeto Usuario.
  */
 size_t Usuario::get_obj_size() const 
 { 
    size_t total_size = sizeof(Usuario); 
    total_size += strlen(m_password) + 1;
 
    return total_size;
 }

/**
* @brief Destructor de la clase Usuario.
* 
* Libera los recursos ocupados por el objeto Usuario.
*/

Usuario::~Usuario() 
{
    delete[] m_password;
    std::cout << "Usuario: " << m_documento << " destruido" << std::endl;
}
 