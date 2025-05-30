#ifndef FECHA_HPP
#define FECHA_HPP

#include <stdint.h>
#include <cstddef>


#define LONG_FECHA_CADENA 10
#define CANT_MESES 12
#define DIA_MIN 1
#define MES_MIN 1
#define MES_MAX 12
#define ANIO_MIN 1
#define DIA_DEFECTO 1
#define MES_DEFECTO 1
#define ANIO_DEFECTO 1970

/**
 * @class Fecha
 * @brief Clase que representa una fecha con día, mes y año.
 *
 * Proporciona métodos para validación, comparación, conversión y
 * operaciones con fechas como suma de noches.
 */
class Fecha {
private:
    uint8_t dia;   ///< Día del mes 
    uint8_t mes;   ///< Mes del año 
    int16_t anio;  ///< Año

    /**
     * @brief Verifica si un año es bisiesto.
     * @param anio Año a evaluar.
     * @return true si es bisiesto, false en caso contrario.
     */
    bool es_bisiesto(int16_t anio) const;

    /**
     * @brief Obtiene la cantidad de días en un mes dado.
     * @param mes Mes (1-12).
     * @param anio Año.
     * @return Número de días del mes.
     */
    uint8_t dias_en_mes(uint8_t mes, int16_t anio) const;

    /**
     * @brief Compara esta fecha con otra.
     * @param otra Otra fecha a comparar.
     * @return -1 si esta < otra, 0 si son iguales, 1 si esta > otra.
     */
    int32_t comparar(const Fecha& otra) const;

    /**
     * @brief Obtiene el día de la semana de la fecha.
     * @return Día de la semana (0=Domingo, 1=Lunes, ..., 6=Sábado).
     */
    uint8_t dia_semana() const;

public:
    /**
     * @brief Constructor por defecto. Inicializa con 01/01/1970. (Año Cero Unix \m/)
     */
    Fecha();

    /**
     * @brief Constructor con parámetros.
     * @param d Día.
     * @param m Mes.
     * @param a Año.
     */
    Fecha(uint8_t d, uint8_t m, int16_t a);

    /**
     * @brief Asigna una nueva fecha si es válida.
     * @param d Día.
     * @param m Mes.
     * @param a Año.
     * @return true si la fecha es válida y fue asignada, false en caso contrario.
     */
    bool set_fecha(uint8_t d, uint8_t m, int16_t a);

    /**
     * @brief Convierte la fecha en una cadena "DD/MM/AAAA".
     * @param destino Puntero a un arreglo de al menos 11 caracteres.
     * @return Cadena con la fecha formateada.
     * @note El arreglo destino debe tener al menos 11 caracteres para almacenar la fecha.
     */
    char *a_cadena(char* destino) const;

    /**
     * @brief Carga una fecha desde una cadena en formato "DD/MM/AAAA".
     * @param cadena Cadena de entrada (10 caracteres exactos).
     * @return true si la cadena es válida y la fecha fue asignada.
     */
    bool cargar_desde_cadena(const char* cadena);

    
    /// Operador de igualdad.
    bool operator==(const Fecha& otra) const;

    /// Operador de desigualdad.
    bool operator!=(const Fecha& otra) const;

    /// Operador menor que.
    bool operator<(const Fecha& otra) const;

    /// Operador menor o igual.
    bool operator<=(const Fecha& otra) const;

    /// Operador mayor que.
    bool operator>(const Fecha& otra) const;

    /// Operador mayor o igual.
    bool operator>=(const Fecha& otra) const;

    /**
     * @brief Retorna una nueva fecha sumando noches (días) a la actual.
     * @param noches Número de noches (días) a sumar.
     * @return Nueva fecha resultante.
     */
    Fecha *sumar_noches(uint16_t noches) const;

    /**
     * @brief Muestra la fecha en formato "DD/MM/AAAA".
     * @param fecha Fecha a mostrar.
     */
    void mostrar_fecha(const Fecha& fecha);

    /**
     * @brief Dice cuanto pesa, aproximadamente un objeto creado en memoria
     * @return El peso de bytes del objeto
     */

    size_t get_size();

    /**
     * @brief Agrega años a la fecha actual y entrega una nueva fecha
     * 
     * @param anios Número de años a sumar
     * @return Fecha* Puntero a una nueva instancia de Fecha con la fecha resultante.
     *               El usuario es responsable de liberar la memoria.
     */
    Fecha* agregar_anios(uint8_t anios) const;

    /**
     * @brief Muestra la fecha en un formato legible.
     */
    void formato_legible() const;

    /**
     * @brief Destructor de la clase Fecha.
     * 
     * No se requiere liberar memoria ya que no se usan punteros dinámicos.
     */
    ~Fecha();
};

#endif
