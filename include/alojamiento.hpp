#ifndef __ALOJAMIENTO_HPP__
#define __ALOJAMIENTO_HPP__

#include <stdint.h>
#include <cstddef>  // Para size_t

/**
 * @class Alojamiento
 * @brief Representa un alojamiento en una plataforma de alquiler.
 * 
 * Esta clase gestiona información básica como nombre, dirección, código,
 * ubicación y tipo de alojamiento, así como el identificador del responsable.
 */
class Alojamiento {
private:
    char* m_nombre;        /**< Nombre del alojamiento */
    char* m_codigo;        /**< Código único del alojamiento */
    uint32_t m_responsable_id; /**< ID (cédula) del responsable */
    char* m_municipio;     /**< Municipio donde se encuentra el alojamiento */
    char* m_departamento;  /**< Departamento donde se encuentra el alojamiento */
    uint8_t m_tipo;        /**< Tipo de alojamiento: 1 = Casa, 2 = Apartamento */
    char* m_direccion;     /**< Dirección física del alojamiento */
    uint16_t m_precio;       /**< Precio por una noche en el alojamiento */
    uint8_t m_cant_ameneidades; /**< Cantidad de ameneidades que tiene el alojamiento */
    char** m_amenidades; /**< Conjunto de ameneidades */

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
     */
    Alojamiento();

    /**
     * @brief Establece el nombre del alojamiento.
     * 
     * @param nombre Cadena con el nuevo nombre.
     * @return true si se asignó correctamente, false si hubo error.
     */
    bool set_nombre(const char* nombre);

    /**
     * @brief Establece el código del alojamiento.
     * 
     * @param codigo Cadena con el nuevo código.
     * @return true si se asignó correctamente, false si hubo error.
     */
    bool set_codigo(const char* codigo);

    /**
     * @brief Establece el ID del responsable del alojamiento.
     * 
     * @param id Entero que representa la cédula del responsable.
     */
    void set_responsable_id(uint32_t id);

    /**
     * @brief Establece el departamento del alojamiento.
     * 
     * @param dpto Cadena con el nombre del departamento.
     * @return true si se asignó correctamente, false si hubo error.
     */
    bool set_departamento(const char* dpto);

    /**
     * @brief Establece el municipio del alojamiento.
     * 
     * @param municipio Cadena con el nombre del municipio.
     * @return true si se asignó correctamente, false si hubo error.
     */
    bool set_municipio(const char* municipio);

    /**
     * @brief Establece el tipo de alojamiento.
     * 
     * @param tipo Valor numérico: 1 = Casa, 2 = Apartamento.
     */
    void set_tipo(uint8_t tipo);

    /**
     * @brief Establece la dirección del alojamiento.
     * 
     * @param direccion Cadena con la dirección física.
     * @return true si se asignó correctamente, false si hubo error.
     */
    bool set_direccion(const char* direccion);

    /**
     * @brief Establece el precio por noche del alojamiento.
     * 
     * @param precio Numero de 16 bits con el precio en USD del apartamento
     * @return true si se asignó correctamente, false si hubo error.
     */

    void set_precio(uint16_t precio);

    /**
     * @brief Establece la cantidad de ameneidades que tiene el alojamiento
     * @param cant_ameneidades Dato del archivo de texto que indica cuantas ameneidades tiene el alojamiento
     * @return true se pudo crear el arreglo de punteros para almacenar las ameneidades
     * @return false no se pudo crear el arreglo de punteros para almacenar las ameneidades
    */

    bool set_ameneidades(uint8_t cant_ameneidades);

    /**
     * @brief Agrega una ameneidad siempre que sea posible. 
     * @param ameneidad Puntero que almacena una referencia a la ameneidad a agregar
     * @return true se pudo agregar una ameneidad
     * @return false no se pudo crear una ameneidad
    */
    bool set_ameneidad(char *ameneidad);

    /**
     * @brief Destructor.
     * 
     * Libera toda la memoria dinámica utilizada.
     * 
     */

    ~Alojamiento();
};

#endif
