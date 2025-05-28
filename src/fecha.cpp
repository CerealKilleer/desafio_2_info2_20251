#include "fecha.hpp"
#include <cstdio>
#include <cctype>
#include <cstring>
#include <iostream>
#include "performance.hpp"
/**
 * @brief Clase para manejar fechas con validación y operaciones básicas.
 */
Fecha::Fecha() : dia(DIA_DEFECTO), mes(MES_DEFECTO), anio(ANIO_DEFECTO) {
    g_tamano += sizeof(Fecha);
}

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
    g_strlen_cnt++;
    if (!cadena || std::strlen(cadena) != LONG_FECHA_CADENA) {
        return false;
    }
        
    for (int i = 0; i < LONG_FECHA_CADENA; ++i) {
        if (i == 2 || i == 5) {
            if (cadena[i] != '/') 
                return false;
        } else {
            g_is_digit_cnt++;
            if (!std::isdigit(cadena[i])) {
                return false;
            }
        }
        g_ciclos++;
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
 * @return char* Puntero al buffer destino.
 * 
 */
char *Fecha::a_cadena(char* destino) const 
{
    std::sprintf(destino, "%02u/%02u/%04d", dia, mes, anio);
    g_sprintf_cnt++;
    return destino;
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

int32_t Fecha::comparar(const Fecha& otra) const {
    if (anio != otra.anio)
        return anio - otra.anio;
    if (mes != otra.mes)
        return mes - otra.mes;
    return dia - otra.dia;
}

/**
 * @brief Sobrecarga de operadores para comparar fechas.
 * 
 * @param otra Fecha a comparar.
 * @return true Si son iguales.
 * @return false Si son diferentes.
 */
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

void Fecha::mostrar_fecha(const Fecha& fecha) 
{
    char buffer[LONG_FECHA_CADENA + 1];
    std::cout << "Fecha: " << fecha.a_cadena(buffer) << std::endl;
}
/**
 * @brief Suma un número dado de noches a la fecha actual, devolviendo una nueva fecha.
 * 
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
        g_ciclos++;
    }   

    Fecha* nueva = new Fecha();
    nueva->set_fecha(d, m, a);
    return nueva;
}

/**
 * @brief Agrega años a la fecha actual y entrega una nueva fecha
 * 
 * @param anios Número de años a sumar
 * @return Fecha* Puntero a una nueva instancia de Fecha con la fecha resultante.
 *               El usuario es responsable de liberar la memoria.
 */
Fecha* Fecha::agregar_anios(uint8_t anios) const 
{
    Fecha *nueva_fecha = new Fecha(dia, mes, anio + anios);
    return nueva_fecha;
}

/**
 * @brief Obtener el tamaño en memoria de un objeto
 * @return el tamaño en bytes del objeto
 */
size_t Fecha::get_size()
{
    return sizeof(*this);
}

/**
 * @brief Muestra la fecha en un formato legible.
 */

void Fecha::formato_legible() const {
    static const char* NOMBRES_DIA[] = {
    "domingo", "lunes", "martes", "miércoles", "jueves", "viernes", "sábado"
    };

    static const char* NOMBRES_MES[] = {
        "enero", "febrero", "marzo", "abril", "mayo", "junio",
        "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"
    };

    uint8_t dia_semana = this->dia_semana();
    std::cout << NOMBRES_DIA[dia_semana] << ", " << (int)dia << " de "
              << NOMBRES_MES[mes - 1] << " del " << anio << std::endl;

}
/**
 * @brief dia de la semana con formula de Zeller
 */
uint8_t Fecha::dia_semana() const {
    uint8_t d = dia;
    uint8_t m = mes;
    uint16_t y = anio;

    if (m < 3) {
        m += 12;
        y -= 1;
    }

    uint16_t k = y % 100;
    uint16_t j = y / 100;

    uint16_t h = (d + 13*(m + 1)/5 + k + k/4 + j/4 + 5*j) % 7;
    return (h + 6) % 7;
}

/**
 * @brief Destructor de la clase Fecha.
 * 
 * No se requiere liberar memoria ya que no se usan punteros dinámicos.
 */
Fecha::~Fecha()
{
    g_tamano -= this->get_size();
}