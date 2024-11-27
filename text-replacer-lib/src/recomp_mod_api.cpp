// #include <filesystem>
// #include <fstream>

// #include <stdio.h>
// #include <stdlib.h>
// #include <assert.h>

#include "./lib_main.hpp"
#include "./recomp_mod_api.hpp"

RECOMP_EXPORT extern "C" uint32_t recomp_api_version = RECOMP_API_VERSION;

RECOMP_DLL_FUNC(text_replacer_on_load) {
    lib_startup();
    _return(ctx, (s32) 1);
}

RECOMP_DLL_FUNC(text_replacer_set_char_in_buffer) {
    set_char_in_buffer(_arg<0, char>(rdram, ctx), _arg<1, uint32_t>(rdram, ctx));
    _return(ctx, (s32) 1);
}

RECOMP_DLL_FUNC(text_replacer_add_buffer_as_entry) {
    add_buffer_as_entry(_arg<0, uint32_t>(rdram, ctx), _arg<1, uint32_t>(rdram, ctx));
    _return(ctx, (s32) 1);
}


RECOMP_DLL_FUNC(text_replacer_on_shutdown) {
    lib_shutdown();
    _return(ctx, (s32) 1);
}
