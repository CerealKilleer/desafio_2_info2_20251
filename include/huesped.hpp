#ifndef __HUESPED_HPP__
#define __HUESPED_HPP__
#include <stdint.h>
#include <string>

/**
 * @class Huesped
 * @brief Clase que representa a un huesped con información personal y de acceso.
 * 
 * Esta clase contiene los datos relacionados con un huesped como el documento de identidad, 
 * la contraseña, la antigüedad en meses, la puntuación y si es anfitrión o no.
 */
class Huesped
{
    public:

        
        /**
         * @brief Constructor de la clase Huesped.
         * 
         * Inicializa un objeto Huesped con los datos proporcionados.
         * 
         * @param documento Documento del huesped (ID o similar).
         * @param password Contraseña del huesped.
         * @param antiguedad Antigüedad del huesped en meses.
         * @param puntuacion Puntuación del huesped, por ejemplo, en un sistema de calificación.
         */
        Huesped(uint64_t documento, char *password, uint8_t antiguedad, float puntuacion);
        /**
         * @brief Obtiene el documento del huesped.
         * 
         * @return Referencia constante al documento del huesped.
         */
        uint64_t get_documento() const;

        /**
         * @brief Obtiene la contraseña del huesped.
         * 
         * @return Referencia constante a la contraseña del huesped.
         */
        const char *get_password() const;

        /**
         * @brief Obtiene la antigüedad del huesped.
         * 
         * @return La antigüedad del huesped en meses.
         */
        uint8_t get_antiguedad() const;

        /**
         * @brief Obtiene la puntuación del huesped.
         * 
         * @return La puntuación del huesped.
         */
        float get_puntuacion() const;


        /**
         * @brief Calcula el tamaño total en memoria de un objeto Huesped.
         * 
         * Calcula el tamaño total de un objeto Huesped, incluyendo la memoria ocupada
         * por los strings `m_documento` y `m_password`. Esta función no garantiza
         * el tamaño exacto debido a la naturaleza de las implementaciones de std::string.
         * 
         * @return El tamaño total en bytes ocupado por el objeto Huesped.
         */
        size_t get_obj_size() const;

        /**
         * @brief Destructor de la clase Huesped.
         * 
         * Libera los recursos ocupados por el objeto Huesped.
         */
        ~Huesped();

    private:
        uint64_t m_documento;      ///< Documento de identificación del huesped.
        char * m_password;         ///< Contraseña del huesped.
        uint16_t m_antiguedad;     ///< Antigüedad del huesped en meses.
        float m_puntuacion;        ///< Puntuación del usuario.
};

#endif
