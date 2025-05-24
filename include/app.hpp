#ifndef __APP_H__
#define __APP_H__

#define MAX_PASSWORD_LENGTH 20
#define SUCCESS_LOG(fn, msg) std::cout << "[App/" << fn << "]: " << msg << std::endl
#define ERROR_LOG(fn, msg) std::cerr << "[App/" << fn << "]: " << msg << std::endl
#define CAMPOS_MAX_ALOJAMIENTO 9
#define CAMPOS_MAX_RESERVA 9
#define CAMPOS_MAX_HUESPED 5
#define RESERVAS_SIZE(size) ((size * 1.5) + 1) // Redimensiona la tabla hash al 150% de su tamaño original
#define HUESPED_FILE "huespedes.txt"
#define ANFITRION_FILE "anfitriones.txt"
#define ALOJAMIENTO_FILE "alojamientos.txt"
#define RESERVAS_FILE "reservaciones.txt"
#define HISTORICO_FILE "historico.txt"
#define CANCELACIONES_FILE "cancelaciones.txt"
#define ESTA_ACTIVA(fin, sistema) (fin > sistema) // Verifica si la reserva está activa
#define MAX_NOCHES_RESERVA 365
#define LONG_ANOTACIONES 1000
#define DEFAULT_NUMERO_RESERVAS 10
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
#include "reserva.hpp"
#include "linked_list.hpp"
#include <limits>

struct callback_param_historico {
    std::ofstream* archivo;
    Fecha* fecha;
    Anfitrion* anfitrion;
    Linked_List<Reserva*>* historico;
};

struct callback_param_reservacion {
    Fecha *inicio;
    Fecha *fin;
    std::string *mpo;
    Linked_List<Alojamiento*>* alojamientos;
    Linked_List<Alojamiento*>* alojamientos_disponibles;
};

void app_main(void);
#endif