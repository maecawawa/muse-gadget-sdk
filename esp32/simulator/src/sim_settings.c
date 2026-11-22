/* Device services used by the settings preview. No external I/O or storage. */
#include <string.h>

#include "esp_app_desc.h"
#include "esp_mac.h"
#include "muse_audio.h"
#include "muse_battery.h"
#include "muse_ble.h"
#include "muse_chat.h"
#include "muse_input.h"
#include "muse_link.h"
#include "muse_settings.h"
#include "muse_state.h"
#include "muse_voice.h"
#include "muse_wifi.h"
#include "sim_services.h"

static bool s_wifi_on = true;
static bool s_ble_on = true;
static int s_gain = 30;
static int s_sleep = 120;

bool muse_settings_wifi_on(void) { return s_wifi_on; }
void muse_settings_set_wifi_on(bool on) { s_wifi_on = on; }
void muse_wifi_apply(void) { sim_services_set_wifi(s_wifi_on ? MUSE_WIFI_CONNECTED : MUSE_WIFI_OFF, NULL); }
bool muse_settings_ble_on(void) { return s_ble_on; }
void muse_settings_set_ble_on(bool on)
{
    s_ble_on = on;
    sim_services_set_ble(on ? MUSE_BLE_ADVERTISING : MUSE_BLE_OFF, NULL, 0);
}
int muse_settings_mic_gain(void) { return s_gain; }
void muse_settings_set_mic_gain(int gain) { s_gain = gain; }
void muse_audio_set_mic_gain(int gain) { (void)gain; }
int muse_settings_sleep_s(void) { return s_sleep; }
void muse_settings_set_sleep_s(int secs) { s_sleep = secs; }
void muse_settings_set_text_size(int size) { muse_settings_set_text_size_default(size); }
void muse_voice_set_monitor(bool on) { (void)on; }
float muse_voice_monitor_db(void) { return -48.0f; }
void muse_voice_request_chirp(void) {}
void muse_battery_read(muse_battery_t *out) { *out = (muse_battery_t){0}; }
bool muse_battery_drain(const muse_battery_t *b, int *rate, int *hours) { (void)b; (void)rate; (void)hours; return false; }
void muse_battery_reset(void) {}
void muse_ble_forget_all(void) {}
void muse_hatch_test(void) {}
void muse_input_request_power_off(void) { muse_state_set_mode(MUSE_MODE_OFF); }
void muse_input_request_restart(void) { muse_state_set_mode(MUSE_MODE_BOOT); }
void muse_link_reset_setup(void) {}
bool muse_link_sdk_token_set(const char *text) { (void)text; return false; }
int muse_link_wifi_proxy_get(const char *ssid, network_proxy_config_t *proxy) { (void)ssid; (void)proxy; return -1; }
bool muse_link_wifi_proxy_set(const char *ssid, const network_proxy_config_t *proxy) { (void)ssid; (void)proxy; return false; }
bool network_proxy_valid(const network_proxy_config_t *proxy) { (void)proxy; return false; }
void network_proxy_wipe(void *data, unsigned len) { memset(data, 0, len); }
void muse_settings_hatch_host(char out[MUSE_HOST_MAX + 1]) { out[0] = '\0'; }
void muse_settings_hatch_vm(char out[MUSE_VM_MAX + 1]) { out[0] = '\0'; }
void muse_settings_set_hatch_host(const char *text) { (void)text; }
void muse_settings_set_hatch_vm(const char *text) { (void)text; }
esp_err_t muse_settings_set_hatch_token(const char *text, bool check) { (void)text; (void)check; return ESP_FAIL; }
size_t muse_settings_hatch_token_len(void) { return 0; }
bool muse_link_hatch_linked(void) { return false; }
const char *muse_link_sdk_token(void) { return NULL; }
bool muse_link_sdk_token_changed(void) { return false; }
const char *muse_hatch_state_name(muse_hatch_state_t state)
{
    return state == MUSE_HATCH_REACHABLE ? "Reachable" : "Not set";
}
const char *muse_link_state_name(muse_link_state_t state)
{
    return state == MUSE_LINK_ONLINE ? "Connected" : "Unpaired";
}
const esp_app_desc_t *esp_app_get_description(void)
{
    static const esp_app_desc_t desc = { .version = "preview" };
    return &desc;
}
esp_err_t esp_read_mac(uint8_t *mac, int type) { (void)mac; (void)type; return ESP_FAIL; }
void muse_wifi_forget(const char *ssid) { (void)ssid; }
int muse_wifi_saved(muse_wifi_saved_t *out, int max) { (void)out; (void)max; return 0; }
int muse_wifi_scan_results(muse_wifi_ap_t *out, int max, uint32_t *gen) { (void)out; (void)max; *gen = 0; return 0; }
bool muse_wifi_scanning(void) { return false; }
esp_err_t muse_wifi_scan(void) { return ESP_OK; }
void muse_settings_set_wifi(const char *ssid, const char *password) { (void)ssid; (void)password; }
