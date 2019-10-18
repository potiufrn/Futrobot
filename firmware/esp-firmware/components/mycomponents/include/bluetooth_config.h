#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"


bool initBluetooth(const char* deviceName, esp_spp_cb_t *esp_spp_cb);
