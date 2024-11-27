#pragma once

#include <filesystem>
#include <fstream>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <mod_recomp.h>

#if defined(_MSC_VER)
    //  Microsoft
    #define DLLEXPORT __declspec(dllexport)
    #define DLLIMPORT __declspec(dllimport)
#elif defined(__GNUC__)
    //  GCC
    #define DLLEXPORT __attribute__((visibility("default")))
    #define DLLIMPORT
#else
    #define DLLEXPORT
    #define DLLIMPORT
#endif

extern "C" {
    #define RECOMP_API_VERSION 1
    #define TO_PTR(type, var) ((type*)(&rdram[(uint64_t)var - 0xFFFFFFFF80000000]))

    #define RECOMP_DLL_C_FUNC(_f_name) DLLEXPORT void _f_name(uint8_t* rdram, recomp_context* ctx)

    // Type Defs:
    typedef uint8_t u8;
    typedef uint16_t u16;
    typedef uint32_t u32;
    typedef uint64_t u64;

    typedef int8_t s8;
    typedef int16_t s16;
    typedef int32_t s32;
    typedef int64_t s64;
}

#define RECOMP_DLL_FUNC(_f_name) extern "C" RECOMP_DLL_C_FUNC(_f_name)
#define RECOMP_ARG(_pos, _type) _arg<_pos, _type>(rdram, ctx)
#define RECOMP_RETURN(_type, _value) _return(ctx, (_type) _value)

template<int index, typename T>
inline T _arg(uint8_t* rdram, recomp_context* ctx) {
    static_assert(index < 4, "Only args 0 through 3 supported");
    gpr raw_arg = (&ctx->r4)[index];
    if constexpr (std::is_same_v<T, float>) {
        if constexpr (index < 2) {
            static_assert(index != 1, "Floats in arg 1 not supported");
            return ctx->f12.fl;
        }
        else {
            // static_assert in else workaround
            [] <bool flag = false>() {
                static_assert(flag, "Floats in a2/a3 not supported");
            }();
        }
    }
    else if constexpr (std::is_pointer_v<T>) {
        static_assert (!std::is_pointer_v<std::remove_pointer_t<T>>, "Double pointers not supported");
        return TO_PTR(std::remove_pointer_t<T>, raw_arg);
    }
    else if constexpr (std::is_integral_v<T>) {
        static_assert(sizeof(T) <= 4, "64-bit args not supported");
        return static_cast<T>(raw_arg);
    }
    else {
        // static_assert in else workaround
        [] <bool flag = false>() {
            static_assert(flag, "Unsupported type");
        }();
    }
}

template <typename T>
inline void _return(recomp_context* ctx, T val) {
    static_assert(sizeof(T) <= 4, "Only 32-bit value returns supported currently");
    if constexpr (std::is_same_v<T, float>) {
        ctx->f0.fl = val;
    }
    else if constexpr (std::is_integral_v<T> && sizeof(T) <= 4) {
        ctx->r2 = int32_t(val);
    }
    else {
        // static_assert in else workaround
        [] <bool flag = false>() {
            static_assert(flag, "Unsupported type");
        }();
    }
}