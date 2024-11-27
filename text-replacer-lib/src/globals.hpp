#pragma once

#include <iostream>
#include <map>
#include "utils.hpp"
#include "TextEntry.hpp"
#include "settings.hpp"
namespace global {
    inline fs::path settings_path;
    inline text_replacer_lib::Settings settings;
    
    // Dumping:
    inline fs::path dump_path;
    inline ns::json dump_table;

    // Main stuff
    inline fs::path game_data_dir;
    inline fs::path dialog_replacement_dir;
    inline std::map<uint16_t, text_replacer_lib::TextEntry> text_map;
    inline char message_buffer[MESSAGE_BUFFER_SIZE];

}