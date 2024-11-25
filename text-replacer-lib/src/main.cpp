#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>

#include <windows.h>
#include <nlohmann/json.hpp>

#include "recomp_utils.hpp"
#include "main.hpp"

namespace ns = nlohmann;
namespace fs = std::filesystem;

namespace globals {
    fs::path dump_path;

}

void lib_startup() {
    
    std::cout << "Hello from text-replacer-lib!";
}

void lib_shutdown() {

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