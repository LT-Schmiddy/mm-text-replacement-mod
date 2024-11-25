#ifndef MESSAGE_PATCH
#define MESSAGE_PATCH

#include "global.h"
#include "z64horse.h"
#include "z64shrink_window.h"
#include "z64view.h"
#include "message_data_static.h"
// #include "interface/parameter_static/parameter_static.h"
#include "overlays/kaleido_scope/ovl_kaleido_scope/z_kaleido_scope.h"

u8 D_801C6A70 = 0;
s16 sOcarinaButtonIndexBufPos = 0;
s16 sOcarinaButtonIndexBufLen = 0;
s16 sLastPlayedSong = 0xFF;
s16 sOcarinaButtonStepR = 0;
s16 sOcarinaButtonStepG = 0;
s16 sOcarinaButtonStepB = 0;
s16 sOcarinaButtonFlashTimer = 12;
s16 sOcarinaButtonFlashColorIndex = 1;
s16 D_801C6A94 = 0;

s32 sCharTexSize;
s32 sCharTexScale;
s32 D_801F6B08;

void Message_FindMessage(PlayState* play, u16 textId);

#endif