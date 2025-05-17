#ifndef __ANFITRION_HPP__
#define __ANFITRION_HPP__
#include <stdint.h>
#include <string>

/**
 * @class Anfitrion
 * @brief Clase que representa a un anfitrion con información personal y de acceso.
 * 
 * Esta clase contiene los datos relacionados con un anfitrion como el documento de identidad, 
 * la contraseña, la antigüedad en meses, la puntuación y si es anfitrión o no.
 */
class Anfitrion
{
    public:

        
        /**
         * @brief Constructor de la clase Anfitrion.
         * 
         * Inicializa un objeto Anfitrion con los datos proporcionados.
         * 
         * @param documento Documento del anfitrion (ID o similar).
         * @param password Contraseña del anfitrion.
         * @param antiguedad Antigüedad del anfitrion en meses.
         * @param puntuacion Puntuación del anfitrion, por ejemplo, en un sistema de calificación.
         * @param anfitrion Indica si el anfitrion es un anfitrión o no.
         */
        Anfitrion(uint64_t documento, char *password, uint16_t antiguedad, float puntuacion);
        /**
         * @brief Obtiene el documento del anfitrion.
         * 
         * @return Referencia constante al documento del anfitrion.
         */
        uint64_t get_documento() const;

        /**
         * @brief Obtiene la contraseña del anfitrion.
         * 
         * @return Referencia constante a la contraseña del anfitrion.
         */
        const char *get_password() const;

        /**
         * @brief Obtiene la antigüedad del anfitrion.
         * 
         * @return La antigüedad del anfitrion en meses.
         */
        uint16_t get_antiguedad() const;

        /**
         * @brief Obtiene la puntuación del anfitrion.
         * 
         * @return La puntuación del anfitrion.
         */
        float get_puntuacion() const;


        /**
         * @brief Calcula el tamaño total en memoria de un objeto Anfitrion.
         * 
         * Calcula el tamaño total de un objeto Anfitrion, incluyendo la memoria ocupada
         * por los strings `m_documento` y `m_password`. Esta función no garantiza
         * el tamaño exacto debido a la naturaleza de las implementaciones de std::string.
         * 
         * @return El tamaño total en bytes ocupado por el objeto Anfitrion.
         */
        size_t get_obj_size() const;

        /**
         * @brief Destructor de la clase Anfitrion.
         * 
         * Libera los recursos ocupados por el objeto Anfitrion.
         */
        ~Anfitrion();

    private:
        uint64_t m_documento;      ///< Documento de identificación del anfitrion.
        char * m_password;         ///< Contraseña del anfitrion.
        uint16_t m_antiguedad;     ///< Antigüedad del anfitrion en meses.
        float m_puntuacion;        ///< Puntuación del usuario.
};

#endif
