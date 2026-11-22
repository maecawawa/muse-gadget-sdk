#include "muse_b_light.h"
#include <string.h>

LV_FONT_DECLARE(muse_font_cjk_noto_16)
LV_FONT_DECLARE(muse_font_b_ja_16)

extern const uint8_t muse_b_portrait_data[] __asm__("_binary_muse_b_portraits_rgb565_start");

#define PORTRAIT_BYTES (MUSE_B_PORTRAIT_PX * MUSE_B_PORTRAIT_PX * 2)
#define PORTRAIT(index) { \
    .header = { .magic = LV_IMAGE_HEADER_MAGIC, .cf = LV_COLOR_FORMAT_RGB565, \
                .w = MUSE_B_PORTRAIT_PX, .h = MUSE_B_PORTRAIT_PX, .stride = MUSE_B_PORTRAIT_PX * 2 }, \
    .data_size = PORTRAIT_BYTES, .data = muse_b_portrait_data + (index) * PORTRAIT_BYTES \
}

const lv_image_dsc_t *muse_b_portrait(muse_mode_t mode, bool happy)
{
    static const lv_image_dsc_t portraits[MUSE_MODE_COUNT] = {
        [MUSE_MODE_BOOT] = PORTRAIT(0),
        [MUSE_MODE_IDLE] = PORTRAIT(1),
        [MUSE_MODE_LISTENING] = PORTRAIT(2),
        [MUSE_MODE_THINKING] = PORTRAIT(3),
        [MUSE_MODE_SPEAKING] = PORTRAIT(4),
        [MUSE_MODE_ERROR] = PORTRAIT(6),
        [MUSE_MODE_OFF] = PORTRAIT(7),
    };
    static const lv_image_dsc_t reaction = PORTRAIT(5);
    return mode == MUSE_MODE_IDLE && happy ? &reaction : &portraits[mode];
}

const lv_font_t *muse_b_font(const lv_font_t *base)
{
    static lv_font_t fonts[4], cjk;
    int index = base == &lv_font_montserrat_14 ? 0 :
                base == &lv_font_montserrat_20 ? 2 :
                base == &lv_font_montserrat_28 ? 3 : 1;
    static const lv_font_t *const bases[] = {
        &lv_font_montserrat_14, &lv_font_montserrat_16,
        &lv_font_montserrat_20, &lv_font_montserrat_28,
    };
    if (!fonts[index].get_glyph_dsc) {
        if (!cjk.get_glyph_dsc) {
            cjk = muse_font_cjk_noto_16;
            cjk.fallback = &muse_font_b_ja_16;
        }
        fonts[index] = *bases[index];
        fonts[index].fallback = &cjk;
        fonts[index].line_height = LV_MAX(fonts[index].line_height, 21);
    }
    return &fonts[index];
}

void muse_b_surface(lv_obj_t *obj, uint32_t color, int radius, int pad_h, int pad_v)
{
    lv_obj_set_style_bg_color(obj, lv_color_hex(color), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(obj, radius, 0);
    lv_obj_set_style_pad_hor(obj, pad_h, 0);
    lv_obj_set_style_pad_ver(obj, pad_v, 0);
}

const char *muse_b_text(const char *english)
{
    static const struct { const char *en; const char *ja; } labels[] = {
        { "WAKING UP", "起動中" },
        { "UNPAIRED", "未ペア" },
        { "SET UP MUSE FIRST", "先にMuseを設定してください" },
        { "READY", "待機中" },
        { "WI-FI OFF", "Wi-Fiオフ" },
        { "SET UP WI-FI", "Wi-Fiを設定" },
        { "NO WI-FI", "Wi-Fiがありません" },
        { "CONNECTING", "接続中" },
        { "RECONNECTING", "接続中" },
        { "LISTENING", "聞いています" },
        { "THINKING", "考え中" },
        { "SPEAKING", "話しています" },
        { "WORKING", "考え中" },
        { "ERROR", "エラー" },
        { "AUDIO INIT FAILED", "音声機能を起動できませんでした" },
        { "NO REPLY YET", "まだ返信がありません" },
        { "CONNECTING TO MUSE...", "Museに接続中…" },
        { "NOTE SENT - WAITING FOR MUSE", "送信しました。Museの返信を待っています" },
        { "RECORDING...", "録音中…" },
        { "SENDING VOICE NOTE", "音声を送信中…" },
        { "ONE MOMENT...", "少しお待ちください" },
        { "saved", "保存済み" }, { "secured", "保護あり" }, { "open", "保護なし" },
        { "saved  ", "保存済み  " }, { "open  ", "保護なし  " },
        { "No battery", "バッテリーなし" },
        { "Unplug USB to start measuring.", "USBを抜くと計測を開始します。" },
        { "%d h %d min", "%d時間%d分" }, { "%d min", "%d分" },
        { "On battery for %s", "バッテリーで%s使用中" },
        { "Last run: %s on battery", "前回のバッテリー使用時間 %s" },
        { "lasts ~%d h", "約%d時間" }, { "%d%% so far", "これまでに%d%%" },
        { "Custom", "カスタム" },
        { "Cloud connections use this network's proxy. Local traffic stays direct.",
          "クラウドに接続するときにこのプロキシを使います。" },
        { "Muse remembers up to 8 networks and joins the strongest one in range. Tap a saved one twice to forget it.",
          "ネットワークを8件まで保存できます。保存先を削除するには2回タップしてください。" },
        { "Pair with the Muse app to use your account; a device token here overrides it, and a long one is easier to send over Bluetooth. The VM ID picks one of your VMs. The SDK token is from gadgets.muse.ai; one set here replaces the firmware's. Reset pairing forgets Wi-Fi and the app pairing, then restarts.",
          "Museアプリとペアリングしてアカウントを使います。ここで設定したトークンが優先されます。ペアリングを解除するとWi-Fiの保存先も削除され、再起動します。" },
        { "When on, Muse is visible to phones nearby. Open tools/muse/ble_setup.html in Chrome, connect, and enter the code Muse shows to pair.",
          "オンにすると近くのスマホから接続できます。Chromeでtools/muse/ble_setup.htmlを開き、画面のコードを入力してください。" },
        { "History", "履歴" },
        { "GOODBYE!", "さようなら" },
        { "HOLD TO POWER OFF", "長押しで電源を切る" },
        { "COULDN'T POWER OFF", "電源を切れませんでした" },
        { "RESTARTING...", "再起動中…" },
        { "Unpaired", "未ペア" }, { "not paired", "未ペア" },
        { "paired", "ペアリング済み" },
        { "Check server, port and authentication", "サーバー、ポート、認証情報を確認してください" },
        { "Busy or network forgotten; try again", "ネットワークを確認してもう一度お試しください" },
        { "Save failed; try again", "保存できませんでした。もう一度お試しください" },
        { "Talk at arm's length: the bar should reach green (-30 to -15 dBFS) without going orange.",
          "腕を伸ばした距離で話し、マイクレベルが−30〜−15 dBFSになるように調整してください。" },
        { "Measures from unplugging USB until it's plugged back in. The gauge moves in 1% steps, so give it a few hours. Chip asleep is time in light sleep; CPU busy is time a core was running a task.",
          "USBを抜いてから次につなぐまでを計測します。残量は1%刻みのため、数時間を目安にしてください。" },
        { "None", "なし" },
        { "Also kept awake by: %s", "スリープを妨げている処理 %s" },
        { "Joining", "接続中" }, { "Failed", "接続できません" },
        { "Not nearby", "圏外" },
        { "Extra small", "最小" }, { "Small", "小" }, { "Medium", "中" },
        { "Large", "大" }, { "Extra large", "最大" },
        { "Not a token", "トークンを確認してください" },
        { "Muse server", "Museサーバー" },
        { "Empty keeps the current one", "空欄のままなら現在の値を使います" },
        { "Empty keeps the current one; a new one restarts", "新しい値を保存すると再起動します" },
        { "Empty for the default", "空欄の場合は既定値を使います" },
        { "Optional", "任意" },
        { "GOODBYE", "さようなら" },
        { "USB POWER", "USB給電" },
        { "CHARGING %d%%", "充電中 %d%%" },
        { "BATTERY %d%%", LV_SYMBOL_BATTERY_FULL " %d%%" },
        { "Pairing code", "ペアリングコード" },
        { "Pair with Muse app", "Museアプリとペアリング" },
        { "Press button", "ボタンを押す" },
        { "Press the %s button", "%sのボタンを押してください" },
        { "Tap screen", "画面をタップ" },
        { "Tap to confirm pairing", "タップしてペアリング" },
        { "Enter it on your phone", "スマホにコードを入力してください" },
        { "top-right", "右上" },
        { "top-left", "左上" },
        { "red bottom-left", "左下の赤" },
        { "Options", "選択肢" },
        { "Choose an option", "選択してください" },
        { "Answer selected", "選択済み" },
        { "Sending answer...", "送信中…" },
        { "Continue in the Muse app", "Museアプリで続けてください" },
        { "SETTINGS", "設定" },
        { "WI-FI", "Wi-Fi" },
        { "MUSE", "Muse" },
        { "BLUETOOTH", "Bluetooth" },
        { "Sound", "サウンド" }, { "SOUND", "サウンド" },
        { "Sleep", "自動スリープ" }, { "AUTO-SLEEP", "自動スリープ" },
        { "Battery", "バッテリー" }, { "BATTERY", "バッテリー" },
        { "Power off", "電源を切る" }, { "POWER", "電源を切る" },
        { "Screen", "画面" }, { "SCREEN", "画面" },
        { "PROXY", "プロキシ" },
        { "Configure Proxy", "プロキシを設定" },
        { "Server", "サーバー" },
        { "Port", "ポート" },
        { "Username", "ユーザー名" },
        { "Password", "パスワード" },
        { "Wi-Fi password", "Wi-Fiパスワード" },
        { "Show", "表示" }, { "Hide", "隠す" },
        { "Save", "保存" }, { "Saved", "保存しました" },
        { "Saving...", "保存中…" },
        { "Saved networks", "保存済みのネットワーク" },
        { "No networks found", "ネットワークが見つかりません" },
        { LV_SYMBOL_REFRESH "  Scan for networks", LV_SYMBOL_REFRESH "  ネットワークを検索" },
        { "Scanning...", "検索中…" },
        { "Other network...", "ほかのネットワーク…" },
        { "MAC address", "MACアドレス" },
        { "Wi-Fi is off", "Wi-Fiはオフです" },
        { "No saved networks. Scan and pick one.", "保存先がありません。検索して選んでください。" },
        { "Joining %s\n%s", "%sに接続中\n%s" },
        { "Connected to %s\n%s", "%sに接続済み\n%s" },
        { "Connected to %s\n%s  -  %d dBm", "%sに接続済み\n%s  -  %d dBm" },
        { "No saved network nearby\nLooking again within a minute", "Wi-Fiがありません\n1分以内に再検索します" },
        { "Couldn't join %s\n%s", "%sに接続できませんでした\n%s" },
        { "Reset pairing", "ペアリングを解除" },
        { "Tap again to reset", "もう一度タップして解除" },
        { "Resetting...", "解除中…" },
        { "Device token", "デバイストークン" },
        { "SDK token", "SDKトークン" },
        { "Test connection", "接続をテスト" },
        { "Muse app: %s\n%s", "Museアプリ %s\n%s" },
        { "Not set", "未設定" },
        { "Phone setup", "スマホ設定" },
        { "Forget paired phones", "スマホとのペアリングを解除" },
        { "Visible as %s", "表示名 %s" },
        { "Phone connected\n%s", "スマホと接続済み\n%s" },
        { "Paired", "ペアリング済み" },
        { "Waiting for pairing", "ペアリング待ち" },
        { "Connected", "接続済み" }, { "Connecting", "接続中" },
        { "Reachable", "接続済み" }, { "Unreachable", "接続できません" },
        { "None nearby", "圏外" },
        { "Off", "オフ" }, { "On", "オン" },
        { "Speaker", "スピーカー" }, { "Volume", "音量" },
        { "Muted", "消音" }, { "Mic only", "マイクのみ" }, { "No audio", "音声なし" },
        { "Mic gain", "マイクゲイン" }, { "Mic level", "マイクレベル" },
        { "Brightness", "明るさ" }, { "Text size", "文字サイズ" },
        { "Never", "しない" }, { "30 seconds", "30秒" },
        { "1 minute", "1分" }, { "2 minutes", "2分" },
        { "5 minutes", "5分" }, { "10 minutes", "10分" },
        { "30 sec", "30秒" }, { "1 min", "1分" }, { "2 min", "2分" },
        { "5 min", "5分" }, { "10 min", "10分" },
        { LV_SYMBOL_EYE_CLOSE "  Sleep now", LV_SYMBOL_EYE_CLOSE "  今すぐスリープ" },
        { "Turn the screen off after Muse has been idle for:", "画面が消えるまでの待ち時間" },
        { "Tap the screen or press either button to wake.", "画面のタップかボタンで復帰します。" },
        { "Used", "使用量" }, { "A full charge", "満充電からの目安" },
        { "Screen off", "画面オフ" }, { "Chip asleep", "チップスリープ" },
        { "Wakes", "復帰回数" }, { "CPU busy", "CPU使用中" },
        { "measuring", "計測中" },
        { LV_SYMBOL_POWER "  Power off", LV_SYMBOL_POWER "  電源を切る" },
        { LV_SYMBOL_REFRESH "  Restart", LV_SYMBOL_REFRESH "  再起動" },
        { LV_SYMBOL_REFRESH "  Start over", LV_SYMBOL_REFRESH "  計測し直す" },
        { "Cancel", "キャンセル" },
        { "Power Muse off completely?", "電源を切りますか？" },
        { "Press the %s button to turn it back on.", "電源を入れるには%sのボタンを押してください。" },
        { "Press the %s button to turn it back on. To just turn the screen off, press the %s button.",
          "電源を入れるには%sのボタンを押してください。画面だけを消すには%sのボタンを押してください。" },
    };
    for (size_t i = 0; i < sizeof(labels) / sizeof(labels[0]); i++) {
        if (strcmp(english, labels[i].en) == 0) return labels[i].ja;
    }
    return english;
}
