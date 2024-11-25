// #include <filesystem>
// #include <fstream>

// #include <stdio.h>
// #include <stdlib.h>
// #include <assert.h>

#include "./main.hpp"
#include "./recomp_utils.hpp"

RECOMP_EXPORT extern "C" uint32_t recomp_api_version = RECOMP_API_VERSION;

RECOMP_DLL_FUNC(text_replacer_on_load) {
    lib_startup();
    _return(ctx, (s32) 1);
}

RECOMP_DLL_FUNC(text_replacer_on_shutdown) {
    lib_shutdown();
    _return(ctx, (s32) 1);
}
