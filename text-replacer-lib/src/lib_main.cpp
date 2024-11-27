#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <exception>
#include <iomanip>

#include "recomp_mod_api.hpp"
#include "utils.hpp"
#include "TextEntry.hpp"
#include "lib_main.hpp"



namespace global {
    bool dumping_enabled = false;

    fs::path dump_path;
    ns::json text_table;

    char message_buffer[MESSAGE_BUFFER_SIZE];
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
    TextEntry entry(id, global::message_buffer, len);

    global::text_table[to_hex((char*)&id, 2)] = entry.to_json();

    save_entries_json();
}

void save_entries_json() {
    std::ofstream o(global::dump_path);
    try {
        o << std::setw(4) << global::text_table.dump(4) << std::endl;
        // std::cout << global::text_table.dump(4);
    } catch (std::exception e){
        std::cerr << e.what();
    }
    o.close();
}

void lib_shutdown() {}


// Utilities: