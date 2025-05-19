/**
 * @file app.cpp
 * @brief Implementación de la función principal de la aplicación.
 * 
 * Este archivo contiene la implementación de la función principal de la aplicación
 */

#include "app.hpp"

/**
 * @brief Crea un archivo de histórico de reservas a partir de las reservas activas del sistema.
 * 
 * @param reservas Mapa a las reservas.
 * @param num_reservas Número de reservas en el arreglo.
 * @param historico_file Ruta del archivo donde se guardará el histórico.
 */
static bool crear_historico_reservas(Unordered_Map<uint32_t, Reserva>* Reservas, const char* filename, Anfitrion* anfitrion, Fecha *fecha_sistema);

/**
 * @brief Busca un huésped en el archivo dado a partir de su documento y contraseña.
 * 
 * @param huesped_file Ruta al archivo que contiene los datos de los huéspedes.
 * @param documento Documento de identidad del huésped a buscar.
 * @param password Contraseña del huésped (para validación).
 * @return Puntero al huésped si se encuentra y valida, nullptr en caso contrario.
 */
static Huesped* buscar_huesped(const char* huesped_file, uint64_t documento, char* password);

/**
 * @brief Busca un anfitrión en el archivo dado a partir de su documento y contraseña.
 * 
 * @param anfitrion_file Ruta al archivo que contiene los datos de los anfitriones.
 * @param documento Documento de identidad del anfitrión a buscar.
 * @param password Contraseña del anfitrión (para validación).
 * @return Puntero al anfitrión si se encuentra y valida, nullptr en caso contrario.
 */
static Anfitrion* buscar_anfitrion(const char* anfitrion_file, uint64_t documento, char* password);

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
 * @param huespedes Mapa hash de huespedes.
 * @param num_reservas Referencia a una variable donde se guardará la cantidad de reservas leidas
 * @return Mapa hash que contiene las reservas leídas.
*/

static Unordered_Map<uint32_t, Reserva> *leer_reservas(const char* filename, 
                                        Unordered_Map<uint32_t, Alojamiento>**alojamientos,
                                        Huesped *huesped,
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
                                        Huesped *huesped,
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
    Alojamiento *alojamiento = nullptr;

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
            Fecha *fecha_final_obj = fecha_inicio_obj->sumar_noches(duracion);
            bool activa = ESTA_ACTIVA((*fecha_final_obj), (*fecha_actual));

            Reserva *reserva = new Reserva(fecha_inicio_obj, fecha_final_obj, duracion, codigo_reserva,
                                            codigo_alojamiento, documento_huesped, metodo_pago,
                                            fecha_pago_obj, monto, anotaciones, activa);
            
            reservas->insert(codigo_reserva, reserva);
            if (*alojamientos != nullptr) {
                alojamiento = (*alojamientos)->find(reserva->get_codigo_alojamiento());
            }
                
            if (alojamiento != nullptr)
                alojamiento->set_reserva(reserva);

            if (huesped != nullptr) {
                if (reserva->get_documento_huesped() == (huesped)->get_documento()) {
                    (huesped)->set_reserva(reserva);
                }
            }

        } catch (const std::exception& e) {
            std::cerr << "Error al convertir campos en línea: " << linea << std::endl;
            continue;
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

/**
 * @brief Inicia sesión para un anfitrión.
 * @param anfitrion_user Referencia a un puntero de tipo Anfitrion.
 */
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

static void escribir_cancelaciones(Reserva *reserva, const char *filename)
{
    std::ofstream archivo(filename, std::ios::app);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo para escribir cancelaciones." << std::endl;
        return;
    }

    char buffer[LONG_FECHA_CADENA + 1] = {0};
    reserva->get_fecha_entrada()->a_cadena(buffer);
    archivo << buffer << ";";
    archivo << reserva->get_duracion() << ";";
    archivo << reserva->get_codigo_reserva() << ";";
    archivo << reserva->get_codigo_alojamiento() << ";";
    archivo << reserva->get_documento_huesped() << ";";
    archivo << reserva->get_metodo_pago() << ";";

    reserva->get_fecha_pago()->a_cadena(buffer);
    archivo << buffer << ";";
    archivo << reserva->get_monto() << ";";
    archivo << (reserva->get_anotaciones() ? reserva->get_anotaciones() : "") << "\n";

    archivo.close();
}
/**
 * @brief Callback para escribir una reserva en un archivo.
 * 
 * @param codigo_reserva Código de la reserva.
 * @param reserva Reserva a escribir.
 * @param archivo Puntero al archivo donde se escribirá la reserva.
 */
static void escribir_reserva_callback(uint32_t codigo_reserva, Reserva* reserva, void* archivo_void)
{
    if (reserva != nullptr) {
        std::ofstream& archivo = *reinterpret_cast<std::ofstream*>(archivo_void);
        char buffer[LONG_FECHA_CADENA + 1] = {0};

        reserva->get_fecha_entrada()->a_cadena(buffer);
        archivo << buffer << ";";
        archivo << reserva->get_duracion() << ";";
        archivo << reserva->get_codigo_reserva() << ";";
        archivo << reserva->get_codigo_alojamiento() << ";";
        archivo << reserva->get_documento_huesped() << ";";
        archivo << reserva->get_metodo_pago() << ";";

        reserva->get_fecha_pago()->a_cadena(buffer);
        archivo << buffer << ";";
        archivo << reserva->get_monto() << ";";
        archivo << (reserva->get_anotaciones() ? reserva->get_anotaciones() : "") << "\n";
    }
}


/**
 * @brief Escribe las reservas en un archivo de texto.
 * 
 * @param reservas Mapa hash que contiene las reservas a escribir.
 * @param filename Nombre del archivo donde se guardarán las reservas.
 */
static void escribir_reservas(Unordered_Map<uint32_t, Reserva>* reservas, const char* filename)
{
    std::ofstream archivo(filename);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo para escribir reservas." << std::endl;
        return;
    }

    //Aquí vengo a usar un poquito de programación funcional y aprovecharme de características de C++
    //Lo que llaman sugar syntax
    //El callback se encarga de escribir la reserva en el archivo
    reservas->for_each(escribir_reserva_callback, &archivo);
    archivo.close();
}

/**
 * @brief Zona de operaciones para el anfitrión.
 * 
 * Esta función permite al anfitrión realizar operaciones como iniciar sesión,
 * ver reservas, etc.
 */
void zona_anfitrion(Fecha *fecha_sistema)
{
    Anfitrion *anfitrion_user = nullptr;
    if (!iniciar_sesion_anfitrion(&anfitrion_user)) {
        std::cerr << "Error al iniciar sesión." << std::endl;
        return;
    }

    uint8_t opc = 0;
    bool update_reservas = false;
    Unordered_Map<uint32_t, Alojamiento>* Alojamientos = nullptr;
    Unordered_Map<uint32_t, Reserva>* Reservas = nullptr;
    size_t num_alojamientos = 0;
    Fecha *fecha_actual = new Fecha();
    fecha_actual->cargar_desde_cadena("18/05/2025");

    Alojamientos = leer_alojamientos(ALOJAMIENTO_FILE, anfitrion_user, num_alojamientos);
    Reservas = leer_reservas(RESERVAS_FILE, &Alojamientos, nullptr, num_alojamientos, fecha_actual);
    Reserva *reserva = nullptr;
    char fecha_inicio[LONG_FECHA_CADENA + 1];
    char fecha_fin[LONG_FECHA_CADENA + 1];
    Fecha *fecha_inicio_obj = nullptr;
    Fecha *fecha_fin_obj = nullptr;
    do {
        std::cout << "Bienvenido Anfitrion" << std::endl;
        std::cout << "Bienvenido: " << (anfitrion_user)->get_documento() << std::endl;
        std::cout << "Seleccione:\n1. Consultar reservaciones\n2. Anular reservacion\n3. Crear historico\n4. Cambiar fecha del sistema\n5. Guardar y salir" << std::endl;
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
                if (fecha_inicio_obj->cargar_desde_cadena(fecha_inicio) && fecha_fin_obj->cargar_desde_cadena(fecha_fin))
                    anfitrion_user->mostrar_alojamientos(*fecha_inicio_obj, *fecha_fin_obj);
                else 
                    std::cerr << "Revise el formato de las fechas" << std::endl;
                delete fecha_inicio_obj;
                delete fecha_fin_obj;
                break;
            case 2:
                std::cout << "Anular reservación" << std::endl;
                std::cout << "Ingrese el código de la reserva a anular: ";
                uint32_t codigo_reserva;
                std::cin >> codigo_reserva;
                reserva = Reservas->find(codigo_reserva);

                if (reserva == nullptr) {
                    std::cerr << "No se encontró la reserva con el código: " << codigo_reserva << std::endl;
                    break;
                }

                if (anfitrion_user->eliminar_reserva(reserva)) {
                    escribir_cancelaciones(reserva, CANCELACIONES_FILE);
                    delete Reservas->erase(codigo_reserva);
                    update_reservas = true;
                }

                break;
            case 3:
                std::cout << "Crear histórico de reservas" << std::endl;
                update_reservas = crear_historico_reservas(Reservas, HISTORICO_FILE, anfitrion_user, fecha_actual);
                break;
            case 4:
                std::cout << "Cambiar fecha del sistema" << std::endl;
                std::cout << "Ingrese la nueva fecha (dd/mm/aaaa): ";
                char nueva_fecha[LONG_FECHA_CADENA + 1];
                std::cin >> nueva_fecha;
                if (fecha_actual->cargar_desde_cadena(nueva_fecha)) {
                    std::cout << "Fecha del sistema actualizada a: " << nueva_fecha << std::endl;
                } else {
                    std::cerr << "Error al cargar la fecha." << std::endl;
                }
                break;
            case 5:
                std::cout << "Saliendo..." << std::endl;
                break;
            default:
                std::cout << "Opción no válida." << std::endl;
                break;
        }
    } while (opc != 4);


    if (Alojamientos != nullptr) {
        Alojamientos->clear_values();
        delete Alojamientos;
    }

    if (fecha_actual != nullptr) {
        delete fecha_actual;
    }

    if (update_reservas) {
        escribir_reservas(Reservas, RESERVAS_FILE);
        std::cout << "Reservas actualizadas." << std::endl;
    }

    if (Reservas != nullptr) {
        Reservas->clear_values();
        delete Reservas;
    }
    delete anfitrion_user; // Liberar memoria del anfitrión
}

/**
 * @brief Compara dos fechas.
 * @param reserva_1 Reserva inicial.
 * @param reserva_2 Reserva final.
 * @return true si la fecha de salida de la reserva 1 es menor que la de la reserva 2.
 */
static bool comparar_fecha_reservas(Reserva *reserva_1, Reserva *reserva_2)
{
    return (*reserva_1->get_fecha_salida()) < (*reserva_2->get_fecha_salida());
}
/**
 * @brief Agrega una reserva al histórico de reservas y la elimina de las reservas del anfitrión.
 * @param codigo_reserva Código de la reserva.
 * @param reserva Puntero a la reserva a agregar al histórico.
 * @param archivo Puntero al archivo donde se escribirá el histórico.
 * @param fecha Fecha de corte para el histórico.
 * @param anfitrion Puntero al anfitrión que está creando el histórico.
 * @param params Parámetros adicionales para el callback (archivo, fecha, anfitrion, historico)
 */
static void agregar_historico_reservas(uint32_t codigo_reserva, Reserva* reserva, void* params)
{
    callback_param_historico *param = reinterpret_cast<callback_param_historico*>(params);
    if (reserva == nullptr)
        return;

    if (*(reserva->get_fecha_salida()) >= *(param->fecha))
        return;
    
    param->anfitrion->eliminar_reserva(reserva);
    param->historico->insert_sorted(reserva, comparar_fecha_reservas);
}

/**
 * @brief Crea un archivo histórico de reservas.
 * 
 * @param Reservas Mapa hash que contiene las reservas a escribir.
 * @param filename Nombre del archivo donde se guardarán las reservas.
 * @param anfitrion Puntero al anfitrión que está creando el histórico.
 * @param fecha_sistema Fecha del sistema actual.
 */
static bool crear_historico_reservas(Unordered_Map<uint32_t, Reserva>* Reservas, const char* filename, Anfitrion* anfitrion, Fecha *fecha_sistema)
{
    std::ofstream archivo(filename, std::ios::app);
    struct callback_param_historico params = {&archivo, fecha_sistema, anfitrion};
    params.historico = new Linked_List<Reserva*>();

    if (params.historico == nullptr) {
        std::cerr << "Error al crear la lista de histórico." << std::endl;
        return false;
    }

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo para escribir el historico." << std::endl;
        return false;
    }

    Reservas->for_each(agregar_historico_reservas, &params);
    
    if (params.historico->get_size() == 0) {
        delete params.historico;
        return false;
    }
    std::cout << "Histórico de reservas creado con éxito." << std::endl;
    Node<Reserva*> *current = params.historico->get_head();
    char buffer[LONG_FECHA_CADENA + 1] = {0};

    while (current != nullptr) {
        current->data->get_fecha_entrada()->a_cadena(buffer);
        archivo << buffer << ";";
        archivo << current->data->get_duracion() << ";";
        archivo << current->data->get_codigo_reserva() << ";";
        archivo << current->data->get_codigo_alojamiento() << ";";
        archivo << current->data->get_documento_huesped() << ";";
        archivo << current->data->get_metodo_pago() << ";";
        archivo << current->data->get_fecha_pago()->a_cadena(buffer) << ";";
        archivo << current->data->get_monto() << ";";
        archivo << (current->data->get_anotaciones() ? current->data->get_anotaciones() : "") << "\n";
        delete Reservas->erase(current->data->get_codigo_reserva());
        current = current->next;
    }
    
    delete params.historico;
    params.historico = nullptr;
    archivo.close();
    
    return true;
}
/**
 * @brief Zona de operaciones para el huésped.
 * 
 * Esta función permite al huésped realizar operaciones como iniciar sesión,
 * ver reservas, etc.
 */
void zona_huesped(Fecha *fecha_sistema)
{
    Huesped *huesped_user = nullptr;
    if (!iniciar_sesion_huesped(&huesped_user)) {
        std::cerr << "Error al iniciar sesión." << std::endl;
        return;
    }

    Unordered_Map<uint32_t, Reserva>* Reservas = nullptr;
    Unordered_Map<uint32_t, Alojamiento>* Alojamientos = nullptr;
    size_t num_reservas = 0;
    bool update_reservas = false;
    Reservas = leer_reservas(RESERVAS_FILE, &Alojamientos, huesped_user, num_reservas, fecha_sistema);
    Reserva *reserva = nullptr;
    uint8_t opc = 0;
    do {
        std::cout << "Bienvenido Huesped" << std::endl;
        std::cout << "Bienvenido: " << (huesped_user)->get_documento() << std::endl;
        std::cout << "Seleccione:\n1. Anular reservaciones\n2. Hacer reservaciones\n3.Guardar y Salir" << std::endl;
        std::cin >> opc;
        opc = opc - '0'; // Convertir al numerito :)
        switch (opc) {
            case 1:
                std::cout << "Anular reservación" << std::endl;
                std::cout << "Ingrese el código de la reserva a anular: ";
                uint32_t codigo_reserva;
                std::cin >> codigo_reserva;
                reserva = Reservas->find(codigo_reserva);
                if (reserva == nullptr) {
                    std::cerr << "No se encontró la reserva con el código: " << codigo_reserva << std::endl;
                    break;
                }
                if (huesped_user->eliminar_reserva(reserva)) {
                    escribir_cancelaciones(reserva, CANCELACIONES_FILE);
                    delete Reservas->erase(codigo_reserva);
                    update_reservas = true;
                }
                break;
            case 2:
                std::cout << "Crear reservación" << std::endl;
                break;
            case 3:
                std::cout << "Saliendo..." << std::endl;
                break;
            default:
                std::cout << "Opción no válida." << std::endl;
                break;
        }
    } while (opc != 4);
    
    if (Alojamientos != nullptr) {
        Alojamientos->clear_values();
        delete Alojamientos;
    }

    if (update_reservas) {
        escribir_reservas(Reservas, RESERVAS_FILE);
        std::cout << "Reservas actualizadas." << std::endl;
    }

    if (Reservas != nullptr) {
        Reservas->clear_values();
        delete Reservas;
    }

    delete huesped_user; // Liberar memoria del huésped
}

void app_main() 
{
    Fecha *fecha_sistema = new Fecha();
    char fecha[LONG_FECHA_CADENA + 1] = {0};
    obtener_fecha_actual(fecha, LONG_FECHA_CADENA + 1);
    std::cout << "Fecha del sistema: " << fecha << std::endl;
    fecha_sistema->cargar_desde_cadena(fecha);

    zona_anfitrion(fecha_sistema);

    //zona_huesped(fecha_sistema);
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
    delete fecha_sistema;
}