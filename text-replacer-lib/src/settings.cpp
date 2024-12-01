#include <iostream>
#include <fstream>
#include <format>
#include <memory>

#include "settings.hpp"

namespace text_replacer_lib {
    template <typename T> static bool load_json_setting(ns::json& data, std::string name, T* attr) {
        if(data.contains(name)) {
            (*attr) = data[name].get<T>();
            return true;
        } else {
            std::cerr << "ERROR: attribute '" << name << "' is missing from settings.";
            std::cerr << "Default value used instead.\n";
            return false;
        }
    }

    void load_settings(Settings* p_settings, std::filesystem::path file_path) {
        std::ifstream in_file(file_path);
        
        if (!in_file.is_open()){
            std::cout << "'" + file_path.filename().string() + "' not found. Creating...\n";
            save_settings(p_settings, file_path);

            return;
        }
        
        ns::json settings_json;
        try {
            in_file >> settings_json;
            in_file.close();
        }
        catch (std::exception e) {
            in_file.close();
            std::cerr << "'" + file_path.filename().string() + "' is invalid. Using defaults...\n";
            return;
        } 
        

        if (settings_json.empty()) {
            // PLOG_WARNING.printf("Settings file at '%s' was empty.", file_path.string().c_str());
            return;
        }

        load_json_setting(settings_json, "dumping_enabled", &p_settings->dumping_enabled);
        load_json_setting(settings_json, "prefer_hex_values", &p_settings->prefer_hex_values);
        load_json_setting(settings_json, "text_dump_output", &p_settings->text_dump_output);
        load_json_setting(settings_json, "text_replacement_dir", &p_settings->text_replacement_dir);

        // Loading settings category structs:
        
    }

    void save_settings(Settings* p_settings, fs::path file_path) {
        ns::json settings_json;

        // Generating Json for settings category structs:
        settings_json["dumping_enabled"] = p_settings->dumping_enabled;
        settings_json["prefer_hex_values"] = p_settings->prefer_hex_values;
        settings_json["text_dump_output"] = p_settings->text_dump_output;
        settings_json["text_replacement_dir"] = p_settings->text_replacement_dir;

        std::ofstream out_file(file_path);
        out_file << settings_json.dump(4);
        out_file.close();
    }
}