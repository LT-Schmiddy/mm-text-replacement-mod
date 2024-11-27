#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <iomanip>


#include "globals.hpp"

#include "recomp_mod_api.hpp"
#include "utils.hpp"
#include "TextEntry.hpp"
#include "lib_main.hpp"

namespace text_replacer_lib {
void startup() {
    std::cout << "Hello from text-replacer-lib!\n";
    global::dump_path = get_lib_path().parent_path().append("text_dump.json");
    std::cout << global::dump_path.string().c_str() << "\n";

    global::text_table = ns::json();
}




void shutdown() {}
}

// Utilities: