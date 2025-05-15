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
#include "usuario.hpp"
#include "unordered_map.hpp"

#define MAX_PASSWORD_LENGTH 20
#define MEM_LOG(fn, msg) std::cout << "[App/" << fn << "]: " << msg << std::endl

static Unordered_Map<uint64_t, Usuario>* cargar_usuarios(const std::string &archivo_nombre);

/**
 * @brief Carga los usuarios desde un archivo de texto y los imprime.
 * 
 * @param archivo_nombre Nombre del archivo a cargar.
*/

 static Unordered_Map<uint64_t, Usuario>* cargar_usuarios(const std::string &archivo_nombre) 
 {
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

     Unordered_Map<uint64_t, Usuario>* usuarios = new Unordered_Map<uint64_t, Usuario>(size);

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
 

void app_main() 
{
    Unordered_Map<uint64_t, Usuario>* usuarios = cargar_usuarios("usuarios.txt");
    Usuario * usuario = usuarios->find(6784498765);
    if (usuario != nullptr) {
        std::cout << "Usuario encontrado: " << usuario->get_documento() << std::endl;
        std::cout << "Contraseña: " << usuario->get_password() << std::endl;
        std::cout << "Antigüedad: " << usuario->get_antiguedad() << " meses" << std::endl;
        std::cout << "Puntuación: " << usuario->get_puntuacion() << std::endl;
        std::cout << "Anfitrión: " << (usuario->get_anfitrion() ? "Sí" : "No") << std::endl;
    } else {
        std::cout << "Usuario no encontrado." << std::endl;
    }

    delete usuarios;
}




