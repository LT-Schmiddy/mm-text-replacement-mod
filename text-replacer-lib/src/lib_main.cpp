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
    std::cout << "Loaded text-replacer-lib!\n";
    // Gets us out of the mods folder:
    global::game_data_dir = utils::get_lib_path().parent_path().parent_path();

    // Loading settings:
    global::settings_path = fs::path(global::game_data_dir).append(SETTINGS_FILE_NAME);
    load_settings(&global::settings, global::settings_path);

    // Setting dump path for text replacement.
    if (global::settings.text_dump_output.is_absolute()) {
        global::dump_path = global::settings.text_dump_output;
    } else {
        global::dump_path = global::game_data_dir / global::settings.text_dump_output;
    }
    global::dump_table = ns::json();

    // Setting loading directory for text replacement.
    if (global::settings.text_replacement_dir.is_absolute()) {
        global::text_replacement_dir = global::settings.text_replacement_dir;
    } else {
        global::text_replacement_dir = global::game_data_dir / global::settings.text_replacement_dir;
    }

    if (!fs::exists(global::text_replacement_dir)) {
        try {
            fs::create_directory(global::text_replacement_dir);
            std::cout << "Creating '" << global::text_replacement_dir.string().c_str() << "'\n";
        } catch(std::exception e) {
            std::cerr << "ERROR: Could not create '" << global::text_replacement_dir.string().c_str() << "'\n";
        }
    }

    // Processing text replacement files:
    for (const auto & entry : fs::directory_iterator(global::text_replacement_dir)) {
        if (entry.path().extension() != ".json") {
            continue;
        }

        // Skipping if the file cannot be loaded or json is invalid.
        std::ifstream in_file(entry.path());
        if (!in_file.is_open()){
            std::cerr << "ERROR: Could not open " << fs::relative(entry.path()) << std::endl;
            continue;
        } else {
            std::cout << "Found " << entry.path().filename() << std::endl;
        }

        ns::json replacement_json;
        try {
            in_file >> replacement_json;
            in_file.close();
        }
        catch (std::exception e) {
            in_file.close();
            std::cerr << "'" + entry.path().filename().string() + "' is invalid and will not be loaded.\n";
            continue;
        } 

        for (auto it = replacement_json.begin(); it != replacement_json.end(); ++it) {
            // std::cout << *it << '\n';

            uint16_t message_id = 0;
            utils::from_hex(it.key(), &message_id, sizeof(uint16_t));

            std::cout << "\tLoading Message ID: 0x" << it.key() 
                << " -> " << std::to_string((unsigned int)message_id) 
                // << " -> 0x" << utils::to_hex(&message_id, sizeof(uint16_t)) 
                << std::endl;

            global::text_map.emplace(message_id, TextEntry(message_id, it.value()));
        }
    }
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