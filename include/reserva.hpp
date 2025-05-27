#ifndef RESERVA_HPP
#define RESERVA_HPP

#include <stdint.h>
#include "fecha.hpp"

/**
 * @brief Representa una reservación en el sistema de alojamientos.
 */
class Reserva {
private:
    Fecha * m_fecha_entrada;       ///< Fecha de entrada al alojamiento.
    Fecha * m_fecha_salida;      ///< Fecha de salida del alojamiento.
    uint16_t m_duracion;          ///< Duración de la estancia en noches.
    uint32_t m_codigo_reserva;    ///< Código único de la reservación.
    uint32_t m_codigo_alojamiento;///< Código del alojamiento reservado.
    uint64_t m_documento_huesped; ///< Documento del huésped (ID).
    char m_metodo_pago;           ///< Método de pago: 'P' (PSE) o 'T' (Tarjeta de crédito).
    Fecha * m_fecha_pago;          ///< Fecha en que se realizó el pago.
    float m_monto;                ///< Monto total pagado.
    char * m_anotaciones;         ///< Anotaciones del huésped (máx. 1000 caracteres).

public:
    /**
     * @brief Constructor con parámetros.
     * @param fecha_entrada Fecha de entrada.
     * @param duracion Duración en noches.
     * @param cod_reserva Código de la reserva.
     * @param cod_alojamiento Código del alojamiento.
     * @param doc_huesped Documento del huésped.
     * @param metodo_pago Método de pago ('P' o 'T').
     * @param fecha_pago Fecha del pago.
     * @param monto Monto del pago.
     * @param notas Anotaciones del huésped.
     */
    Reserva(Fecha *fecha_entrada, Fecha *salida, uint16_t duracion,
            uint32_t cod_reserva, uint32_t cod_alojamiento,
            uint64_t doc_huesped, char metodo_pago,
            Fecha *fecha_pago, float monto,
            const char* notas);

   
    /**
     * @brief Destructor de la clase Reserva.
     */
    ~Reserva();

    /** 
     * @brief Obtiene la fecha de entrada. 
    */
    const Fecha* get_fecha_entrada() const;
    
    /** 
     * @brief Obtiene la fecha de salida. 
    */
    const Fecha* get_fecha_salida() const;
    /** 
     * @brief Obtiene la duración en noches. 
    */
    uint16_t get_duracion() const;

    /** @brief Obtiene el código de la reserva. 
     * 
    */
    uint32_t get_codigo_reserva() const;

    /** 
     * @brief Obtiene el código del alojamiento. 
    */
    uint32_t get_codigo_alojamiento() const;

    /** 
     * @brief Obtiene el documento del huésped. 
    */
    uint64_t get_documento_huesped() const;

    /** 
     * @brief Obtiene el método de pago. 
    */
    char get_metodo_pago() const;

    /** 
     * @brief Obtiene la fecha del pago. 
    */
    const Fecha* get_fecha_pago() const;

    /** 
     * @brief Obtiene el monto pagado. 
    */
    float get_monto() const;

    /** 
     * @brief Obtiene las anotaciones del huésped. 
    */
    const char* get_anotaciones() const;
    
    /**
     * @brief Muestra la información de la reserva.
     */
    void mostrar() const;

    /**
     * @brief Obtiene el tamaño total en memoria de un objeto Reserva.
     * 
     * @return Tamaño total en bytes ocupado por el objeto Reserva.
     */

    size_t get_size() const;
};

#endif
