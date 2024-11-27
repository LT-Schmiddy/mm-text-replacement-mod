#pragma once

#include <iostream>
#include <map>
#include "utils.hpp"
#include "TextEntry.hpp"
namespace global {
    // Dumping
    inline bool dumping_enabled = false;
    inline fs::path dump_path;
    inline ns::json text_table;

    // Main stuff
    inline fs::path game_data_dir;
    inline fs::path dialog_replacement_dir;
    inline std::map<uint16_t, text_replacer_lib::TextEntry> text_map;
    inline char message_buffer[MESSAGE_BUFFER_SIZE];

}