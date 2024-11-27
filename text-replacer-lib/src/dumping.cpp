#include <fstream>
#include <iostream>

#include "dumping.hpp"
#include "globals.hpp"

namespace text_replacer_lib::dumping {

// Dumping:
void set_char_in_buffer(char c, uint32_t pos) {
    global::message_buffer[pos] = c;
}

void add_buffer_as_entry(uint32_t id, uint32_t len) {
    TextEntry entry(id, global::message_buffer, len);

    global::dump_table[utils::to_hex((char*)&id, 2)] = entry.to_json();

    save_entries_json();
}

void save_entries_json() {
    std::ofstream o(global::dump_path);
    try {
        o << std::setw(4) << global::dump_table.dump(4) << std::endl;
        // std::cout << global::dump_table.dump(4);
    } catch (std::exception e){
        std::cerr << e.what();
    }
    o.close();
}
}