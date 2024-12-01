#pragma once
#include <string>
#include <vector>

#include "utils.hpp"

#define SETTINGS_FILE_NAME "text_replacer_settings.json"

namespace text_replacer_lib {
    struct Settings {
        bool dumping_enabled = false;
        bool prefer_hex_values = true;
        fs::path text_dump_output = "text_dump.json";
        fs::path text_replacement_dir = "text_replacement";
    };

    void load_settings(Settings* settings, fs::path file);
    void save_settings(Settings* settings, fs::path file);
}