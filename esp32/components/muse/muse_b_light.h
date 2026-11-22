#pragma once

#include "sdkconfig.h"
#include "lvgl.h"

#ifdef CONFIG_MUSE_UI_B_LIGHT
#include "muse_state.h"

#define MUSE_B_LIGHT 1
#define MUSE_B_PORTRAIT_PX 208

enum {
    MUSE_B_GROUND = 0xf1f3f7,
    MUSE_B_CARD = 0xfafbfd,
    MUSE_B_INK = 0x1c1c1f,
    MUSE_B_DIM = 0x6b7280,
    MUSE_B_BLUE = 0x2563eb,
    MUSE_B_TINT = 0xd4e4fd,
    MUSE_B_TRACK = 0xdce2ec,
    MUSE_B_DOT = 0xaab2c2,
    MUSE_B_AMBER = 0xd97706,
    MUSE_B_AMBER_TINT = 0xfef3c7,
    MUSE_B_RED = 0xdc2626,
    MUSE_B_RED_TINT = 0xfee2e2,
    MUSE_B_SLEEP = 0x1f2933,
};

#define MUSE_B_COLOR(light, legacy) (light)

const lv_font_t *muse_b_font(const lv_font_t *base);
const char *muse_b_text(const char *english);
const lv_image_dsc_t *muse_b_portrait(muse_mode_t mode, bool happy);
void muse_b_surface(lv_obj_t *obj, uint32_t color, int radius, int pad_h, int pad_v);

#else
#define MUSE_B_LIGHT 0
#define MUSE_B_COLOR(light, legacy) (legacy)
#endif
