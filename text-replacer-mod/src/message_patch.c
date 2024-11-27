#include "tweak_globals.h"
#include "message_patch.h"
void Message_OpenText(PlayState* play, u16 textId);

RECOMP_IMPORT(".", void text_replacer_lib_startup());
RECOMP_IMPORT(".", void text_replacer_lib_set_char_in_buffer(char c, u32 pos));
RECOMP_IMPORT(".", void text_replacer_lib_add_buffer_as_entry(u32 id, u32 len));
RECOMP_IMPORT(".", u32 text_replacer_lib_has_replacement(u16 id));
RECOMP_IMPORT(".", u32 text_replacer_lib_prepare_replacement(u16 id));
RECOMP_IMPORT(".", char text_replacer_lib_get_replacement_char(u32 pos));

RECOMP_CALLBACK("*", recomp_on_init) void load_lib () {
    text_replacer_lib_startup();
}

void Message_FindMessage(PlayState* play, u16 textId);
void Message_GrowTextbox(PlayState* play);
void Message_ResetOcarinaButtonState(PlayState* play);
void func_80150A84(PlayState* play);

void handle_text_replacement(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;

    for (int i = 0; i < msgCtx->msgLength; i++) {
        text_replacer_lib_set_char_in_buffer(font->msgBuf.schar[i], i);
        // recomp_printf("%c", font->msgBuf.schar[i]);
    }
    text_replacer_lib_add_buffer_as_entry(textId, msgCtx->msgLength);


    if (text_replacer_lib_has_replacement(textId)) {
        recomp_printf("Replacing text %d.\n", textId);
        s32 len = text_replacer_lib_prepare_replacement(textId);
        
        for (s32 i = 0; i < len; i++) {
            font->msgBuf.schar[i] = text_replacer_lib_get_replacement_char(i);
        }
        msgCtx->msgLength = len;
    }

    // Copied from Message_OpenText
    msgCtx->choiceNum = 0;
    msgCtx->textUnskippable = false;
    msgCtx->textboxEndType = TEXTBOX_ENDTYPE_00;
    msgCtx->textDrawPos = 0;
    msgCtx->msgBufPos = 0;
    msgCtx->decodedTextLen = 0;

    msgCtx->unk11F08 = font->msgBuf.schar[msgCtx->msgBufPos] << 8;
    msgCtx->unk11F08 |= font->msgBuf.schar[msgCtx->msgBufPos + 1];

    msgCtx->unk11F18 = (msgCtx->unk11F08 & 0xF000) >> 0xC;
    msgCtx->textBoxType = (msgCtx->unk11F08 & 0xF00) >> 8;
    msgCtx->textBoxPos = (msgCtx->unk11F08 & 0xF0) >> 4;
    msgCtx->unk11F0C = msgCtx->unk11F08 & 0xF;

    if ((msgCtx->unk11F0C == 1) || (msgCtx->unk11F0C == 3)) {
        msgCtx->textUnskippable = true;
    }
    msgCtx->itemId = 0xFE;

    if ((msgCtx->textBoxType == TEXTBOX_TYPE_5) || (msgCtx->textBoxType == TEXTBOX_TYPE_D) ||
        (play->pauseCtx.bombersNotebookOpen)) {
        msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2 = 0;
    } else {
        msgCtx->unk120CE = msgCtx->unk120D0 = msgCtx->unk120D2 = 0xFF;
    }

    msgCtx->choiceIndex = 0;

    if (msgCtx->unk11F0C != 3) {
        msgCtx->textColorAlpha = 0xFF;
    } else {
        msgCtx->textColorAlpha = 0;
    }
}

RECOMP_PATCH void Message_StartTextbox(PlayState* play, u16 textId, Actor* actor) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;

    msgCtx->ocarinaAction = 0xFFFF;
    Message_OpenText(play, textId);
    handle_text_replacement(play, textId);

    msgCtx->talkActor = actor;
    msgCtx->msgMode = MSGMODE_TEXT_START;
    msgCtx->stateTimer = 0;
    msgCtx->textDelayTimer = 0;
    play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;
}

RECOMP_PATCH void Message_ContinueTextbox(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;
    InterfaceContext* interfaceCtx = &play->interfaceCtx;
    Font* font = &msgCtx->font;

    msgCtx->msgLength = 0;
    Message_OpenText(play, textId);
    handle_text_replacement(play, textId);

    func_80150A84(play);

    msgCtx->msgMode = MSGMODE_TEXT_CONTINUING;
    msgCtx->stateTimer = 8;
    msgCtx->textDelayTimer = 0;

    if (interfaceCtx->unk_222 == 0) {
        if (textId != 0x1B93) {
            func_8011552C(play, DO_ACTION_NEXT);
        } else if (textId != 0xF8) {
            func_8011552C(play, DO_ACTION_DECIDE);
        }
    }
    msgCtx->textboxColorAlphaCurrent = msgCtx->textboxColorAlphaTarget;

    if (play->pauseCtx.bombersNotebookOpen) {
        msgCtx->textboxXTarget = 34;
        msgCtx->textboxYTarget = 350;
        Message_GrowTextbox(play);
        msgCtx->stateTimer = 1;
    }
}

RECOMP_PATCH void Message_DisplaySceneTitleCard(PlayState* play, u16 textId) {
    MessageContext* msgCtx = &play->msgCtx;

    msgCtx->msgLength = 0;
    Message_OpenText(play, textId);
    handle_text_replacement(play, textId);

    func_80150A84(play);
    Message_DecodeNES(play);
    msgCtx->msgMode = MSGMODE_SCENE_TITLE_CARD_FADE_IN_BACKGROUND;
    msgCtx->textDelayTimer = 0;
    msgCtx->textboxColorAlphaCurrent = msgCtx->textboxColorAlphaTarget = msgCtx->textColorAlpha = 0;
    msgCtx->stateTimer = 30;

    // Day/Dawn/Night.. Messages
    if ((msgCtx->currentTextId >= 0x1BB2) && (msgCtx->currentTextId <= 0x1BB6)) {
        XREG(74) = 0x6A;
        XREG(75) = 0;
        XREG(77) = 0x58;
        XREG(76) = 0x44;
    } else {
        XREG(74) = 0x42;
        XREG(75) = 0x1E;
        XREG(77) = 0x3C;
        XREG(76) = 0x1C;
        msgCtx->unk11F1A[0] = msgCtx->unk11F1A[1] = msgCtx->unk11F1A[2] = 0;
        Interface_SetHudVisibility(HUD_VISIBILITY_NONE);
    }
}

void Message_DisplayOcarinaStaffImpl(PlayState* play, u16 ocarinaAction) {
    MessageContext* msgCtx = &play->msgCtx;
    s16 j;
    s16 noStop;
    s32 k;
    u32 i;

    msgCtx->ocarinaAvailableSongs = 0;

    for (i = msgCtx->ocarinaAvailableSongs; i <= (QUEST_SONG_SUN - QUEST_SONG_SONATA); i++) {
        if (CHECK_QUEST_ITEM(QUEST_SONG_SONATA + i)) {
            msgCtx->ocarinaAvailableSongs = msgCtx->ocarinaAvailableSongs | sOcarinaSongFlagsMap[i];
        }
    }

    if (CHECK_QUEST_ITEM(QUEST_SONG_TIME)) {
        msgCtx->ocarinaAvailableSongs |= 0x3000;
    }
    if (CHECK_QUEST_ITEM(QUEST_SONG_LULLABY_INTRO)) {
        msgCtx->ocarinaAvailableSongs |= 0x4000;
    }
    if (CHECK_QUEST_ITEM(QUEST_SONG_LULLABY)) {
        msgCtx->ocarinaAvailableSongs &= (u16)~0x4000;
    }
    if (gSaveContext.save.saveInfo.scarecrowSpawnSongSet) {
        msgCtx->ocarinaAvailableSongs |= 0x400000;
    }
    if (CHECK_EVENTINF(EVENTINF_31)) {
        msgCtx->ocarinaAvailableSongs |= 0x800000;
    }

    msgCtx->ocarinaStaff = AudioOcarina_GetRecordingStaff();

    if ((ocarinaAction == OCARINA_ACTION_PROMPT_EVAN_PART1_SECOND_HALF) ||
        (ocarinaAction == OCARINA_ACTION_PROMPT_EVAN_PART2_SECOND_HALF)) {
        sOcarinaButtonAlphaValues[0] = sOcarinaButtonAlphaValues[1] = sOcarinaButtonAlphaValues[2] =
            sOcarinaButtonAlphaValues[3] = 255;
    } else {
        msgCtx->ocarinaStaff->pos = 0;
        sOcarinaButtonIndexBufPos = 0;
        sOcarinaButtonIndexBufLen = 0;
        Message_ResetOcarinaButtonState(play);
    }

    sLastPlayedSong = 0xFF;
    msgCtx->lastPlayedSong = 0xFF;
    msgCtx->ocarinaButtonIndex = OCARINA_BTN_INVALID;
    noStop = false;
    msgCtx->ocarinaAction = ocarinaAction;

    if ((ocarinaAction >= OCARINA_ACTION_TIMED_PROMPT_SONATA) &&
        (ocarinaAction <= OCARINA_ACTION_TIMED_PROMPT_STORMS)) {
        Message_OpenText(play, 0x1B59);
        handle_text_replacement(play, 0x1B59);
        func_80150A84(play);
    } else if ((ocarinaAction == OCARINA_ACTION_3B) || (ocarinaAction == OCARINA_ACTION_3C)) {
        noStop = true;
        Message_OpenText(play, D_801D028C[ocarinaAction - 0x29]);
        handle_text_replacement(play, D_801D028C[ocarinaAction - 0x29]);
        func_80150A84(play);
    } else if ((ocarinaAction >= OCARINA_ACTION_DEMONSTRATE_EVAN_PART1_FIRST_HALF) &&
               (ocarinaAction <= OCARINA_ACTION_PROMPT_EVAN_PART2_SECOND_HALF)) {
        noStop = true;
        Message_OpenText(play, D_801D028C[ocarinaAction - 0x29]);
        handle_text_replacement(play, D_801D028C[ocarinaAction - 0x29]);
        func_80150A84(play);
    } else if ((ocarinaAction >= OCARINA_ACTION_PROMPT_WIND_FISH_HUMAN) &&
               (ocarinaAction <= OCARINA_ACTION_PROMPT_WIND_FISH_DEKU)) {
        noStop = true;
        Message_OpenText(play, 0x1B59);
        handle_text_replacement(play, 0x1B59);
        func_80150A84(play);
    } else if ((ocarinaAction == OCARINA_ACTION_FREE_PLAY) || (ocarinaAction >= OCARINA_ACTION_CHECK_TIME)) {
        if ((ocarinaAction >= OCARINA_ACTION_CHECK_TIME) && (ocarinaAction <= OCARINA_ACTION_CHECK_STORMS)) {
            Audio_PlaySfx(NA_SE_SY_TRE_BOX_APPEAR);
        }
        if (ocarinaAction == OCARINA_ACTION_SCARECROW_SPAWN_DEMONSTRATION) {
            Message_OpenText(play, 0x1B5B);
            handle_text_replacement(play, 0x1B5B);
            func_80150A84(play);
        } else {
            Message_OpenText(play, 0x1B5A);
            handle_text_replacement(play, 0x1B5A);
            func_80150A84(play);
        }
    } else {
        noStop = true;
        if (ocarinaAction >= OCARINA_ACTION_PROMPT_SONATA) {
            Message_OpenText(play, 0x1B59);
            handle_text_replacement(play, 0x1B59);
            func_80150A84(play);
        } else {
            Message_OpenText(play, D_801D028C[ocarinaAction]);
            handle_text_replacement(play, D_801D028C[ocarinaAction]);
            func_80150A84(play);
        }
    }

    msgCtx->talkActor = NULL;
    msgCtx->textDelayTimer = 0;
    play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;
    msgCtx->textboxXTarget = 34;
    msgCtx->textboxYTarget = 142;
    msgCtx->stateTimer = 8;
    Message_GrowTextbox(play);
    msgCtx->textboxX = 34;
    msgCtx->textboxY = 142;
    msgCtx->unk1200C = 0x200;
    msgCtx->unk1200E = 0x200;
    msgCtx->unk12008 = 0x100;
    msgCtx->unk1200A = 0x40;
    msgCtx->msgMode = MSGMODE_TEXT_CONTINUING;
    msgCtx->textboxColorAlphaCurrent = msgCtx->textboxColorAlphaTarget;
    msgCtx->textboxColorAlphaCurrent = msgCtx->textboxColorAlphaTarget;

    if (!noStop) {
        func_80115844(play, DO_ACTION_STOP);
        noStop = gSaveContext.hudVisibility;
        Interface_SetHudVisibility(HUD_VISIBILITY_B_ALT);
        gSaveContext.hudVisibility = noStop;
    }

    if ((ocarinaAction == OCARINA_ACTION_FREE_PLAY) || (ocarinaAction == OCARINA_ACTION_CHECK_NOTIME)) {
        msgCtx->msgMode = MSGMODE_OCARINA_STARTING;
        msgCtx->textBoxType = TEXTBOX_TYPE_E;
    } else if (ocarinaAction == OCARINA_ACTION_3A) {
        msgCtx->msgMode = MSGMODE_32;
    } else if (ocarinaAction == OCARINA_ACTION_37) {
        msgCtx->msgMode = MSGMODE_2F;
        msgCtx->textBoxType = TEXTBOX_TYPE_2;
    } else if (ocarinaAction == OCARINA_ACTION_SCARECROW_LONG_DEMONSTRATION) {
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
        AudioOcarina_SetInstrument(OCARINA_INSTRUMENT_DEFAULT);
        msgCtx->ocarinaStaff = AudioOcarina_GetPlaybackStaff();
        msgCtx->ocarinaStaff->pos = 0;
        sOcarinaButtonIndexBufLen = 0;
        sOcarinaButtonIndexBufPos = sOcarinaButtonIndexBufLen;
        Message_ResetOcarinaButtonState(play);
        msgCtx->msgMode = MSGMODE_SCARECROW_LONG_DEMONSTRATION;
        AudioOcarina_SetPlaybackSong(OCARINA_SONG_SCARECROW_LONG + 1, 1);
        gSaveContext.hudVisibility = HUD_VISIBILITY_IDLE;
        Interface_SetHudVisibility(HUD_VISIBILITY_NONE);
    }

    for (j = 0, k = 0; j < 48; j++, k += FONT_CHAR_TEX_SIZE) {
        Font_LoadChar(play, 0x8140, k);
    };

    msgCtx->stateTimer = 3;
}