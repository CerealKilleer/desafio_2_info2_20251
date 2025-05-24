#ifndef __PERFORMANCE_HPP__
#define __PERFORMANCE_HPP__

#include <cstddef>
extern uint32_t g_ciclos;              ///< Contador de ciclos
extern uint32_t g_tamano;              ///< Tamaño total de objetos en memoria
extern uint32_t g_strlen_cnt;          ///< Contador de strlen
extern uint32_t g_memcpy_cnt;          ///< Contador de memcpy
extern uint32_t g_memcmp_cnt;          ///< Contador de memcmp
extern uint32_t g_is_digit_cnt;        ///< Contador de isdigit
extern uint32_t g_getline_cnt;         ///< Contador de getline
extern uint32_t g_string_find_cnt;     ///< Contador de string::find
extern uint32_t g_string_substr_cnt;   ///< Contador de string::substr
extern uint32_t g_std_n_pos_cnt;       ///< Contador de std::npos
extern uint32_t g_c_string_cnt;          ///< Contador de c_str
extern uint32_t g_stoi_cnt;           ///< Contador de stoi
extern uint32_t g_stof_cnt;           ///< Contador de stof
extern uint32_t g_stoull_cnt;        ///< Contador de stoull
extern uint32_t g_strcmp_cnt;        ///< Contador de strcmp
extern uint32_t g_sprintf_cnt;       ///< Contador de sprintf
extern uint32_t g_string_legnth_cnt; ///< Contador de string::length
#endif