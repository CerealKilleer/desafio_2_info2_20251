#include "fecha.hpp"
#include <cstdio>
#include <cctype>
#include <cstring>

/**
 * @brief Clase para manejar fechas con validación y operaciones básicas.
 */
Fecha::Fecha() : dia(DIA_DEFECTO), mes(MES_DEFECTO), anio(ANIO_DEFECTO) {}

/**
 * @brief Constructor con parámetros, valida la fecha y asigna valores.
 *        Si la fecha es inválida, asigna la fecha por defecto.
 * 
 * @param d Día (1-31)
 * @param m Mes (1-12)
 * @param a Año (>= 1900)
 */
Fecha::Fecha(uint8_t d, uint8_t m, int16_t a) 
{
    if (!set_fecha(d, m, a)) {
        dia = DIA_DEFECTO;
        mes = MES_DEFECTO;
        anio = ANIO_DEFECTO;
    }
}

/**
 * @brief Determina si un año es bisiesto.
 * 
 * @param anio Año a evaluar.
 * @return true Si es bisiesto.
 * @return false Si no es bisiesto.
 */
bool Fecha::es_bisiesto(int16_t anio) const 
{
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

/**
 * @brief Obtiene la cantidad de días en un mes dado y año.
 * 
 * @param mes Mes (1-12)
 * @param anio Año
 * @return uint8_t Días que tiene ese mes en ese año.
 */
uint8_t Fecha::dias_en_mes(uint8_t mes, int16_t anio) const 
{
    static const uint8_t dias_por_mes[CANT_MESES + 1] = {
        0, 31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };
    if (mes == 2 && es_bisiesto(anio))
        return 29;
    return dias_por_mes[mes];
}

/**
 * @brief Establece la fecha si es válida.
 * 
 * @param d Día
 * @param m Mes
 * @param a Año
 * @return true Si la fecha es válida y se asignó.
 * @return false Si la fecha es inválida.
 */
bool Fecha::set_fecha(uint8_t d, uint8_t m, int16_t a) 
{
    if (a < ANIO_MIN || m < MES_MIN || m > MES_MAX || d < DIA_MIN || d > dias_en_mes(m, a))
        return false;
    dia = d;
    mes = m;
    anio = a;
    return true;
}

/**
 * @brief Carga una fecha desde una cadena con formato "dd/mm/aaaa".
 * 
 * @param cadena Cadena de fecha (debe tener longitud EXACTA LONG_FECHA_CADENA).
 * @return true Si la cadena tiene formato válido y la fecha es válida.
 * @return false Si la cadena es inválida o la fecha inválida.
 */
bool Fecha::cargar_desde_cadena(const char* cadena) 
{
    if (!cadena || std::strlen(cadena) != LONG_FECHA_CADENA)
        return false;

    for (int i = 0; i < LONG_FECHA_CADENA; ++i) {
        if (i == 2 || i == 5) {
            if (cadena[i] != '/') return false;
        } else {
            if (!std::isdigit(cadena[i])) return false;
        }
    }

    uint8_t d = (cadena[0] - '0') * 10 + (cadena[1] - '0');
    uint8_t m = (cadena[3] - '0') * 10 + (cadena[4] - '0');
    int16_t a = (cadena[6] - '0') * 1000 + (cadena[7] - '0') * 100 +
                (cadena[8] - '0') * 10 + (cadena[9] - '0');

    return set_fecha(d, m, a);
}

/**
 * @brief Convierte la fecha a cadena con formato "dd/mm/aaaa".
 * 
 * @param destino Buffer donde se copiará la cadena (debe tener al menos 11 bytes).
 */
void Fecha::a_cadena(char* destino) const 
{
    std::sprintf(destino, "%02u/%02u/%04d", dia, mes, anio);
}

/**
 * @brief Compara la fecha con otra.
 * 
 * @param otra Fecha a comparar.
 * @return int Resultado de la comparación:
 *             -1 si esta fecha es menor,
 *              0 si son iguales,
 *              1 si esta fecha es mayor.
 */
uint8_t Fecha::comparar(const Fecha& otra) const 
{
    if (anio != otra.anio) return anio < otra.anio ? -1 : 1;
    if (mes != otra.mes) return mes < otra.mes ? -1 : 1;
    if (dia != otra.dia) return dia < otra.dia ? -1 : 1;
    return 0;
}

bool Fecha::operator==(const Fecha& otra) const 
{ 
    return comparar(otra) == 0; 
}

bool Fecha::operator!=(const Fecha& otra) const 
{ 
    return comparar(otra) != 0; 
}

bool Fecha::operator<(const Fecha& otra) const 
{ 
    return comparar(otra) < 0; 
}

bool Fecha::operator<=(const Fecha& otra) const 
{ 
    return comparar(otra) <= 0; 
}

bool Fecha::operator>(const Fecha& otra) const 
{ 
    return comparar(otra) > 0; 
}

bool Fecha::operator>=(const Fecha& otra) const 
{ 
    return comparar(otra) >= 0; 
}

/**
 * @brief Suma un número dado de noches a la fecha actual, devolviendo una nueva fecha.
 * 
 * @param noches Número de noches a sumar.
 * @return Fecha* Puntero a una nueva instancia de Fecha con la fecha resultante.
 *               El usuario es responsable de liberar la memoria.
 */
Fecha* Fecha::sumar_noches(uint16_t noches) const 
{
    uint8_t d = dia, m = mes;
    int16_t a = anio;

    while (noches > 0) {
        uint8_t dias_mes_actual = dias_en_mes(m, a);
        uint8_t restantes = dias_mes_actual - d;

        if (noches <= restantes) {
            d += noches;
            noches = 0;
        } else {
            noches -= (restantes + 1);
            d = 1;
            if (++m > CANT_MESES) {
                m = 1;
                ++a;
            }
        }
    }

    Fecha* nueva = new Fecha();
    nueva->set_fecha(d, m, a);
    return nueva;
}
