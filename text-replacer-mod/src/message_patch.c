#include "tweak_globals.h"
#include "message_patch.h"
void Message_OpenText(PlayState* play, u16 textId);

RECOMP_IMPORT(".", void text_replacer_on_load());

RECOMP_CALLBACK("*", recomp_on_init) void load_lib () {
    text_replacer_on_load();
}

RECOMP_PATCH void Message_StartTextbox(PlayState* play, u16 textId, Actor* actor) {
    MessageContext* msgCtx = &play->msgCtx;
    Font* font = &msgCtx->font;

    msgCtx->ocarinaAction = 0xFFFF;
    Message_OpenText(play, textId);

    recomp_printf("Message: ");

    for (int i = 0; i < msgCtx->msgLength; i++) {
        recomp_printf("%c", font->msgBuf.schar[i]);
    }

    msgCtx->talkActor = actor;
    msgCtx->msgMode = MSGMODE_TEXT_START;
    msgCtx->stateTimer = 0;
    msgCtx->textDelayTimer = 0;
    play->msgCtx.ocarinaMode = OCARINA_MODE_NONE;
}