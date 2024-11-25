#include <filesystem>
namespace fs = std::filesystem;

void lib_startup();
void set_char_in_buffer(char c, uint32_t pos);
void add_buffer_as_entry(uint32_t id, uint32_t len);
void save_entries_json();

void lib_shutdown();

bool is_ascii(char c);

template<typename T>
std::string to_hex(T i);
fs::path get_lib_path();


