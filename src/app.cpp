/**
 * @file app.cpp
 * @brief Implementación de la función principal de la aplicación.
 * 
 * Este archivo contiene la implementación de la función principal de la aplicación
 */

#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <string>
#include <ctime>
#include "huesped.hpp"
#include "anfitrion.hpp"
#include "unordered_map.hpp"
#include "fecha.hpp"
#include "app.hpp"
#include "reserva.hpp"
#include "linked_list.hpp"

#define MAX_PASSWORD_LENGTH 20
#define SUCCESS_LOG(fn, msg) std::cout << "[App/" << fn << "]: " << msg << std::endl
#define ERROR_LOG(fn, msg) std::cerr << "[App/" << fn << "]: " << msg << std::endl
#define CAMPOS_MAX_ALOJAMIENTO 9
#define CAMPOS_MAX_RESERVA 9
#define RESERVAS_SIZE(size) ((size * 1.5) + 1) // Redimensiona la tabla hash al 150% de su tamaño original
#define HUESPED_FILE "huespedes.txt"
#define ANFITRION_FILE "anfitriones.txt"
#define ALOJAMIENTO_FILE "alojamientos.txt"
#define RESERVAS_FILE "reservaciones.txt"
#define ESTA_ACTIVA(fin, sistema) (fin > sistema) // Verifica si la reserva está activa

/**
 * @brief Obtiene la fecha actual del sistema en formato "dd/mm/aaaa".
 * 
 * @param buffer Cadena donde se guardará la fecha formateada.
 * @param buffer_size Tamaño del buffer para evitar desbordamientos.
 */
static void obtener_fecha_actual(char* buffer, size_t buffer_size);

/**
 * @brief Carga los huespedes desde un archivo de texto y los almacena en un mapa hash.
 * 
 * @param archivo_nombre Nombre del archivo que contiene los datos de los huesped.
 * @return Un puntero a Unordered_Map que contiene los huspedes cargados.
 */
static Unordered_Map<uint64_t, Huesped>* cargar_huespedes(const std::string &archivo_nombre);

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
 * @brief Lee el archivo de reservas y carga las reservas para cada alojamiento
 * 
 * @param filename Ruta al archivo de texto.
 * @param alojamientos Mapa hash de alojamientos.
 * @param num_reservas Referencia a una variable donde se guardará la cantidad de reservas leidas
 * @return Mapa hash que contiene las reservas leídas.
*/

static Unordered_Map<uint32_t, Reserva> *leer_reservas(const char* filename, 
                                        Unordered_Map<uint32_t, Alojamiento>*alojamientos, 
                                        size_t &num_reservas, Fecha *fecha_actual);


static void obtener_fecha_actual(char* buffer, size_t buffer_size) 
{
    std::time_t tiempo_actual = std::time(nullptr);
    std::tm* tiempo_local = std::localtime(&tiempo_actual);
    std::strftime(buffer, buffer_size, "%d/%m/%Y", tiempo_local);
}

static Unordered_Map<uint64_t, Huesped>* cargar_huespedes(const std::string &archivo_nombre) 
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

    Unordered_Map<uint64_t, Huesped>* usuarios = new Unordered_Map<uint64_t, Huesped>(RESERVAS_SIZE(size));

    for (size_t i = 0; i < size; ++i) {
        archivo >> documento >> password >> antiguedad >> puntuacion >> anfitrion;
        Huesped *user = new Huesped(documento, password, antiguedad, puntuacion);
        mem_size += user->get_obj_size();
        usuarios->insert(documento, user);
    }

    archivo.close();
    SUCCESS_LOG("cargar_usuarios", "Tamaño total de memoria ocupada por los usuarios: " + std::to_string(mem_size) + " bytes");
    SUCCESS_LOG("cargar_usuarios", "Tamaño total de memoria ocupada por el mapa: " + std::to_string(usuarios->info_map()) + " bytes");
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
 * @brief Lee el archivo de alojamientos y carga los alojamientos correspondientes a un anfitrion
 * 
 * @param filename Ruta al archivo de texto.
 * @param num_reservas Referencia a una variable donde se guardará la cantidad de alojamientos leidos
 * @param id_anfitrion ID del anfitrion para seleccionar sus alojamientos.
 * @return Mapa hash que contiene las reservas leídas.
*/

static Unordered_Map<uint32_t, Alojamiento>* leer_alojamientos(const char* filename, Anfitrion *anfitrion, size_t &num_alojamientos) 
{
    std::ifstream archivo(filename);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return 0;
    }

    std::string linea;
    num_alojamientos = 0;

    while (std::getline(archivo, linea))
        num_alojamientos++;

    archivo.close();
    archivo.open(filename);

    if (num_alojamientos == 0) {
        std::cerr << "El archivo está vacío o no se pudo leer." << std::endl;
        return nullptr;
    }

    Unordered_Map<uint32_t, Alojamiento>*Alojamientos = new Unordered_Map<uint32_t, Alojamiento>(num_alojamientos);
    num_alojamientos = 0;

    if (Alojamientos == nullptr) {
        std::cerr << "Error al asignar memoria para los alojamientos." << std::endl;
        return nullptr;
    }

    std::string campos[CAMPOS_MAX_ALOJAMIENTO];

    for (size_t alojamiento = 0; std::getline(archivo, linea); alojamiento++) {
        uint8_t campos_extraidos = dividir_linea(linea, campos, CAMPOS_MAX_ALOJAMIENTO);
        if (campos_extraidos < CAMPOS_MAX_ALOJAMIENTO) {
            std::cerr << "Línea con menos campos de los esperados: " << linea << std::endl;
            continue;
        }

        //Ahora se puede usar el arreglo campos para crear la reserva
        //Usamos un try-catch para manejar errores de conversión
        //y evitar que el programa se detenga
        try {
            const char* nombre = campos[0].c_str();
            uint32_t codigo_alojamiento = static_cast<uint32_t>(std::stoi(campos[1]));
            uint64_t documento_anfitrion = static_cast<uint64_t>(std::stoull(campos[2]));
            const char* departamento = campos[3].c_str();
            const char* municipio = campos[4].c_str();
            uint8_t tipo = static_cast<uint8_t>(std::stoi(campos[5]));
            const char* direccion = campos[6].c_str();
            float precio = std::stof(campos[7]);
            const char* amenidades = campos[8].c_str();
            uint64_t anfitrion_doc = anfitrion->get_documento();
            if (documento_anfitrion == anfitrion_doc) {
                Alojamiento *alojamiento = new Alojamiento(codigo_alojamiento, nombre, documento_anfitrion,
                                                          direccion, departamento, municipio, tipo, precio, amenidades);
                Alojamientos->insert(codigo_alojamiento, alojamiento);
                anfitrion->set_alojamiento(alojamiento);
                num_alojamientos++;
            }
        } catch (const std::exception& e) {
            std::cerr << "Error al convertir campos en línea: " << linea << std::endl;
        }
    }

    archivo.close();
    return Alojamientos;
}

/**
 * @brief Lee el archivo de reservas y carga las reservas para cada alojamiento
 * 
 * @param filename Ruta al archivo de texto.
 * @param alojamientos Mapa hash de alojamientos.
 * @param num_reservas Referencia a una variable donde se guardará la cantidad de reservas leidas
 * @return Mapa hash que contiene las reservas leídas.
*/

static Unordered_Map<uint32_t, Reserva> *leer_reservas(const char* filename, 
                                        Unordered_Map<uint32_t, Alojamiento>**alojamientos, 
                                        size_t &num_reservas, Fecha *fecha_actual) 
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

    Unordered_Map<uint32_t, Reserva>* reservas = new Unordered_Map<uint32_t, Reserva>(num_reservas);

    if (reservas == nullptr) {
        std::cerr << "Error al asignar memoria para las reservas." << std::endl;
        return nullptr;
    }
    std::string campos[CAMPOS_MAX_RESERVA];

    for (size_t reserva = 0; std::getline(archivo, linea); reserva++) {
        uint8_t campos_extraidos = dividir_linea(linea, campos, CAMPOS_MAX_RESERVA);
        if (campos_extraidos < CAMPOS_MAX_RESERVA) {
            std::cerr << "Línea con menos campos de los esperados: " << linea << std::endl;
            continue;
        }

        //Ahora se puede usar el arreglo campos para crear la reserva
        //Usamos un try-catch para manejar errores de conversión
        //y evitar que el programa se detenga
        try {
            const char *fecha_inicio = campos[0].c_str();
            uint16_t duracion = static_cast<uint16_t>(std::stoi(campos[1]));
            uint32_t codigo_reserva = static_cast<uint32_t>(std::stoi(campos[2]));
            uint32_t codigo_alojamiento = static_cast<uint32_t>(std::stoi(campos[3]));
            uint64_t documento_huesped = static_cast<uint64_t>(std::stoull(campos[4]));
            const char metodo_pago = campos[5][0];
            const char *fecha_pago = campos[6].c_str();
            float monto = std::stof(campos[7]);
            const char *anotaciones = campos[8].c_str();

            Fecha *fecha_inicio_obj = new Fecha();
            fecha_inicio_obj->cargar_desde_cadena(fecha_inicio);
            Fecha *fecha_pago_obj = new Fecha();
            fecha_pago_obj->cargar_desde_cadena(fecha_pago);
            Fecha *fecha_fin_obj = fecha_inicio_obj->sumar_noches(duracion);
            Fecha *fecha_final_obj = fecha_inicio_obj->sumar_noches(duracion);
            bool activa = ESTA_ACTIVA((*fecha_final_obj), (*fecha_actual));

            Reserva *reserva = new Reserva(fecha_inicio_obj, fecha_fin_obj, duracion, codigo_reserva,
                                            codigo_alojamiento, documento_huesped, metodo_pago,
                                            fecha_pago_obj, monto, anotaciones, activa);

            Alojamiento *alojamiento = (*alojamientos)->find(reserva->get_codigo_alojamiento());
            if (alojamiento == nullptr) {
                delete reserva;
                reserva = nullptr;
                continue;
            }

            reservas->insert(codigo_reserva, reserva);
            alojamiento->set_reserva(reserva);
            reserva = nullptr;

        } catch (const std::exception& e) {
            std::cerr << "Error al convertir campos en línea: " << linea << std::endl;
        }
    }

    archivo.close();
    return reservas;
}

/**
 * @brief Esta función busca un huesped en el archivo de huéspedes
 * 
 * @param huesped_file Nombre del archivo de huéspedes.
 * @param documento Documento del huésped a buscar.
 * @param password Contraseña del huésped a buscar.
 * @return true si el huésped fue encontrado, false en caso contrario.
 */
static Huesped * buscar_huesped(const char *huesped_file, uint64_t documento, char *password)
{
    bool encontrado = false;
    uint64_t doc;
    char pass[MAX_PASSWORD_LENGTH];
    uint16_t antiguedad;
    float puntuacion;

    std::ifstream archivo(huesped_file);

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo de huéspedes." << std::endl;
        return nullptr;
    }

    while (archivo >> doc >> pass >> antiguedad >> puntuacion) {
        size_t len = strlen(pass) + 1;
        if (doc == documento && memcmp(pass, password, len) == 0) {
            Huesped *huesped = new Huesped(doc, pass, antiguedad, puntuacion);
            return huesped; //Memoria dinamica, la libera el llamador
        }
    }
    archivo.close();
    return nullptr;
}

/**
 * @brief Esta función busca un anfitrion en el archivo de anfitriones
 * 
 * @param huesped_file Nombre del archivo de anfitriones.
 * @param documento Documento del anfitrion a buscar.
 * @param password Contraseña del anfitrion a buscar.
 * @return true si el anfitrion fue encontrado, false en caso contrario.
 */
static Anfitrion * buscar_anfitrion(const char *anfitrion_file, uint64_t documento, char *password)
{
    bool encontrado = false;
    uint64_t doc;
    char pass[MAX_PASSWORD_LENGTH];
    uint16_t antiguedad;
    float puntuacion;

    std::ifstream archivo(anfitrion_file);

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo de anfitriones." << std::endl;
        return nullptr;
    }

    while (archivo >> doc >> pass >> antiguedad >> puntuacion) {
        size_t len = strlen(pass) + 1;
        if (doc == documento && memcmp(pass, password, len) == 0) {
            Anfitrion *anfitrion = new Anfitrion(doc, pass, antiguedad, puntuacion);
            return anfitrion; //Memoria dinamica, la libera el llamador
        }
    }
    archivo.close();
    return nullptr;
}

/**
 * @brief Inicia sesión para un huésped.
 * 
 * @param huesped Referencia a un puntero de tipo Huesped.
 * @return true si el huesped existe, false en caso contrario.
 */
static bool iniciar_sesion_huesped(Huesped **huesped_user)
{
    uint64_t documento;
    char password[MAX_PASSWORD_LENGTH];
    std::cout << "Ingrese su documento: ";
    std::cin >> documento;
    std::cout << "Ingrese su contraseña: ";
    std::cin >> password;

    *huesped_user = buscar_huesped(HUESPED_FILE, documento, password);

    if (*huesped_user == nullptr) {
        ERROR_LOG("iniciar_sesion", "No se encontró el usuario o la contraseña es incorrecta.");
        return false;
    }

    return true;
}

static bool iniciar_sesion_anfitrion(Anfitrion **anfitrion_user)
{
    uint64_t documento;
    char password[MAX_PASSWORD_LENGTH];
    std::cout << "Ingrese su documento: ";
    std::cin >> documento;
    std::cout << "Ingrese su contraseña: ";
    std::cin >> password;

    *anfitrion_user = buscar_anfitrion(ANFITRION_FILE, documento, password);

    if (*anfitrion_user == nullptr) {
        ERROR_LOG("iniciar_sesion", "No se encontró el usuario o la contraseña es incorrecta.");
        return false;
    }

    return true;
}

/**
 * @brief Zona de operaciones para el anfitrión.
 * 
 * Esta función permite al anfitrión realizar operaciones como iniciar sesión,
 * ver reservas, etc.
 */
void zona_anfitrion(void)
{
    Anfitrion *anfitrion_user = nullptr;
    if (!iniciar_sesion_anfitrion(&anfitrion_user)) {
        std::cerr << "Error al iniciar sesión." << std::endl;
        return;
    }

    uint8_t opc = 0;
    Unordered_Map<uint32_t, Alojamiento>* Alojamientos = nullptr;
    Unordered_Map<uint32_t, Reserva>* Reservas = nullptr;
    size_t num_alojamientos = 0;
    Fecha *fecha_actual = new Fecha();
    fecha_actual->cargar_desde_cadena("18/05/2025");

    Alojamientos = leer_alojamientos(ALOJAMIENTO_FILE, anfitrion_user, num_alojamientos);
    Reservas = leer_reservas(RESERVAS_FILE, &Alojamientos, num_alojamientos, fecha_actual);
    char fecha_inicio[LONG_FECHA_CADENA + 1];
    char fecha_fin[LONG_FECHA_CADENA + 1];
    Fecha *fecha_inicio_obj = nullptr;
    Fecha *fecha_fin_obj = nullptr;
    do {
        std::cout << "Bienvenido Anfitrion" << std::endl;
        std::cout << "Bienvenido: " << (anfitrion_user)->get_documento() << std::endl;
        std::cout << "Seleccione:\n1. Consultar reservaciones\n2. Anular reservacion\n3. Salir" << std::endl;
        std::cin >> opc;
        opc = opc - '0'; // Convertir al numerito :)
        switch (opc) {
            case 1:
                std::cout << "Consulta por fecha\nDesde: (dd/mm/aaaa): ";
                std::cin >> fecha_inicio;
                std::cout << "Hasta: (dd/mm/aaaa): ";
                std::cin >> fecha_fin;
                std::cout << "Activas desde: " << fecha_inicio << " hasta: " << fecha_fin << std::endl;
                fecha_inicio_obj = new Fecha();
                fecha_fin_obj = new Fecha();
                if (fecha_inicio_obj->cargar_desde_cadena(fecha_inicio) && fecha_fin_obj->cargar_desde_cadena(fecha_fin)) {
                    anfitrion_user->mostrar_alojamientos(*fecha_inicio_obj, *fecha_fin_obj);
                    delete fecha_inicio_obj;
                    delete fecha_fin_obj;
                } else {
                    std::cerr << "Revise el formato de las fechas" << std::endl;
                }
                break;
            case 2:
                std::cout << "Bienvenido Anfitrion" << std::endl;
                // Anular reservacion
                break;
            case 3:
                std::cout << "Saliendo..." << std::endl;
                return;
            default:
                std::cout << "Opción no válida." << std::endl;
                return;
        }
    } while (opc != 3);

    if (Alojamientos != nullptr) {
        Alojamientos->clear_values();
        delete Alojamientos;
    }

    if (fecha_actual != nullptr) {
        delete fecha_actual;
    }

    if (Reservas != nullptr) {
        Reservas->clear_values();
        delete Reservas;
    }
    delete anfitrion_user; // Liberar memoria del anfitrión
}

/**
 * @brief Zona de operaciones para el huésped.
 * 
 * Esta función permite al huésped realizar operaciones como iniciar sesión,
 * ver reservas, etc.
 */
void zona_huesped(void)
{
    Huesped *huesped_user = nullptr;
    if (!iniciar_sesion_huesped(&huesped_user)) {
        std::cerr << "Error al iniciar sesión." << std::endl;
        return;
    }

    std::cout << "Bienvenido " << (huesped_user)->get_documento() << std::endl;
    std::cout << "Antiguedad: " << (huesped_user)->get_antiguedad() << " meses" << std::endl;
    
    delete huesped_user; // Liberar memoria del huésped
}

void app_main() 
{
    zona_anfitrion();
    /*
    uint8_t opc = 0;
    std::cout << "Bienvenido a la aplicación de reservas." << std::endl;
    std::cout << "Seleccione su perfil:\n1. Huesped\n2. Anfitrion\n3. Salir" << std::endl;
    std::cin >> opc;
    opc = opc - '0'; // Convertir al numerito :)

    switch (opc) {
        case 1:
            std::cout << "Bienvenido Huesped" << std::endl;
            zona_huesped();
            break;
        case 2:
            std::cout << "Bienvenido Anfitrion" << std::endl;
            zona_anfitrion();
            break;
        case 3:
            std::cout << "Saliendo..." << std::endl;
            return;
        default:
            std::cout << "Opción no válida." << std::endl;
            return;
    }*/
    return;
}