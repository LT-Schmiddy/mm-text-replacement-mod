#pragma once

#include <iostream>
#include <string>

#include "utils.hpp"

namespace text_replacer_lib {
class TextEntry {
public:
    TextEntry(uint16_t p_message_id, char* p_message_buffer, int p_len);
    TextEntry(uint16_t p_message_id, ns::json data);
    ~TextEntry();

    ns::json to_json();
    int prepare_buffer(char* p_message_buffer);

private:
    uint16_t message_id = 0;
    uint8_t text_box_type = 0;
    uint8_t text_box_y_pos = 0;
    uint8_t display_icon = 0;
    uint16_t next_message_id = 0xffff;
    uint16_t first_item_rupees = 0xffff;
    uint16_t second_item_rupees = 0xffff;
    
    std::string content = "";

    
    bool check_json_attr(ns::json* data, std::string name);

    template <typename T> bool load_json_attr(ns::json& data, std::string name, T* attr, bool allow_hex_translation = true);

};
}