#include <iostream>
#include <cstring>
#include "alojamiento.hpp"
#include "reserva.hpp"
#include "linked_list.hpp"
/**
 * @def LOG_ERROR(fn, msg)
 * @brief Macro para imprimir errores con el contexto de Alojamiento.
 * @param fn Nombre de la función donde ocurre el error.
 * @param msg Mensaje de error a imprimir.
 */
#define LOG_ERROR(fn, msg) std::cerr << "[Alojamiento/" << fn << "]: " << msg << std::endl

/**
 * @def LOG_SUCCESS(fn, msg)
 * @brief Macro para imprimir mensajes exitosos con el contexto de Alojamiento.
 * @param fn Nombre de la función donde ocurre el éxito.
 * @param msg Mensaje de éxito a imprimir.
 */
#define LOG_SUCCESS(fn, msg) std::cout << "[Alojamiento/" << fn << "]: " << msg << std::endl 

// Constructor
Alojamiento::Alojamiento(uint32_t id, const char* nombre, uint64_t codigo_anfitrion,
                         const char* direccion, const char* departamento,
                         const char* municipio, uint8_t tipo, float precio, 
                         const char* amenidades)

    : m_id(id), m_codigo_anfitrion(codigo_anfitrion),
      m_tipo(tipo), m_precio(precio), m_nombre(nullptr), m_direccion(nullptr), 
      m_departamento(nullptr), m_municipio(nullptr), m_amenidades(nullptr)
{
    size_t len_nombre = strlen(nombre) + 1;
    size_t len_direccion = strlen(direccion) + 1;
    size_t len_departamento = strlen(departamento) + 1;
    size_t len_municipio = strlen(municipio) + 1;
    size_t len_amenidades = strlen(amenidades) + 1;

    m_nombre = copy_data(nombre, len_nombre);
    m_direccion = copy_data(direccion, len_direccion);
    m_departamento = copy_data(departamento, len_departamento);
    m_municipio = copy_data(municipio, len_municipio);
    m_amenidades = copy_data(amenidades, len_amenidades);
    m_reservas = new Linked_List<Reserva*>();
}

/**
 * @brief Retorna el codigo del anfitrion.
 * 
 * @return Código del anfitrion.
 */

uint64_t Alojamiento::get_codigo_anfitrion() const 
{
    return m_codigo_anfitrion;
}
/**
 * @brief Copia los datos de una cadena a otra.
 * @param data Cadena de caracteres a copiar.
 * @param len Longitud de la cadena a copiar.
 * @return Puntero a la nueva cadena copiada, o nullptr si hubo error.
 */
char* Alojamiento::copy_data(const char* data, size_t len) 
{
    if (data == nullptr)
        return nullptr;

    char* new_data = new char[len];
    if (new_data == nullptr)
        return nullptr;

    memcpy(new_data, data, len);
    return new_data;
}

/**
 * @brief Agrega una reserva activa al alojamiento.
 * @return retorna la referencia a la reserva agregada.
 */

Reserva* Alojamiento::set_reserva(Reserva* reserva) 
{
    if (reserva == nullptr) {
        LOG_ERROR("set_reserva", "La reserva es nula");
        return nullptr;
    }

    m_reservas->insert_front(reserva);
    return reserva;
}

/**
 * @abrief Muestra las reservas activas del alojamiento.
 */
void Alojamiento::mostrar_reservas() const
{
    if (m_reservas == nullptr) {
        std::cout << "No hay reservas activas." << std::endl;
        return;
    }

    Node<Reserva*>* current = m_reservas->get_head();
    while (current != nullptr) {
        Reserva* reserva = current->data;
        reserva->mostrar();
        current = m_reservas->get_next(current);
    }
}

/**
 * @brief Muestra las reservas activas del alojamiento en un rango de fechas
 * 
 * Recorre la lista de reservas y muestra la información de cada una
 * que esté dentro del rango de fechas especificado.
 * 
 * @param desde Fecha de inicio.
 * @param hasta Fecha de fin.
 */

bool Alojamiento::es_candidato_reserva(const Fecha &desde, const Fecha &hasta) const
{
    Node<Reserva*>* current = m_reservas->get_head();
    while (current != nullptr) {
        Reserva* reserva = current->data;
        
        if (*(reserva->get_fecha_entrada()) < hasta && *(reserva->get_fecha_salida()) > desde)
            return false;
        
        current = m_reservas->get_next(current);
    }
    
    return true;
}

/**
 * @brief Muestra las reservas activas del alojamiento en un rango de fechas y para un departamento en especifico.
 * 
 * Recorre la lista de reservas y muestra la información de cada una
 * que esté dentro del rango de fechas especificado.
 * 
 * @param desde Fecha de inicio.
 * @param hasta Fecha de fin.
 * @param dpto Municipio a filtrar.
 */

bool Alojamiento::es_candidato_reserva(const Fecha &desde, const Fecha &hasta, const std::string &mpo) const
{
    Node<Reserva*>* current = m_reservas->get_head();
    while (current != nullptr) {
        Reserva* reserva = current->data;
        
        if (*(reserva->get_fecha_entrada()) < hasta && *(reserva->get_fecha_salida()) > desde)
            return false;
        
        current = m_reservas->get_next(current);
    }

    if (strcmp(m_municipio, mpo.c_str()) == 0)
        return true;
    
    return false;
}

/** 
 * @brief Muestra las reservas activas del alojamiento en un rango de fechas.
 * @param desde Fecha de inicio.
 * @param hasta Fecha de fin.
 
**/
void Alojamiento::mostrar_reservas(Fecha &desde, Fecha &hasta) const
{
    if (m_reservas == nullptr) {
        std::cout << "No hay reservas para el alojamiento." << std::endl;
        return;
    }
    
    Node<Reserva*>* current = m_reservas->get_head();
    while (current != nullptr) {
        Reserva* reserva = current->data;
        
        if (*(reserva->get_fecha_entrada()) < hasta && *(reserva->get_fecha_salida()) > desde) {
            std::cout << "Alojamiento: " << m_nombre << std::endl;
            reserva->mostrar();
        }
        
        current = m_reservas->get_next(current);
    }
}
/**
 * @brief Muestra la información del alojamiento.
 * 
 */
void Alojamiento::mostrar_alojamiento() const
{
   std::cout << "------------*------------" << std::endl;
   std::cout << "ID: " << m_id << std::endl;
   std::cout << "Nombre: " << m_nombre << std::endl;
   std::cout << "Código anfitrión: " << m_codigo_anfitrion << std::endl;
   std::cout << "Dirección: " << m_direccion << std::endl;
   std::cout << "Departamento: " << m_departamento << std::endl;
   std::cout << "Municipio: " << m_municipio << std::endl;
   std::cout << "Tipo: " << (m_tipo == 1 ? "Casa" : "Apartamento") << std::endl;
   std::cout << "Precio: " << m_precio << std::endl;
   std::cout << "Amenidades: " << m_amenidades << std::endl;
   std::cout << "------------*------------" << std::endl;
}

/**
 * @brief Obtiene el ID del alojamiento.
 * 
 * @return ID del alojamiento.
 */
uint32_t Alojamiento::get_id() const 
{
    return m_id;
}

float Alojamiento::get_precio() const 
{
    return m_precio;
}
/**
 * @brief Elimina una reserva del alojamiento.
 * @return true si se eliminó correctamente, false en caso contrario. 
 */

bool Alojamiento::eliminar_reserva(uint32_t codigo_reserva) 
{
    size_t cnt = 0;
    Node<Reserva*>* current = m_reservas->get_head();
    while (current != nullptr) {
        cnt++;
        Reserva* reserva = current->data;
        if (reserva->get_codigo_reserva() == codigo_reserva) {
            m_reservas->remove(reserva);
            LOG_SUCCESS("eliminar_reserva", "La operación tomó: " + std::to_string(cnt) + " ciclos");
            return true;
        }
        current = m_reservas->get_next(current);
    }
    LOG_SUCCESS("eliminar_reserva", "La operación tomó: " + std::to_string(cnt) + " ciclos");
    return false;
}


// Destructor
Alojamiento::~Alojamiento() 
{
    delete[] m_nombre;
    delete[] m_direccion;
    delete[] m_departamento;
    delete[] m_municipio;
    delete[] m_amenidades;
    delete m_reservas;
}
