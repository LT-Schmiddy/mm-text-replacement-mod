#pragma once
#include <string>
#include <vector>

#include "utils.hpp"

namespace text_replacer_lib {
    struct Settings {
        bool dumping_enabled = false;
    };

    void load_settings(Settings* settings, fs::path file);
    void save_settings(Settings* settings, fs::path file);
}