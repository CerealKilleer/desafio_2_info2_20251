/**
 * @file app.cpp
 * @brief Implementación de la función principal de la aplicación.
 * 
 * Este archivo contiene la implementación de la función principal de la aplicación
 */

#include "app.hpp"

/**
 * @brief Permite al usuario ingresar un número entero sin signo de 16 bits.
 * @param numero Referencia al número entero a almacenar.
 */

void get_int_16(uint16_t &numero);

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

/**
 * @brief Agrega una reserva a un alojamiento.
 * 
 * @param aloj Alojamiento al que se agregará la reserva.
 * @param codigo_reserva Código de la reserva.
 * @param duracion Duración de la reserva.
 * @param fecha_entrada Fecha de entrada.
 * @param fecha_salida Fecha de salida.
 * @param huesped Huésped que realiza la reserva.
 * @param sistema Fecha del sistema.
 * @return Reserva* Puntero a la nueva reserva creada.
 */

Reserva *agregar_reserva(Alojamiento *aloj, uint32_t codigo_reserva, uint16_t duracion, 
    Fecha *fecha_entrada, Fecha *fecha_salida, Huesped *huesped, Fecha *sistema);

/**
 * @brief Lee el archivo de alojamientos y carga los alojamientos correspondientes a un anfitrion
 * @param filename Ruta al archivo de texto.
 * @param alojamientos Anfitrion que contiene los alojamientos.
 * @param huesped Referencia al huesped
 * @param num_reservas Referencia a una variable donde se guardará la cantidad de alojamientos leidos
 * @param codigo_reserva Referencia a una variable donde se guardará el ultimo codigo de reserva
 * @return Mapa hash que contiene los alojamientos leídos.
 */
static Unordered_Map<uint32_t, Reserva> *leer_reservas(const char* filename, 
                                        Unordered_Map<uint32_t, Alojamiento>**alojamientos,
                                        Huesped *huesped,
                                        size_t &num_reservas, uint32_t &codigo_reserva);


static void obtener_fecha_actual(char* buffer, size_t buffer_size) 
{
    std::time_t tiempo_actual = std::time(nullptr);
    std::tm* tiempo_local = std::localtime(&tiempo_actual);
    std::strftime(buffer, buffer_size, "%d/%m/%Y", tiempo_local);
}

/**
 * @brief Permite al usuario ingresar un número de punto flotante.
 * @param numero Referencia al número de punto flotante a almacenar.
 */
void get_float(float &numero)
{
    numero = 0.0f;
    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) {
        numero = 0.0f;
        return;
    }

    std::stringstream ss(input);
    ss >> numero;

    if (ss.fail())
        numero = 0.0f;
}

/**
 * @brief Permite al usuario ingresar un número entero sin signo de 32 bits.
 * @param numero Referencia al número entero a almacenar.
 */

void get_int(uint32_t &numero)
{
    numero = 0;
    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) {
        numero = 0;
        return;
    }

    std::stringstream ss(input);
    ss >> numero;

    if (ss.fail())
        numero = 0.0f;
}

/**
 * @brief Permite al usuario ingresar un número entero sin signo de 16 bits.
 * @param numero Referencia al número entero a almacenar.
 */

void get_int_16(uint16_t &numero)
{
    numero = 0;
    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) {
        numero = 0;
        return;
    }

    std::stringstream ss(input);
    ss >> numero;

    if (ss.fail())
        numero = 0;
}

/**
 * @brief Permite al usuario ingresar un número entero sin signo de 8 bits.
 * @param numero Referencia al número entero a almacenar.
 */
void get_int_8(uint8_t &numero)
{
    numero = 0;
    std::string input;
    std::getline(std::cin, input);

    if (input.empty()) {
        numero = 0;
        return;
    }

    std::stringstream ss(input);
    ss >> numero;

    if (ss.fail())
        numero = 0;
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
 * @brief Esta función carga los alojamientos desde un archivo de texto
 * y asigna a cada alojamiento su anfitrion correspondiente.
 * 
 * @param filename Ruta al archivo de texto.
 * @param num_reservas Referencia a una variable donde se guardará la cantidad de alojamientos leidos
 * @param anfitriones Mapa hash de anfitriones.
 * @return Mapa hash que contiene las reservas leídas.
*/

static Unordered_Map<uint32_t, Alojamiento>* cargar_alojamientos_completos(const char* filename, Unordered_Map<uint64_t, Anfitrion>* anfitriones, 
                                                                            size_t &num_alojamientos)
{
    std::ifstream archivo(filename);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return 0;
    }

    std::string linea;
    num_alojamientos = 0;

    archivo >> num_alojamientos;
    archivo.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
            Anfitrion* anfitrion = anfitriones->find(documento_anfitrion);
            if (anfitrion != nullptr) {
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
                                        size_t &num_reservas, uint32_t &codigo_reserva)
{
    size_t ciclos_cnt;
    std::ifstream archivo(filename);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo: " << filename << std::endl;
        return 0;
    }

    std::string linea;
    num_reservas = 0;

    archivo >> num_reservas >> codigo_reserva;

    archivo.close();
    archivo.open(filename);

    if (num_reservas == 0) {
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
            Fecha *fecha_final_obj = fecha_inicio_obj->sumar_noches(duracion, ciclos_cnt);

            Reserva *reserva = new Reserva(fecha_inicio_obj, fecha_final_obj, duracion, codigo_reserva,
                                            codigo_alojamiento, documento_huesped, metodo_pago,
                                            fecha_pago_obj, monto, anotaciones);
            
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
    uint64_t doc;
    char pass[MAX_PASSWORD_LENGTH];
    std::string nombre;
    uint16_t antiguedad;
    float puntuacion;
    std::string nombre_huesped;
    std::string campos[CAMPOS_MAX_HUESPED];
    std::string linea;

    std::ifstream archivo(huesped_file);

    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo de huéspedes." << std::endl;
        return nullptr;
    }

    size_t size = 0;
    archivo >> size;

    while (getline(archivo, linea)) {
        uint8_t campos_extraidos = dividir_linea(linea, campos, CAMPOS_MAX_HUESPED);
        if (campos_extraidos < CAMPOS_MAX_HUESPED) {
            std::cerr << "Línea con menos campos de los esperados: " << linea << std::endl;
            continue;
        }

        doc = static_cast<uint64_t>(std::stoull(campos[0]));
        nombre_huesped = campos[1];
        memcpy(pass, campos[2].c_str(), MAX_PASSWORD_LENGTH);
        antiguedad = static_cast<uint16_t>(std::stoi(campos[3]));
        puntuacion = std::stof(campos[4]);

        if (doc == documento && strcmp(pass, password) == 0) {
            Huesped *huesped = new Huesped(doc, pass, nombre_huesped.c_str(), antiguedad, puntuacion);
            return huesped; //Memoria dinamica, la libera el llamador
        }
    }
    archivo.close();
    return nullptr;
}

/**
 * @brief Esta función busca un anfitrion en el archivo de anfitriones
 * 
 * @param anfitrion Nombre del archivo de anfitriones.
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
    size_t size = 0;
    archivo >> size;

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
 * @brief Esta función busca un anfitrion en el archivo de anfitriones
 * 
 * @param anfitrion Nombre del archivo de anfitriones.
 * @param documento Documento del anfitrion a buscar.
 * @param password Contraseña del anfitrion a buscar.
 * @return true si el anfitrion fue encontrado, false en caso contrario.
 */
static Unordered_Map<uint64_t, Anfitrion> *cargar_anfitriones(const char *anfitrion_file)
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
    size_t size = 0;
    archivo >> size;

    Unordered_Map<uint64_t, Anfitrion>* anfitriones = new Unordered_Map<uint64_t, Anfitrion>(RESERVAS_SIZE(size));
    
    while (archivo >> doc >> pass >> antiguedad >> puntuacion) {
        size_t len = strlen(pass) + 1;
        Anfitrion *anfitrion = new Anfitrion(doc, pass, antiguedad, puntuacion);
        anfitriones->insert(doc, anfitrion);
    }

    archivo.close();
    return anfitriones;
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
 * @param num_reservas Número de reservas a escribir.
 * @param codigo_reserva Código de la reserva.
 */
static void escribir_reservas(Unordered_Map<uint32_t, Reserva>* reservas, const char* filename,
                              size_t num_reservas, uint32_t codigo_reserva)
{
    std::ofstream archivo(filename);
    if (!archivo.is_open()) {
        std::cerr << "Error al abrir el archivo para escribir reservas." << std::endl;
        return;
    }
    archivo << num_reservas << " " << codigo_reserva << "\n";
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
    size_t num_reservas = 0;
    size_t num_alojamientos = 0;
    uint32_t codigo_reserva = 0;

    bool update_reservas = false;
    Unordered_Map<uint32_t, Alojamiento>* Alojamientos = nullptr;
    Unordered_Map<uint32_t, Reserva>* Reservas = nullptr;
    
    Fecha *fecha_actual = new Fecha();
    fecha_actual->cargar_desde_cadena("18/05/2025");

    Alojamientos = leer_alojamientos(ALOJAMIENTO_FILE, anfitrion_user, num_alojamientos);
    Reservas = leer_reservas(RESERVAS_FILE, &Alojamientos, nullptr, num_reservas, codigo_reserva);

    Reserva *reserva = nullptr;
    char fecha_inicio[LONG_FECHA_CADENA + 1];
    char fecha_fin[LONG_FECHA_CADENA + 1];
    Fecha *fecha_inicio_obj = nullptr;
    Fecha *fecha_fin_obj = nullptr;
    do {
        std::cout << "Bienvenido Anfitrion" << std::endl;
        std::cout << "Bienvenido: " << (anfitrion_user)->get_documento() << std::endl;
        std::cout << "Seleccione:\n1. Consultar reservaciones\n2. Anular reservacion\n3. Crear historico\n4. Cambiar fecha del sistema\n5. Guardar y salir" << std::endl;
        get_int_8(opc);
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
                    num_reservas--;
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
    } while (opc != 5);


    if (Alojamientos != nullptr) {
        Alojamientos->clear_values();
        delete Alojamientos;
    }

    if (fecha_actual != nullptr) {
        delete fecha_actual;
    }

    if (update_reservas) {
        escribir_reservas(Reservas, RESERVAS_FILE, num_reservas, codigo_reserva);
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
 * @brief Obtiene la fecha de entrada de la futura reservacion
 * @param sistema fecha del sistema
 * @param obj_creados referencia al contador de objetos creados para esta operacion
 * @param ciclos_cnt referencia al contador de ciclos para la operación
 */
static Fecha *obtener_fecha_entrada(Fecha *sistema, size_t &obj_creados, size_t &ciclos_cnt)
{
    char fecha_inicio[LONG_FECHA_CADENA + 1];
    Fecha *fecha_inicio_obj = new Fecha();
    Fecha *fecha_maxima_obj = nullptr;
    std::cout << "Ingrese la fecha de inicio (dd/mm/aaaa): ";
    std::cin >> fecha_inicio;
    fecha_inicio_obj->cargar_desde_cadena(fecha_inicio);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    if (*fecha_inicio_obj < *sistema) {
        delete fecha_inicio_obj;
        std::cerr << "La fecha de inicio no puede ser anterior a la fecha del sistema." << std::endl;
        return nullptr;
    }

    fecha_maxima_obj = fecha_inicio_obj->agregar_anios(1);

    if (*fecha_inicio_obj >= *fecha_maxima_obj) {
        delete fecha_maxima_obj;
        delete fecha_inicio_obj;
        std::cerr << "No se pueden hacer reservas para dentro de más de un año" << std::endl;
        return nullptr;
    }

    delete fecha_maxima_obj;
    obj_creados = fecha_inicio_obj->get_size();
    return fecha_inicio_obj;
}

static Fecha *obtener_duracion_reserva(uint16_t &duracion, Fecha *inicio_reservacion, size_t ciclos_cnt)
{
    std::cout << "Escriba la duración en noches de su reservación" << std::endl;
    get_int_16(duracion);
    
    Fecha *fin_reservacion = inicio_reservacion->sumar_noches(duracion, ciclos_cnt);
    Fecha *maxima = inicio_reservacion->agregar_anios(1);

    if (*fin_reservacion > *maxima) {
        delete fin_reservacion;
        delete maxima;
        std::cerr << "No se puede reservar hasta una fecha posterior a un año" << std::endl;
        duracion = 0;
        return nullptr;
    }

    return fin_reservacion;
}

/**
 * @brief Callback para validar alojamientos. Esta función la llama el mapa hash
 * @param codigo_alojamiento Código del alojamiento.
 * @param aloj Puntero al alojamiento a validar.
 * @param params Parámetros adicionales para la validación.
 */
static void validar_alojamientos(uint32_t codigo_alojamiento, Alojamiento* aloj, void* params)
{
    callback_param_reservacion *param = reinterpret_cast<callback_param_reservacion*>(params);
    if (aloj == nullptr)
        return;

    if(aloj->es_candidato_reserva(*param->inicio, *param->fin, *(param->mpo))) {
        param->alojamientos->insert_front(aloj);
    }
}

bool mostrar_alojamientos_disponibles(Linked_List<Alojamiento*> *alojamientos, 
                                    Linked_List<Alojamiento*> *disponibles, 
                                    Unordered_Map<uint64_t, Anfitrion> *anfitrion, 
                                    float precio, float puntuacion)
{
    bool encontrado = false;
    if (alojamientos == nullptr || alojamientos->get_size() == 0) {
        std::cerr << "No se encontraron alojamientos disponibles." << std::endl;
        return encontrado;
    }

    Node<Alojamiento*> *current = alojamientos->get_head();
    while (current != nullptr) {
        Alojamiento *alojamiento = current->data;
        Anfitrion *anfitrion_user = anfitrion->find(alojamiento->get_codigo_anfitrion());
        if (anfitrion_user != nullptr) {
            bool cumple_precio = (precio == 0.0f || alojamiento->get_precio() <= precio);
            bool cumple_puntuacion = (puntuacion == 0.0f || anfitrion_user->get_puntuacion() >= puntuacion);

            if (cumple_precio && cumple_puntuacion) {
                encontrado = true;
                disponibles->insert_front(alojamiento);
                alojamiento->mostrar_alojamiento();
                std::cout << "Calificación: " << anfitrion_user->get_puntuacion() << std::endl;
            }
        }
        current = current->next;
    }

    return encontrado;
}

/**
 * @brief Verifica si un alojamiento existe en la lista de alojamientos.
 * @param alojamientos Lista de alojamientos.
 * @param codigo_alojamiento Código del alojamiento a buscar.
 * @return true si el alojamiento existe, false en caso contrario.
 */

Alojamiento *existe_alojamiento(Linked_List<Alojamiento*> *alojamientos, uint32_t codigo_alojamiento)
{
    if (alojamientos == nullptr || alojamientos->get_size() == 0)
        return nullptr;

    Node<Alojamiento*> *current = alojamientos->get_head();
    while (current != nullptr) {
        if (current->data->get_id() == codigo_alojamiento)
            return current->data;
        current = current->next;
    }
    return nullptr;
}

static Reserva *crear_reservacion_codigo(Unordered_Map<uint32_t, Alojamiento> *Alojamientos, 
    Unordered_Map<uint64_t, Anfitrion> *Anfitriones, Fecha *sistema, uint32_t &codigo_reserva,
    Huesped *huesped)
{
    uint16_t duracion;
    size_t ciclos_cnt = 0;
    size_t obj_creados = 0;
    uint32_t codigo_alojamiento;

    std::cout << "Ingrese el código del alojamiento: ";
    get_int(codigo_alojamiento);
    Alojamiento *alojamiento = Alojamientos->find(codigo_alojamiento);

    if (alojamiento == nullptr) {
        std::cerr << "El alojamiento no existe." << std::endl;
        return nullptr;
    }
    alojamiento->mostrar_alojamiento();
    //Se obtiene la fecha de inicio
    Fecha *inicio_reservacion = obtener_fecha_entrada(sistema, obj_creados, ciclos_cnt);
    Fecha *finalizacion_reservacion = nullptr;

    if (inicio_reservacion == nullptr) {
        std::cerr << "No se creó la reservación. " << "Se usaron: " << ciclos_cnt << " ciclos" << std::endl;
        std::cerr << "Los objetos creados ocupan: " << "0" << " bytes" << std::endl; 
        return nullptr;
    }

    //Se obtiene la duración y, por tanto, la fecha de finalización
    finalizacion_reservacion = obtener_duracion_reserva(duracion, inicio_reservacion, ciclos_cnt);

    if (finalizacion_reservacion == nullptr) {
        delete inicio_reservacion;
        std::cerr << "No se creó la reservación. " << "Se usaron: " << ciclos_cnt << " ciclos" << std::endl;
        std::cerr << "Los objetos creados ocupan: " << "0" << " bytes" << std::endl;
        return nullptr;
    }

    if (huesped->tengo_reservas(inicio_reservacion, finalizacion_reservacion)) {
        delete inicio_reservacion;
        delete finalizacion_reservacion;
        std::cerr << "El huésped ya tiene reservas en esas fechas." << std::endl;
        std::cerr << "No se creó la reservación. " << "Se usaron: " << ciclos_cnt << " ciclos" << std::endl;
        std::cerr << "Los objetos creados ocupan: " << "0" << " bytes" << std::endl;
        return nullptr;
    }

    if (!alojamiento->es_candidato_reserva(*inicio_reservacion, *finalizacion_reservacion)) {
        std::cerr << "El alojamiento no está disponible en esas fechas." << std::endl;
        delete inicio_reservacion;
        delete finalizacion_reservacion;
        std::cerr << "No se creó la reservación. " << "Se usaron: " << ciclos_cnt << " ciclos" << std::endl;
        std::cerr << "Los objetos creados ocupan: " << "0" << " bytes" << std::endl;
        return nullptr;
    }

    Reserva *reserva = agregar_reserva(alojamiento, codigo_reserva, duracion, inicio_reservacion, 
                                       finalizacion_reservacion, huesped, sistema);
    
    reserva->mostrar();
    return reserva;
    
}
/**
 * @brief Crea una nueva reservacion
 * @param Alojamientos Mapa de alojamientos
 * @param Fecha del sistema
 * @return Puntero a la nueva reservacion
 */
static Reserva * crear_reservacion(Unordered_Map<uint32_t, Alojamiento> *Alojamientos, 
    Unordered_Map<uint64_t, Anfitrion> *Anfitriones, Fecha *sistema, uint32_t &codigo_reserva,
    Huesped *huesped)
{
    //Todas las variables o estructuras necesarias para crear la reservación
    uint16_t duracion;
    std::string municipio;
    struct callback_param_reservacion params = {nullptr, nullptr, nullptr, nullptr};
    size_t ciclos_cnt = 0;
    size_t obj_creados = 0;
    std::string departamento;
    float puntuacion, precio;
    char fecha[LONG_FECHA_CADENA + 1] = {0};
    //Se obtiene la fecha de inicio
    Fecha *inicio_reservacion = obtener_fecha_entrada(sistema, obj_creados, ciclos_cnt);
    Fecha *finalizacion_reservacion = nullptr;

    if (inicio_reservacion == nullptr) {
        std::cerr << "No se creó la reservación. " << "Se usaron: " << ciclos_cnt << " ciclos" << std::endl;
        std::cerr << "Los objetos creados ocupan: " << "0" << " bytes" << std::endl; 
        return nullptr;
    }

    //Se obtiene la duración y, por tanto, la fecha de finalización
    finalizacion_reservacion = obtener_duracion_reserva(duracion, inicio_reservacion, ciclos_cnt);

    if (huesped->tengo_reservas(inicio_reservacion, finalizacion_reservacion)) {
        delete inicio_reservacion;
        delete finalizacion_reservacion;
        std::cerr << "El huésped ya tiene reservas en esas fechas." << std::endl;
        std::cerr << "No se creó la reservación. " << "Se usaron: " << ciclos_cnt << " ciclos" << std::endl;
        std::cerr << "Los objetos creados ocupan: " << "0" << " bytes" << std::endl;
        return nullptr;
    }

    if (finalizacion_reservacion == nullptr) {
        obj_creados -= inicio_reservacion->get_size();
        delete inicio_reservacion;
        std::cerr << "No se creó la reservación. " << "Se usaron: " << ciclos_cnt << " ciclos" << std::endl;
        std::cerr << "Los objetos creados ocupan: " << "0" << " bytes" << std::endl;
        return nullptr;
    }

    //Se aplica el filtro
    std::cout << "Ingrese el municipio: ";
    getline(std::cin, municipio);
    params.inicio = inicio_reservacion;
    params.fin = finalizacion_reservacion;
    params.mpo = &municipio;

    //Se crea la lista de alojamientos disponibles
    params.alojamientos = new Linked_List<Alojamiento*>();

    //Se valida que los alojamientos existan y estén disponibles
    Alojamientos->for_each(validar_alojamientos, &params);

    if (params.alojamientos->get_size() == 0) {
        delete params.alojamientos;
        delete inicio_reservacion;
        delete finalizacion_reservacion;
        std::cerr << "No se encontraron alojamientos disponibles." << std::endl;
        std::cerr << "No se creó la reservación. " << "Se usaron: " << ciclos_cnt << " ciclos" << std::endl;
        std::cerr << "Los objetos creados ocupan: " << "0" << " bytes" << std::endl;
        return nullptr;
    }

    std::cout << "Ahora podrá elegir filtros adicionales para su reservación. Simplemente presione ENTER para dejar los campos vacíos" << std::endl;
    std::cout << "Precio maximo por noche: ";
    get_float(precio);
    std::cout << "Mínima calificación anfitrión: ";
    get_float(puntuacion);

    params.alojamientos_disponibles = new Linked_List<Alojamiento*>();

    if (!mostrar_alojamientos_disponibles(params.alojamientos, params.alojamientos_disponibles, 
                                        Anfitriones, precio, puntuacion)) {
        delete params.alojamientos;
        delete inicio_reservacion;
        delete finalizacion_reservacion;
        std::cerr << "No se encontraron alojamientos disponibles." << std::endl;
        std::cerr << "No se creó la reservación. " << "Se usaron: " << ciclos_cnt << " ciclos" << std::endl;
        std::cerr << "Los objetos creados ocupan: " << "0" << " bytes" << std::endl;
        return nullptr;
    }

    delete params.alojamientos;

    //Se muestran los alojamientos disponibles y se permite al usuario elegir uno
    std::cout << "Seleccione el código del alojamiento que desea reservar: ";
    uint32_t codigo_alojamiento;
    get_int(codigo_alojamiento);

    Alojamiento *aloj = existe_alojamiento(params.alojamientos_disponibles, codigo_alojamiento);

    if (aloj == nullptr) {
        delete params.alojamientos_disponibles;
        delete inicio_reservacion;
        delete finalizacion_reservacion;
        std::cerr << "El alojamiento no existe." << std::endl;
        std::cerr << "No se creó la reservación. " << "Se usaron: " << ciclos_cnt << " ciclos" << std::endl;
        std::cerr << "Los objetos creados ocupan: " << "0" << " bytes" << std::endl;
        return nullptr;
    }
    delete params.alojamientos_disponibles;
    //Ahora se crea la reserva
    Reserva *reserva = agregar_reserva(aloj, codigo_reserva, duracion, inicio_reservacion, 
                                       finalizacion_reservacion, huesped, sistema);
    
    reserva->mostrar();
    
    return reserva;    
}

/**
 * @brief Agrega una reserva a un alojamiento.
 * 
 * @param aloj Alojamiento al que se agregará la reserva.
 * @param codigo_reserva Código de la reserva.
 * @param duracion Duración de la reserva.
 * @param fecha_entrada Fecha de entrada.
 * @param fecha_salida Fecha de salida.
 * @param huesped Huésped que realiza la reserva.
 * @param sistema Fecha del sistema.
 * @return Reserva* Puntero a la nueva reserva creada.
 */

Reserva *agregar_reserva(Alojamiento *aloj, uint32_t codigo_reserva, uint16_t duracion, 
    Fecha *fecha_entrada, Fecha *fecha_salida, Huesped *huesped, Fecha *sistema)
{
    char pago = 0;

    while (pago != 'T' && pago != 'P') {
        std::cout << "Ingrese el método de pago (T: T.Credito, P: PSE): ";
        std::cin >> pago;
    }

    float monto = aloj->get_precio() * duracion;
    std::string anotaciones;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    char fch_pago[LONG_FECHA_CADENA + 1] = {0};
    Fecha *fecha_pago = new Fecha();
    bool valida = false;

    do {
        std::cout << "Escriba la fecha de pago (dd/mm/aaaa): ";
        std::cin.getline(fch_pago, LONG_FECHA_CADENA + 1);
        
        bool valida = fecha_pago->cargar_desde_cadena(fch_pago);
    } while (!valida && (*fecha_pago > *sistema));

    do {
        std::cout << "Escriba sus anotaciones (1000 caracteres max): ";
        std::getline(std::cin, anotaciones);
    } while (anotaciones.length() > 1000);
    
    const char *notas = anotaciones.c_str();
    codigo_reserva++;

    Reserva *reserva = new Reserva(fecha_entrada,fecha_salida, duracion,
                 codigo_reserva, aloj->get_id(),
                 huesped->get_documento(), pago,
                 fecha_pago, monto,
                 notas);
    
    huesped->set_reserva(reserva);
    aloj->set_reserva(reserva);
    return reserva;
}
/**
 * @brief Presenta el menú con las opciones de reservación.
 * @param Alojamientos Mapa de alojamientos.
 * @param Anfitriones Mapa de anfitriones.
 * @param sistema Fecha del sistema.
 * @param codigo_reserva Código de la reserva.
 * @param huesped Huésped que realiza la reserva.
 * @return Reserva* Puntero a la nueva reserva creada.
 */
Reserva *menu_reservacion(Unordered_Map<uint32_t, Alojamiento> *Alojamientos, 
    Unordered_Map<uint64_t, Anfitrion> *Anfitriones, Fecha *sistema, uint32_t &codigo_reserva,
    Huesped *huesped)
{
    uint8_t opc = 0;
    Reserva *reserva = nullptr;

    do {
        std::cout << "Bienvenido a la sección de reservaciones" << std::endl;
        std::cout << "Seleccione:\n1. Crear reservación por código\n2. Crear reservación con filtros\n3. Salir" << std::endl;
        get_int_8(opc);
        opc = opc - '0'; // Convertir al numerito :)
    } while (opc < 1 || opc > 3);
    
    switch (opc) {
        case 1:
            std::cout << "Crear reservación por código" << std::endl;
            reserva = crear_reservacion_codigo(Alojamientos, Anfitriones, sistema, codigo_reserva, huesped);
            break;
        case 2:
            std::cout << "Crear reservación con filtros" << std::endl;
            reserva = crear_reservacion(Alojamientos, Anfitriones, sistema, codigo_reserva, huesped);
            break;
        case 3:
            std::cout << "Saliendo..." << std::endl;
            break;
        default:
            std::cout << "Opción no válida." << std::endl;
            break;
    }
    
    return reserva;
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
    Unordered_Map<uint64_t, Anfitrion>* Anfitriones = nullptr;

    size_t num_reservas = 0;
    bool update_reservas = false;
    uint32_t codigo_reserva;
    uint32_t cod_buscar_reserva;
    uint8_t opc;
    Anfitriones = cargar_anfitriones(ANFITRION_FILE);
    Alojamientos = cargar_alojamientos_completos(ALOJAMIENTO_FILE, Anfitriones, num_reservas);
    Reservas = leer_reservas(RESERVAS_FILE, &Alojamientos, huesped_user, num_reservas, codigo_reserva);

    Reserva *reserva = nullptr;
    get_int_8(opc);

    do {
        std::cout << "Bienvenido Huesped" << std::endl;
        std::cout << "Bienvenido: " << (huesped_user)->get_documento() << std::endl;
        std::cout << "Seleccione:\n1. Anular reservaciones\n2. Hacer reservaciones\n3.Guardar y Salir" << std::endl;
        get_int_8(opc);
        opc = opc - '0'; // Convertir al numerito :)
        switch (opc) {
            case 1:
                std::cout << "Anular reservación" << std::endl;
                std::cout << "Ingrese el código de la reserva a anular: ";
                
                get_int(cod_buscar_reserva);
                reserva = Reservas->find(cod_buscar_reserva);
                if (reserva == nullptr) {
                    std::cerr << "No se encontró la reserva con el código: " << cod_buscar_reserva << std::endl;
                    break;
                }
                if (huesped_user->eliminar_reserva(reserva)) {
                    num_reservas--;
                    escribir_cancelaciones(reserva, CANCELACIONES_FILE);
                    delete Reservas->erase(cod_buscar_reserva);
                    update_reservas = true;
                }
                break;
            case 2:
                std::cout << "Crear reservación" << std::endl;
                reserva = menu_reservacion(Alojamientos, Anfitriones, fecha_sistema, codigo_reserva, huesped_user);
                if(reserva != nullptr) {
                    codigo_reserva = reserva->get_codigo_reserva();
                    num_reservas++;
                    Reservas->insert(codigo_reserva, reserva);
                    huesped_user->mostrar_reserva_huesped(reserva);
                    update_reservas = true;
                }
                break;
            case 3:
                std::cout << "Saliendo..." << std::endl;
                break;
            default:
                std::cout << "Opción no válida." << std::endl;
                break;
        }
    } while (opc != 3);
    
    if (Anfitriones != nullptr) {
        Anfitriones->clear_values();
        delete Anfitriones;
    }

    if (Alojamientos != nullptr) {
        Alojamientos->clear_values();
        delete Alojamientos;
    }

    if (update_reservas) {
        escribir_reservas(Reservas, RESERVAS_FILE, num_reservas, codigo_reserva);
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

    //zona_anfitrion(fecha_sistema);

    zona_huesped(fecha_sistema);
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