#include "tweak_globals.h"
#include "message_patch.h"
void Message_OpenText(PlayState* play, u16 textId);

RECOMP_IMPORT(".", void text_replacer_lib_startup());
RECOMP_IMPORT(".", void text_replacer_lib_set_char_in_buffer(char c, u32 pos));
RECOMP_IMPORT(".", void text_replacer_lib_add_buffer_as_entry(u32 id, u32 len));

RECOMP_CALLBACK("*", recomp_on_init) void load_lib () {
    text_replacer_lib_startup();
}

void Message_FindMessage(PlayState* play, u16 textId);
void Message_GrowTextbox(PlayState* play);
void func_80150A84(PlayState* play);

RECOMP_PATCH void Message_StartTextbox(PlayState* play, u16 textId, Actor* actor) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;

    msgCtx->ocarinaAction = 0xFFFF;
    Message_OpenText(play, textId);

    for (int i = 0; i < msgCtx->msgLength; i++) {
        text_replacer_lib_set_char_in_buffer(font->msgBuf.schar[i], i);
        // recomp_printf("%c", font->msgBuf.schar[i]);
    }

    text_replacer_lib_add_buffer_as_entry(textId, msgCtx->msgLength);

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
    func_80150A84(play);

    for (int i = 0; i < msgCtx->msgLength; i++) {
        text_replacer_set_char_in_buffer(font->msgBuf.schar[i], i);
        // recomp_printf("%c", font->msgBuf.schar[i]);
    }

    text_replacer_add_buffer_as_entry(textId, msgCtx->msgLength);

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