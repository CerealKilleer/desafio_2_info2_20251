#include <iostream>
#include <cstring>
#include "alojamiento.hpp"

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
Alojamiento::Alojamiento()
    : m_nombre(nullptr),
      m_codigo(nullptr),
      m_direccion(nullptr),
      m_departamento(nullptr),
      m_municipio(nullptr),
      m_responsable_id(0),
      m_tipo(0),
      m_amenidades(nullptr),
      m_cant_ameneidades(0)
{}

// Métodos

bool Alojamiento::set_nombre(const char* nombre) 
{
    if (nombre == nullptr)
        return false;

    char* nuevo_nombre = copy_data(nombre, strlen(nombre) + 1);

    if (nuevo_nombre == nullptr) {
        LOG_ERROR("set_nombre", "Error cargando el nombre en el objeto");
        return false;
    }

    delete[] m_nombre;
    m_nombre = nuevo_nombre;

    return true;
}

bool Alojamiento::set_codigo(const char* codigo) 
{
    if (codigo == nullptr)
        return false;

    char* nuevo_codigo = copy_data(codigo, strlen(codigo) + 1);

    if (nuevo_codigo == nullptr) {
        LOG_ERROR("set_codigo", "Error cargando el código en el objeto");
        return false;
    }

    delete[] m_codigo;
    m_codigo = nuevo_codigo;

    return true;
}

bool Alojamiento::set_direccion(const char* direccion) 
{
    if (direccion == nullptr)
        return false;

    char* nueva_direccion = copy_data(direccion, strlen(direccion) + 1);

    if (nueva_direccion == nullptr) {
        LOG_ERROR("set_direccion", "Error cargando la dirección en el objeto");
        return false;
    }

    delete[] m_direccion;
    m_direccion = nueva_direccion;

    return true;
}

void Alojamiento::set_responsable_id(uint32_t id)
{
    m_responsable_id = id;
}

bool Alojamiento::set_departamento(const char* dpto) 
{
    if (dpto == nullptr)
        return false;

    char* nuevo_dpto = copy_data(dpto, strlen(dpto) + 1);

    if (nuevo_dpto == nullptr) {
        LOG_ERROR("set_departamento", "Error cargando el departamento en el objeto");
        return false;
    }

    delete[] m_departamento;
    m_departamento = nuevo_dpto;

    return true;
}

bool Alojamiento::set_municipio(const char* municipio) 
{
    if (municipio == nullptr)
        return false;

    char* nuevo_municipio = copy_data(municipio, strlen(municipio) + 1);

    if (nuevo_municipio == nullptr) {
        LOG_ERROR("set_municipio", "Error cargando el municipio en el objeto");
        return false;
    }

    delete[] m_municipio;
    m_municipio = nuevo_municipio;

    return true;
}

void Alojamiento::set_tipo(uint8_t tipo)
{
    m_tipo = tipo;
}

void Alojamiento::set_precio(uint16_t precio)
{
    m_precio = precio;
}

bool Alojamiento::set_ameneidades(uint8_t cant_ameneidades)
{
    m_cant_ameneidades = cant_ameneidades;
    m_amenidades = new char*[cant_ameneidades];
    if (m_amenidades == nullptr) {
        LOG_ERROR("set_ameneidades", "Error creando el arreglo de ameneidades para este objeto");
        return false;
    }

    return true;
}

bool Alojamiento::set_ameneidad(char *ameneidad)
{
    static size_t idx;
    if (ameneidad == nullptr) {
        LOG_ERROR("set_ameneidad", "La ameneidad es nula");
        return false;
    }
    char *new_ameneidad = new char[strlen(ameneidad) + 1];

    if (new_ameneidad == nullptr) {
        LOG_ERROR("set_ameneidad", "La ameneidad es nula");
        return false;
    }
    
    if (idx >= m_cant_ameneidades) {
        delete[] new_ameneidad;
        LOG_ERROR("set_ameneidad", "Ya no hay espacio para nuevas ameneidades en el objeto");
        return false;
    }

    m_amenidades[idx] = ameneidad;
    return true;
}

// Copia segura de memoria
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

// Destructor
Alojamiento::~Alojamiento() 
{
    delete[] m_nombre;
    delete[] m_codigo;
    delete[] m_direccion;
    delete[] m_departamento;
    delete[] m_municipio;
    for (size_t i=0; i < m_cant_ameneidades; i++) {
        delete[] m_amenidades[i];
    }
    delete[] m_amenidades;
}
