#ifndef __USUARIO_HPP__
#define __USUARIO_HPP__
#include <stdint.h>
#include <string>

/**
 * @class Usuario
 * @brief Clase que representa a un usuario con información personal y de acceso.
 * 
 * Esta clase contiene los datos relacionados con un usuario como el documento de identidad, 
 * la contraseña, la antigüedad en meses, la puntuación y si es anfitrión o no.
 */
class Usuario
{
    public:

        /**
         * @brief Constructor de copia de la clase Usuario.
         * 
         * Crea un nuevo objeto Usuario como copia de otro existente.
         * 
         * @param other Objeto Usuario a copiar.
         */
        Usuario(const Usuario& other);
        
        /**
         * @brief Constructor de la clase Usuario.
         * 
         * Inicializa un objeto Usuario con los datos proporcionados.
         * 
         * @param documento Documento del usuario (ID o similar).
         * @param password Contraseña del usuario.
         * @param antiguedad Antigüedad del usuario en meses.
         * @param puntuacion Puntuación del usuario, por ejemplo, en un sistema de calificación.
         * @param anfitrion Indica si el usuario es un anfitrión o no.
         */
        Usuario(uint64_t documento, char *password, uint16_t antiguedad, float puntuacion, bool anfitrion);
        /**
         * @brief Obtiene el documento del usuario.
         * 
         * @return Referencia constante al documento del usuario.
         */
        uint64_t get_documento() const;

        /**
         * @brief Obtiene la contraseña del usuario.
         * 
         * @return Referencia constante a la contraseña del usuario.
         */
        const char *get_password() const;

        /**
         * @brief Obtiene la antigüedad del usuario.
         * 
         * @return La antigüedad del usuario en meses.
         */
        uint16_t get_antiguedad() const;

        /**
         * @brief Obtiene la puntuación del usuario.
         * 
         * @return La puntuación del usuario.
         */
        float get_puntuacion() const;

        /**
         * @brief Obtiene si el usuario es anfitrión.
         * 
         * @return `true` si el usuario es anfitrión, `false` en caso contrario.
         */
        bool get_anfitrion() const;

        /**
         * @brief Calcula el tamaño total en memoria de un objeto Usuario.
         * 
         * Calcula el tamaño total de un objeto Usuario, incluyendo la memoria ocupada
         * por los strings `m_documento` y `m_password`. Esta función no garantiza
         * el tamaño exacto debido a la naturaleza de las implementaciones de std::string.
         * 
         * @return El tamaño total en bytes ocupado por el objeto Usuario.
         */
        size_t get_obj_size() const;

        /**
         * @brief Destructor de la clase Usuario.
         * 
         * Libera los recursos ocupados por el objeto Usuario.
         */
        ~Usuario();

    private:
        uint64_t m_documento;      ///< Documento de identificación del usuario.
        char * m_password;         ///< Contraseña del usuario.
        uint16_t m_antiguedad;     ///< Antigüedad del usuario en meses.
        float m_puntuacion;        ///< Puntuación del usuario.
        bool m_anfitrion;          ///< Indica si el usuario es anfitrión o no.
};

#endif
