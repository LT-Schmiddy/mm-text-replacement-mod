#include <bit>
#include "TextEntry.hpp"

namespace text_replacer_lib {
TextEntry::TextEntry(uint16_t p_message_id, char* p_message_buffer, int p_len) {
    message_id = p_message_id;
    
    // Loading Header:    
    memcpy(&text_box_type, &p_message_buffer[0], sizeof(uint8_t));
    memcpy(&text_box_y_pos, &p_message_buffer[1], sizeof(uint8_t));
    memcpy(&display_icon, &p_message_buffer[2], sizeof(uint8_t));
    memcpy(&next_message_id, &p_message_buffer[3], sizeof(uint16_t));
    memcpy(&first_item_rupees, &p_message_buffer[5], sizeof(uint16_t));
    memcpy(&second_item_rupees, &p_message_buffer[7], sizeof(uint16_t));

    // Loading Content:
    content = "";
    for (int i = MESSAGE_HEADER_SIZE; i < p_len; i++) {
        char c = p_message_buffer[i];
        // if(c != '\0') {
        if(utils::is_ascii(c)) {
            char c_str[2] = {c, '\0'};
            content.append(c_str);
        
        } else {
            content.append("|" + utils::to_hex(&c, 1));
        }
    }
}

TextEntry::TextEntry(uint16_t p_message_id, ns::json data) {
    message_id = p_message_id;
    
    load_json_attr(data, "text_box_type", &text_box_type);
    load_json_attr(data, "text_box_y_pos", &text_box_y_pos);
    load_json_attr(data, "display_icon", &display_icon);
    load_json_attr(data, "next_message_id", &next_message_id);
    load_json_attr(data, "first_item_rupees", &first_item_rupees);
    load_json_attr(data, "second_item_rupees", &second_item_rupees);
    load_json_attr(data, "content", &content);
}

TextEntry::~TextEntry() {}

ns::json TextEntry::to_json() {
    ns::json retVal;

    retVal["text_box_type"] = text_box_type;
    retVal["text_box_y_pos"] = text_box_y_pos;
    retVal["display_icon"] = display_icon;
    retVal["next_message_id"] = next_message_id;
    retVal["first_item_rupees"] = first_item_rupees;
    retVal["first_item_rupees"] = first_item_rupees;
    retVal["second_item_rupees"] = second_item_rupees;
    retVal["content"] = content;

    return retVal;
}

int TextEntry::prepare_buffer(char* p_message_buffer) {
    // Handling Header:
    memcpy(&p_message_buffer[0], &text_box_type, sizeof(uint8_t));
    memcpy(&p_message_buffer[1], &text_box_y_pos, sizeof(uint8_t));
    memcpy(&p_message_buffer[2], &display_icon, sizeof(uint8_t));
    memcpy(&p_message_buffer[3], &next_message_id, sizeof(uint16_t));
    memcpy(&p_message_buffer[5], &first_item_rupees, sizeof(uint16_t));
    memcpy(&p_message_buffer[7], &second_item_rupees, sizeof(uint16_t));

    // Handling Content:
    int len = MESSAGE_HEADER_SIZE;
    int pos = 0;
    while (pos < content.size()) {
        char c = content.at(pos);

        if (c == '|') {
            // Checking next character:
            if (content.at(pos + 1) == '|') {
                // Use a double pipe "||" to enter a "|"
                p_message_buffer[len] = '|';
                pos += 2;
            } else {
                // This is a byte representation
                std::string byte_str = {content.at(pos + 1), content.at(pos + 2)};
                char replace_char;
                utils::from_hex(byte_str, &replace_char, sizeof(char));

                p_message_buffer[len] = replace_char;
                pos += 3;
            }

        } else {
            p_message_buffer[len] = c;
            pos++;
        }

        len++;
    }
    return len;
}

bool TextEntry::check_json_attr(ns::json* data, std::string name){
    if(data->contains(name)) {
        return true;
    } else {
        std::cerr << "ERROR: attribute '" << name << "' is missing from text entry '" << utils::to_hex(&message_id, sizeof(uint16_t)) << "'.\n";
        std::cerr << "Default value used instead.\n";
        return false;
    }
}

template <typename T> bool TextEntry::load_json_attr(ns::json& data, std::string name, T* attr) {
    if(data.contains(name)) {
        (*attr) = data[name].get<T>();
        return true;
    } else {
        std::cerr << "ERROR: attribute '" << name << "' is missing from text entry '" << utils::to_hex(&message_id, sizeof(uint16_t)) << "'.\n";
        std::cerr << "Default value used instead.\n";
        return false;
    }
}
}
