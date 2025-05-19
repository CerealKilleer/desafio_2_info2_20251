#include <iostream>
#include <cstring>
#include <cstdlib>
#include "reserva.hpp"
#include "linked_list.hpp"
#include "fecha.hpp"

/**
 * @brief Constructor de la clase Reserva.
 * @param fecha_entrada Fecha de entrada de la reserva.
 * @param fecha_salida Fecha de salida de la reserva.
 * @param duracion Duración de la reserva en noches.
 * @param cod_reserva Código único de la reserva.
 * @param cod_alojamiento Código del alojamiento asociado a la reserva.
 * @param doc_huesped Documento del huésped que hizo la reserva.
 * @param metodo_pago Método de pago utilizado en la reserva.
 * @param fecha_pago Fecha en que se realizó el pago.
 * @param monto Monto pagado en la reserva.
 * @param notas Anotaciones o comentarios que el huésped incluyó en la reserva.
 * @param activa Indica si la reserva está activa o no.
 */
Reserva::Reserva(Fecha *fecha_entrada, Fecha *fecha_salida, uint16_t duracion,
                 uint32_t cod_reserva, uint32_t cod_alojamiento,
                 uint64_t doc_huesped, char metodo_pago,
                 Fecha *fecha_pago, float monto,
                 const char* notas, bool activa)

    : m_duracion(duracion), m_codigo_reserva(cod_reserva),
      m_codigo_alojamiento(cod_alojamiento), m_documento_huesped(doc_huesped),
      m_metodo_pago(metodo_pago), m_monto(monto), m_fecha_entrada(fecha_entrada), 
      m_fecha_salida(fecha_salida), m_fecha_pago(fecha_pago), m_anotaciones(nullptr), 
      m_activa(activa)
{
    if (notas != nullptr) {
        size_t len = strlen(notas) + 1;
        m_anotaciones = new char[len];
        if (m_anotaciones != nullptr) {
            memcpy(m_anotaciones, notas, len);
        }
    }
}

/**
 * @brief Obtiene la fecha de entrada de la reserva.
 * @return Cadena de caracteres con la fecha en formato dd/mm/aaaa.
 */
const Fecha* Reserva::get_fecha_entrada() const 
{
    return m_fecha_entrada;
}

/**
 * @brief Obtiene la duración de la reserva en noches.
 * @return Duración como entero sin signo de 16 bits.
 */
uint16_t Reserva::get_duracion() const 
{
    return m_duracion;
}

/**
 * @brief Obtiene el código único de la reserva.
 * @return Código de reserva como entero sin signo de 32 bits.
 */
uint32_t Reserva::get_codigo_reserva() const 
{
    return m_codigo_reserva;
}


void Reserva::mostrar() const
{
    std::cout << "------------*------------" << std::endl;
    char fecha[LONG_FECHA_CADENA];
    std::cout << "Reserva: " << m_codigo_reserva << std::endl;
    std::cout << "Código de alojamiento: " << m_codigo_alojamiento << std::endl;
    std::cout << "Documento del huésped: " << m_documento_huesped << std::endl;
    std::cout << "Método de pago: " << ((m_metodo_pago == 'P') ? "PSE" : "T. Credito") << std::endl;
    std::cout << "Fecha de entrada: " << 
    m_fecha_entrada->a_cadena(fecha) << std::endl;
    std::cout << "Fecha de salida: " << m_fecha_salida->a_cadena(fecha) << std::endl;
    std::cout << "Duración: " << m_duracion << " noches" << std::endl;
    std::cout << "Fecha de pago: " << m_fecha_pago->a_cadena(fecha) << std::endl;
    std::cout << "Monto pagado: " << m_monto << std::endl;
    std::cout << "Anotaciones: " << (m_anotaciones ? m_anotaciones : "(Ninguna)") << std::endl;
    std::cout << "------------*------------" << std::endl;
}

/**
 * @brief Obtiene el código del alojamiento asociado a la reserva.
 * @return Código de alojamiento como entero sin signo de 32 bits.
 */
uint32_t Reserva::get_codigo_alojamiento() const 
{
    return m_codigo_alojamiento;
}

/**
 * @brief Obtiene el documento del huésped que hizo la reserva.
 * @return Documento como entero sin signo de 64 bits.
 */
uint64_t Reserva::get_documento_huesped() const 
{
    return m_documento_huesped;
}

/**
 * @brief Obtiene el método de pago utilizado en la reserva.
 * @return Caracter indicando el método de pago ('P' para PSE, 'T' para Tarjeta de Crédito, etc.).
 */
char Reserva::get_metodo_pago() const 
{
    return m_metodo_pago;
}

/**
 * @brief Obtiene la fecha en que se realizó el pago.
 * @return Cadena de caracteres con la fecha en formato dd/mm/aaaa.
 */
const Fecha* Reserva::get_fecha_pago() const 
{
    return m_fecha_pago;
}

/**
 * @brief Obtiene el monto pagado en la reserva.
 * @return Monto como número flotante.
 */
float Reserva::get_monto() const 
{
    return m_monto;
}

/**
 * @brief Obtiene las anotaciones o comentarios que el huésped incluyó en la reserva.
 * @return Cadena de caracteres con las anotaciones.
 */
const char* Reserva::get_anotaciones() const 
{
    return m_anotaciones;
}

/**
 * @brief Dice si la reserva está activa o no
 */
bool Reserva::get_activa() const 
{
    return m_activa;
}

/**
 * @brief Destructor de la clase Reserva.
 * 
 * Libera la memoria ocupada por los atributos de la clase.
 */

const Fecha *Reserva::get_fecha_salida() const 
{
    return m_fecha_salida;
}

/**
 * @brief Destructor de la clase Reserva.
 * 
 * Libera la memoria ocupada por los atributos de la clase.
 */
Reserva::~Reserva()
{
    delete[] m_fecha_entrada;
    delete[] m_fecha_pago;
    delete[] m_fecha_salida;
    delete[] m_anotaciones;
}