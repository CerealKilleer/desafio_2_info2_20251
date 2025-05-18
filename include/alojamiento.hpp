#ifndef __ALOJAMIENTO_HPP__
#define __ALOJAMIENTO_HPP__

#include <stdint.h>
#include <cstddef>  // Para size_t
#include "reserva.hpp"
#include "linked_list.hpp"
/**
 * @class Alojamiento
 * @brief Representa un alojamiento en una plataforma de alquiler.
 * 
 * Esta clase gestiona información básica como nombre, dirección, código,
 * ubicación y tipo de alojamiento, así como el identificador del responsable.
 */
class Alojamiento {
private:
    uint32_t m_id;                ///< ID del alojamiento.
    uint64_t m_codigo_anfitrion;  ///< Código del anfitrión.
    char* m_nombre;              ///< Nombre del alojamiento.
    char* m_direccion;           ///< Dirección del alojamiento.
    char* m_departamento;        ///< Departamento del alojamiento.
    char* m_municipio;          ///< Municipio del alojamiento.
    uint8_t m_tipo;              ///< Tipo de alojamiento (1 = Casa, 2 = Apartamento).
    float m_precio;             ///< Precio por noche
    char* m_amenidades;         ///< Amenidades del alojamiento.
    Linked_List<Reserva*> *m_reservas; ///< Lista de reservas de un alojamiento (si aplica).

    /**
     * @brief Copia una cadena de caracteres a memoria dinámica.
     * 
     * @param data Cadena de caracteres a copiar.
     * @param len Longitud de la cadena (incluyendo el terminador nulo).
     * @return Puntero a la nueva cadena copiada, o nullptr si falla.
     */
    char* copy_data(const char* data, size_t len);

public:
    /**
     * @brief Constructor por defecto.
     * 
     * Inicializa los punteros a nullptr y valores numéricos a 0.
     * @param id ID del alojamiento.
     * @param nombre Nombre del alojamiento.
     * @param codigo_anfitrion Código único del responsable del alojamiento.
     * @param direccion Dirección física del alojamiento.
     * @param departamento Departamento donde se encuentra el alojamiento.
     * @param municipio Municipio donde se encuentra el alojamiento.
     * @param tipo Tipo de alojamiento (1 = Casa, 2 = Apartamento).
     * @param precio Precio por una noche en el alojamiento.
     * @param amenidades Conjunto de ameneidades que tiene el alojamiento.
     */

    Alojamiento(uint32_t id, const char* nombre, uint64_t codigo_anfitrion,
                const char* direccion, const char* departamento,
                const char* municipio, uint8_t tipo, float precio, const char* amenidades);

    
    
    /**
     * @brief Agrega una reserva activa al alojamiento
     * @return ID del alojamiento.
     */
    Reserva *set_reserva(Reserva *reserva); 
    
    /**
     * @brief Muestra las reservas activas del alojamiento.
     * 
     * Recorre la lista de reservas y muestra la información de cada una.
     */
    void mostrar_reservas() const;

    /**
     * @breif Muestra las reservas activas del alojamiento en un rango de fechas.
     * @brief desde Fecha de inicio.
     * @brief hasta Fecha de fin.
     */
    void mostrar_reservas(Fecha &desde, Fecha &hasta) const;

    uint32_t get_id() const;
    /**
     * @brief Obtiene el codigo del anfitrion.
     * @return El código del anfitrion.
     */
    uint64_t get_codigo_anfitrion() const;
    /**
     * @brief Obtiene el municipio del.
     * @return Cadena con el nombre del municipio.
     */
    char *get_municipio() const;
    /**
     * @brief Obtiene el departamento del alojamiento.
     * @return Cadena con el nombre del departamento.
     */
    char *get_departamento() const;
    /**
     * @brief Obtiene el precio por noche del alojamiento.
     * @return precio por noche del alojamiento.
     */
    float get_precio() const;
    
    /**
     * @brief Muestra la información del alojamiento.
     */
    void mostrar_alojamiento() const;

    /**
     * @brief Destructor.
     * 
     * Libera toda la memoria dinámica utilizada.
     * 
     */
    ~Alojamiento();
};

#endif
