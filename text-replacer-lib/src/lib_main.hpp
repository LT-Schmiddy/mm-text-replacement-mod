#pragma once

#include "utils.hpp"

namespace text_replacer_lib {
    void startup();
    // Dumping:
    void set_char_in_buffer(char c, uint32_t pos);
    void add_buffer_as_entry(uint32_t id, uint32_t len);
    void save_entries_json();
    
    // Replacing:
    bool has_replacement(uint16_t p_message_id);
    int prepare_replacement(uint16_t p_message_id);
    char get_replacement_char(uint32_t pos);

    void shutdown();
}
