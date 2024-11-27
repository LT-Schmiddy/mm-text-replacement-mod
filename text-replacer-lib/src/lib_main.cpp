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
#include "settings.hpp"
#include "lib_main.hpp"

namespace text_replacer_lib {
void startup() {
    std::cout << "Hello from text-replacer-lib!\n";
    // Gets us out of the mods folder:
    global::game_data_dir = utils::get_lib_path().parent_path().parent_path();

    // Loading settings:
    global::settings_path = fs::path(global::game_data_dir).append("text_replacer_settings.json");
    load_settings(&global::settings, global::settings_path);

    // Setting dump path for text replacement.
    global::dump_path = fs::path(global::game_data_dir).append("text_dump.json");

    // Processing text replacement files:
    global::dialog_replacement_dir = fs::path(global::game_data_dir).append("text_replacement");;
    if (!fs::exists(global::dialog_replacement_dir)) {
        std::cout << global::dialog_replacement_dir.string().c_str() << "\n";
        fs::create_directory(global::dialog_replacement_dir);
    }

    for (const auto & entry : fs::directory_iterator(global::dialog_replacement_dir)) {
        if (entry.path().extension() != ".json") {
            continue;
        }

        std::ifstream in_file(entry.path());
        if (!in_file.is_open()){
            std::cerr << "ERROR: Could not open " << fs::relative(entry.path()) << std::endl;
            return;
        } else {
            std::cout << "Found " << entry.path().filename() << std::endl;
        }

        ns::json root_replacement_json;
        in_file >> root_replacement_json;
        in_file.close();

        for (auto it = root_replacement_json.begin(); it != root_replacement_json.end(); ++it) {
            // std::cout << *it << '\n';

            uint16_t message_id = 0;
            utils::from_hex(it.key(), &message_id, sizeof(uint16_t));

            std::cout << "Loading Message ID: 0x" << it.key() 
                << " -> " << std::to_string((unsigned int)message_id) 
                // << " -> 0x" << utils::to_hex(&message_id, sizeof(uint16_t)) 
                << std::endl;

            global::text_map.emplace(message_id, TextEntry(message_id, it.value()));
        }
    }

    // For dumping:
    global::dump_table = ns::json();
}

bool has_replacement(uint16_t p_message_id) {
    return global::text_map.contains(p_message_id);
}

int prepare_replacement(uint16_t p_message_id) {
    return global::text_map.at(p_message_id).prepare_buffer(global::message_buffer);
}

char get_replacement_char(uint32_t pos) {
    return global::message_buffer[pos];
}

void shutdown() {}
}

// Utilities: