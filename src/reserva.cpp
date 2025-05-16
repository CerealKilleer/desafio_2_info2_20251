#include <iostream>
#include <cstring>
#include <cstdlib>
#include "reserva.hpp"

Reserva::Reserva(const char *fecha_entrada, uint16_t duracion,
                 uint32_t cod_reserva, uint32_t cod_alojamiento,
                 uint64_t doc_huesped, char metodo_pago,
                 const char *fecha_pago, float monto,
                 const char* notas, bool activa)

    : m_duracion(duracion), m_codigo_reserva(cod_reserva),
      m_codigo_alojamiento(cod_alojamiento), m_documento_huesped(doc_huesped),
      m_metodo_pago(metodo_pago), m_monto(monto), m_fecha_entrada(nullptr),
      m_fecha_pago(nullptr), m_anotaciones(nullptr), m_activa(activa)
{
    m_fecha_entrada = new char[LONG_FECHA_CADENA + 1];
    
    if (m_fecha_entrada == nullptr) {
        std::cerr << "Error al asignar memoria para fecha de entrada." << std::endl;
        exit(EXIT_FAILURE);
    }
    
    m_fecha_pago = new char[LONG_FECHA_CADENA + 1];
    
    if (m_fecha_pago == nullptr) {
        std::cerr << "Error al asignar memoria para fecha de pago." << std::endl;
        exit(EXIT_FAILURE);
    }

    size_t size = strlen(notas);
    m_anotaciones = new char[size + 1];

    if (m_anotaciones == nullptr) {
        std::cerr << "Error al asignar memoria para anotaciones." << std::endl;
        exit(EXIT_FAILURE);
    }

    strncpy(m_fecha_entrada, fecha_entrada, LONG_FECHA_CADENA);
    m_fecha_entrada[LONG_FECHA_CADENA] = '\0'; 
    strncpy(m_fecha_pago, fecha_pago, LONG_FECHA_CADENA);
    m_fecha_pago[LONG_FECHA_CADENA] = '\0';
    strncpy(m_anotaciones, notas, size);
    m_anotaciones[size] = '\0';
}

/**
 * @brief Obtiene la fecha de entrada de la reserva.
 * @return Cadena de caracteres con la fecha en formato dd/mm/aaaa.
 */
const char* Reserva::get_fecha_entrada() const {
    return m_fecha_entrada;
}

/**
 * @brief Obtiene la duración de la reserva en noches.
 * @return Duración como entero sin signo de 16 bits.
 */
uint16_t Reserva::get_duracion() const {
    return m_duracion;
}

/**
 * @brief Obtiene el código único de la reserva.
 * @return Código de reserva como entero sin signo de 32 bits.
 */
uint32_t Reserva::get_codigo_reserva() const {
    return m_codigo_reserva;
}

/**
 * @brief Obtiene el código del alojamiento asociado a la reserva.
 * @return Código de alojamiento como entero sin signo de 32 bits.
 */
uint32_t Reserva::get_codigo_alojamiento() const {
    return m_codigo_alojamiento;
}

/**
 * @brief Obtiene el documento del huésped que hizo la reserva.
 * @return Documento como entero sin signo de 64 bits.
 */
uint64_t Reserva::get_documento_huesped() const {
    return m_documento_huesped;
}

/**
 * @brief Obtiene el método de pago utilizado en la reserva.
 * @return Caracter indicando el método de pago ('P' para PSE, 'T' para Tarjeta de Crédito, etc.).
 */
char Reserva::get_metodo_pago() const {
    return m_metodo_pago;
}

/**
 * @brief Obtiene la fecha en que se realizó el pago.
 * @return Cadena de caracteres con la fecha en formato dd/mm/aaaa.
 */
const char* Reserva::get_fecha_pago() const {
    return m_fecha_pago;
}

/**
 * @brief Obtiene el monto pagado en la reserva.
 * @return Monto como número flotante.
 */
float Reserva::get_monto() const {
    return m_monto;
}

/**
 * @brief Obtiene las anotaciones o comentarios que el huésped incluyó en la reserva.
 * @return Cadena de caracteres con las anotaciones.
 */
const char* Reserva::get_anotaciones() const {
    return m_anotaciones;
}

/**
 * @brief Dice si la reserva está activa o no
 */
bool Reserva::get_activa() const {
    return m_activa;
}
Reserva::~Reserva()
{
    delete[] m_fecha_entrada;
    delete[] m_fecha_pago;
    delete[] m_anotaciones;
    std::cout << "Reserva: " << m_codigo_reserva << " destruido" << std::endl;
}