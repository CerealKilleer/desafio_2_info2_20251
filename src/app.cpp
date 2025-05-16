/**
 * @file app.cpp
 * @brief Implementación de la función principal de la aplicación.
 * 
 * Este archivo contiene la implementación de la función principal de la aplicación
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include "usuario.hpp"
#include "unordered_map.hpp"
#include "fecha.hpp"
#include "app.hpp"
#include "reserva.hpp"

#define MAX_PASSWORD_LENGTH 20
#define MEM_LOG(fn, msg) std::cout << "[App/" << fn << "]: " << msg << std::endl
#define CAMPOS_MAX 9
#define RESERVAS_SIZE(size) ((size * 1.5) + 1) // Redimensiona la tabla hash al 150% de su tamaño original

/**
 * @brief Verifica si una reserva está activa comparando la fecha de entrada y la duración con la fecha actual.
 * 
 * @param fecha_entrada Fecha de entrada en formato "dd/mm/aaaa".
 * @param duracion Duración de la reserva en noches.
 * @param fecha_actual Fecha actual en formato "dd/mm/aaaa".
 * @return true si la reserva está activa, false en caso contrario.
 */

static bool esta_activa(const char *fecha_entrada, uint8_t duracion, const char *fecha_actual)
{
    Fecha *fecha_inicio = new Fecha();
    fecha_inicio->cargar_desde_cadena(fecha_entrada);
    Fecha *fecha_fin = fecha_inicio->sumar_noches(duracion);
    delete fecha_inicio;
    Fecha *actual = new Fecha();
    actual->cargar_desde_cadena(fecha_actual);
    bool activa = (*actual < *fecha_fin);
    delete fecha_fin;
    delete actual;

    return activa;
}

/**
 * @brief Obtiene la fecha actual del sistema en formato "dd/mm/aaaa".
 * 
 * @param buffer Cadena donde se guardará la fecha formateada.
 * @param buffer_size Tamaño del buffer para evitar desbordamientos.
 */
static void obtener_fecha_actual(char* buffer, size_t buffer_size);

/**
 * @brief Carga los usuarios desde un archivo de texto y los almacena en un mapa hash.
 * 
 * @param archivo_nombre Nombre del archivo que contiene los datos de los usuarios.
 * @return Un puntero a Unordered_Map que contiene los usuarios cargados.
 */
static Unordered_Map<uint64_t, Usuario>* cargar_usuarios(const std::string &archivo_nombre);

/**
 * @brief Divide una línea separada por ';' en un arreglo de campos.
 * 
 * @param linea Cadena con la línea completa.
 * @param campos Arreglo donde se guardan los campos extraídos.
 * @param max_campos Número máximo de campos a extraer.
 * @return int Cantidad de campos extraídos.
 */
static uint32_t dividir_linea(const std::string &linea, std::string *campos, uint8_t max_campos);
/**
 * @brief Lee reservas desde un archivo de texto con formato separado por ';' y crea objetos Reserva.
 * 
 * @param filename Ruta al archivo de texto.
 * @param num_reservas Referencia a una variable donde se guardará la cantidad de reservas leídas.
 * @return Mapa hash que contiene las reservas leídas.
 */
static Unordered_Map<uint32_t, Reserva>* leer_reservas(const char* filename, size_t &num_reservas, char *fecha);


static void obtener_fecha_actual(char* buffer, size_t buffer_size) 
{
    std::time_t tiempo_actual = std::time(nullptr);
    std::tm* tiempo_local = std::localtime(&tiempo_actual);
    std::strftime(buffer, buffer_size, "%d/%m/%Y", tiempo_local);
}

static Unordered_Map<uint64_t, Usuario>* cargar_usuarios(const std::string &archivo_nombre) 
{
    /*
    * @brief Carga los usuarios desde un archivo de texto y los almacena en un mapa hash.
    * 
    */
     std::ifstream archivo(archivo_nombre);
     
    if (!archivo.is_open()) {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return nullptr;
    }
     
    uint64_t documento;
    char password[MAX_PASSWORD_LENGTH];
    uint16_t antiguedad;
    float puntuacion;
    bool anfitrion;
    size_t size = 0;
    size_t mem_size = 0;

    while (archivo >> documento >> password >> antiguedad >> puntuacion >> anfitrion)
        size++;

    archivo.close();
    archivo.open(archivo_nombre);

    Unordered_Map<uint64_t, Usuario>* usuarios = new Unordered_Map<uint64_t, Usuario>(RESERVAS_SIZE(size));

    for (size_t i = 0; i < size; ++i) {
        archivo >> documento >> password >> antiguedad >> puntuacion >> anfitrion;
        Usuario *user = new Usuario(documento, password, antiguedad, puntuacion, anfitrion);
        mem_size += user->get_obj_size();
        usuarios->insert(documento, user);
    }

    archivo.close();
    MEM_LOG("cargar_usuarios", "Tamaño total de memoria ocupada por los usuarios: " + std::to_string(mem_size) + " bytes");
    MEM_LOG("cargar_usuarios", "Tamaño total de memoria ocupada por el mapa: " + std::to_string(usuarios->info_map()) + " bytes");
    return usuarios;
}
 
/**
 * @brief Divide una línea separada por ';' en un arreglo de campos.
 * 
 * @param linea Cadena con la línea completa.
 * @param campos Arreglo donde se guardan los campos extraídos.
 * @param max_campos Número máximo de campos a extraer.
 * @return int Cantidad de campos extraídos.
 */
static uint32_t dividir_linea(const std::string &linea, std::string *campos, uint8_t max_campos) 
{
    size_t pos_ini = 0;
    uint32_t cnt = 0;

    while (cnt < max_campos) {
        size_t pos_fin = linea.find(';', pos_ini);
        if (pos_fin == std::string::npos) {
            campos[cnt++] = linea.substr(pos_ini);
            break;
        } else {
            campos[cnt++] = linea.substr(pos_ini, pos_fin - pos_ini);
            pos_ini = pos_fin + 1;
        }
    }

    return cnt;
}

/**
 * @brief Lee reservas desde un archivo de texto con formato separado por ';' y crea objetos Reserva.
 * 
 * @param filename Ruta al archivo de texto.
 * @param num_reservas Referencia a una variable donde se guardará la cantidad de reservas leídas.
 * @param fecha Fecha actual en formato "dd/mm/aaaa".
 * @return Mapa hash que contiene las reservas leídas.
*/

static Unordered_Map<uint32_t, Reserva>* leer_reservas(const char* filename, size_t &num_reservas, char *fecha) 
{
    std::ifstream archivo(filename);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return 0;
    }

    std::string linea;
    num_reservas = 0;

    while (std::getline(archivo, linea))
        num_reservas++;

    archivo.close();
    archivo.open(filename);

    if (num_reservas == 0) {
        std::cerr << "El archivo está vacío o no se pudo leer." << std::endl;
        return nullptr;
    }

    Unordered_Map<uint32_t, Reserva>*reservas = new Unordered_Map<uint32_t, Reserva>(num_reservas);

    if (reservas == nullptr) {
        std::cerr << "Error al asignar memoria para las reservas." << std::endl;
        return 0;
    }

    std::string campos[CAMPOS_MAX];

    for (size_t reserva = 0; std::getline(archivo, linea); reserva++) {
        uint8_t campos_extraidos = dividir_linea(linea, campos, CAMPOS_MAX);
        if (campos_extraidos < CAMPOS_MAX) {
            std::cerr << "Línea con menos campos de los esperados: " << linea << std::endl;
            continue;
        }

        //Ahora se puede usar el arreglo campos para crear la reserva
        //Usamos un try-catch para manejar errores de conversión
        //y evitar que el programa se detenga
        try {
            const char* fecha_entrada = campos[0].c_str();
            uint16_t duracion = static_cast<uint16_t>(std::stoi(campos[1]));
            uint32_t codigo_reserva = static_cast<uint32_t>(std::stoi(campos[2]));
            uint32_t codigo_alojamiento = static_cast<uint32_t>(std::stoi(campos[3]));
            uint64_t documento_huesped = static_cast<uint64_t>(std::stoull(campos[4]));
            char metodo_pago = campos[5].empty() ? 'X' : campos[5][0];
            const char* fecha_pago = campos[6].c_str();
            float monto = std::stof(campos[7]);
            const char* anotaciones = campos[8].c_str();
            bool activa = esta_activa(fecha_entrada, duracion, fecha);

            Reserva *nueva_reserva = new Reserva(fecha_entrada, duracion, codigo_reserva,
                                            codigo_alojamiento, documento_huesped,
                                            metodo_pago, fecha_pago, monto, anotaciones, activa);

            reservas->insert(codigo_reserva, nueva_reserva);
        } catch (const std::exception& e) {
            std::cerr << "Error al convertir campos en línea: " << linea << std::endl;
        }
    }

    archivo.close();
    return reservas;
}


void app_main() 
{
    Unordered_Map<uint32_t, Reserva>*reservas;
    size_t cantidad_leida; 
    char fecha_actual[11];
    obtener_fecha_actual(fecha_actual, sizeof(fecha_actual));
    std::cout << "Fecha actual: " << fecha_actual << std::endl;
    reservas = leer_reservas("reservaciones.txt", cantidad_leida, fecha_actual);

    if (reservas == nullptr) {
        std::cerr << "No se leyeron reservas." << std::endl;
        return;
    }

    std::cout << "Reservas leídas: " << cantidad_leida << std::endl;

    delete reservas;
}




