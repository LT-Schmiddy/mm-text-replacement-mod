#include <algorithm>
#include <iostream>
#include <windows.h>
#include "utils.hpp"


static char const _hex[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

namespace text_replacer_lib::utils {
bool is_ascii(char c) {
    // if (static_cast<unsigned char>(c) < 31 || static_cast<unsigned char>(c) > 127) {
    if (static_cast<unsigned char>(c) < 31 || static_cast<unsigned char>(c) > 127) {
        return false;
    }
    return true;
}


std::string to_hex(void* bytes, int size) {
    std::string str;
    char* cbytes = (char*)bytes;
    // for (int i = 0; i < size; ++i) {
    for (int i = size - 1; i >= 0; i--) {
    char ch = cbytes[i];
        str.append(&_hex[(ch & 0xF0) >> 4], 1);
        str.append(&_hex[ch & 0xF], 1);
    }
    return str;
}

 
void from_hex(std::string p_str, void* p_bytes, int p_size) {
    std::string str;
    // Handling size check:
    if ((float)(p_str.size()/2.0f) > (float)p_size) {
        str = p_str.substr(p_str.size() - p_size*2, p_size*2);
        std::cerr << "Warning: hex string '" << p_str << "' is too long for memory space of " 
            << std::to_string(p_size) << " byte(s). Trimming to '" << str << "'.";
    } else {
        str = p_str;    
    }
    std::transform(str.cbegin(), str.cend(), str.begin(), [](unsigned char c){
        return std::toupper(c);
    });
    
    char* cbytes = (char*)p_bytes;
    for (int i = 0; i < str.size(); i++) {
        char c = str.at(i);
        // Determining value of the symbol:
        uint8_t val = 0;
        for (uint8_t j = 0; j < 16; j++) {
            if (_hex[j] == c){
                val = j;
            }
        }

        // Loading the value into position:
        int target_pos = (int)((str.size() - 1 - i)/2);
        uint8_t* target = (uint8_t*)(&cbytes[target_pos]);
        if (i % 2 == 0) {
            // Upper:
            uint8_t shift = val << 4;
            (*target) = shift;
        } else if (i % 2 == 1) {
            // Lower
            (*target) |= val;
        }
    }
}

fs::path get_lib_path() {
    char path[MAX_PATH];
    HMODULE hm = NULL;

    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | 
            GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            (LPCSTR) &is_ascii, &hm) == 0)
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
}