#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    KEY_UNKNOWN,

    KEY_LEFT,
    KEY_RIGHT,
    KEY_UP,
    KEY_DOWN,

    KEY_INSERT,
    KEY_DELETE,
    KEY_PAGE_UP,
    KEY_PAGE_DOWN,
    KEY_HOME,
    KEY_END,
    KEY_BACKSPACE,
    KEY_ENTER,
    KEY_SPACE,
    KEY_TAB,
    KEY_ESCAPE,
    KEY_TILDA,

    KEY_F1,
    KEY_F2,
    KEY_F3,
    KEY_F4,
    KEY_F5,
    KEY_F6,
    KEY_F7,
    KEY_F8,
    KEY_F9,
    KEY_F10,
    KEY_F11,
    KEY_F12,

    KEY_0,
    KEY_1,
    KEY_2,
    KEY_3,
    KEY_4,
    KEY_5,
    KEY_6,
    KEY_7,
    KEY_8,
    KEY_9,

    KEY__0,
    KEY__1,
    KEY__2,
    KEY__3,
    KEY__4,
    KEY__5,
    KEY__6,
    KEY__7,
    KEY__8,
    KEY__9,

    KEY_A,
    KEY_B,
    KEY_C,
    KEY_D,
    KEY_E,
    KEY_F,
    KEY_G,
    KEY_H,
    KEY_I,
    KEY_J,
    KEY_K,
    KEY_L,
    KEY_M,
    KEY_N,
    KEY_O,
    KEY_P,
    KEY_Q,
    KEY_R,
    KEY_S,
    KEY_T,
    KEY_U,
    KEY_V,
    KEY_W,
    KEY_X,
    KEY_Y,
    KEY_Z,
    KEY_COUNT
} Key;

typedef struct {
    Key key;
    const char* name;
} KeyInfo;

extern const KeyInfo keyInfos[KEY_COUNT];

const char* getKeyName(Key key);

void dpad();

#ifdef __cplusplus
}
#endif