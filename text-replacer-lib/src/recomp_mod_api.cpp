// #include <filesystem>
// #include <fstream>

// #include <stdio.h>
// #include <stdlib.h>
// #include <assert.h>

#include "./globals.hpp"
#include "./lib_main.hpp"
#include "./dumping.hpp"
#include "./recomp_mod_api.hpp"

RECOMP_EXPORT extern "C" uint32_t recomp_api_version = RECOMP_API_VERSION;

RECOMP_DLL_FUNC(text_replacer_lib_startup) {
    text_replacer_lib::startup();
    RECOMP_RETURN(s32, 1);
}

// Dumping:
RECOMP_DLL_FUNC(text_replacer_lib_dumping_enabled) {
    RECOMP_RETURN(s32, global::settings.dumping_enabled);
}

RECOMP_DLL_FUNC(text_replacer_lib_dumping_set_char_in_buffer) {
    text_replacer_lib::dumping::set_char_in_buffer(RECOMP_ARG(0, char), RECOMP_ARG(1, uint32_t));
    RECOMP_RETURN(s32, 1);
}

RECOMP_DLL_FUNC(text_replacer_lib_dumping_add_buffer_as_entry) {
    text_replacer_lib::dumping::add_buffer_as_entry(_arg<0, uint32_t>(rdram, ctx), _arg<1, uint32_t>(rdram, ctx));
    RECOMP_RETURN(s32, 1);
}

// Replacing:
RECOMP_DLL_FUNC(text_replacer_lib_has_replacement) {
    RECOMP_RETURN(s32, text_replacer_lib::has_replacement(RECOMP_ARG(0, uint16_t)));
}

RECOMP_DLL_FUNC(text_replacer_lib_prepare_replacement) {
    RECOMP_RETURN(s32, text_replacer_lib::prepare_replacement(RECOMP_ARG(0, uint16_t)));
}

RECOMP_DLL_FUNC(text_replacer_lib_get_replacement_char) {
    RECOMP_RETURN(char, text_replacer_lib::get_replacement_char(RECOMP_ARG(0, u32)));
}

// Shutdown
RECOMP_DLL_FUNC(text_replacer_lib_shutdown) {
    text_replacer_lib::shutdown();
    RECOMP_RETURN(s32, 1);
}
