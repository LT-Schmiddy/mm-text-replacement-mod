#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <iomanip>

#include <windows.h>
#include <nlohmann/json.hpp>

#include "recomp_utils.hpp"
#include "main.hpp"

namespace ns = nlohmann;

namespace global {
    fs::path dump_path;
    ns::json text_table;

    char message_buffer[1280];
}

void lib_startup() {
    std::cout << "Hello from text-replacer-lib!\n";
    global::dump_path = get_lib_path().parent_path().append("text_dump.json");
    std::cout << global::dump_path.string().c_str() << "\n";

    global::text_table = ns::json();
}

void set_char_in_buffer(char c, uint32_t pos) {
    global::message_buffer[pos] = c;
}

void add_buffer_as_entry(uint32_t id, uint32_t len) {
    std::string message = "";
    
    for (int i = 0; i < len; i++) {
        char c = global::message_buffer[i];
        // if(c != '\0') {
        if(is_ascii(c)) {
            char c_str[2] = {c, '\0'};
            message.append(c_str);
        
        } else {
            message.append("[" + std::to_string((uint8_t)c) + "]");
        }
    }

    global::text_table[to_hex((uint16_t)id)] = message;

    save_entries_json();
}

void save_entries_json() {
    std::ofstream o(global::dump_path);
    o << std::setw(4) << global::text_table.dump(4) << std::endl;
    o.close();
}

void lib_shutdown() {}


// Utilities:
bool is_ascii(char c) {
    if (static_cast<unsigned char>(c) > 127) {
        return false;
    }
    return true;
}

template<typename T>
std::string to_hex(T i){
    std::stringstream stream;
    stream << "\\x" 
            << std::setfill ('0') << std::setw(sizeof(T)*2) 
            << std::hex << i;
    return stream.str();
}

fs::path get_lib_path() {
    char path[MAX_PATH];
    HMODULE hm = NULL;

    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR) &lib_startup, &hm) == 0)
    {
        int ret = GetLastError();
        fprintf(stderr, "GetModuleHandle failed, error = %d\n", ret);
        throw std::exception("Failed to find path to library for text-replacer-lib: GetModuleHandle");
        // Return or however you want to handle an error.
    }
    if (GetModuleFileName(hm, path, sizeof(path)) == 0)
    {
        int ret = GetLastError();
        fprintf(stderr, "GetModuleFileName failed, error = %d\n", ret);
        throw std::exception("Failed to find path to library for text-replacer-lib: GetModuleFileName");
    }

    return fs::path(path);
}