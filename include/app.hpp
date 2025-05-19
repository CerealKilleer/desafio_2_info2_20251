#ifndef __APP_H__
#define __APP_H__

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
#define HISTORICO_FILE "historico.txt"
#define CANCELACIONES_FILE "cancelaciones.txt"
#define ESTA_ACTIVA(fin, sistema) (fin > sistema) // Verifica si la reserva está activa
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

struct callback_param_historico {
    std::ofstream* archivo;
    Fecha* fecha;
    Anfitrion* anfitrion;
    Linked_List<Reserva*>* historico;
};

void app_main(void);
#endif