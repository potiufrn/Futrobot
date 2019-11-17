deps_config := \
	/home/lgabriel/esp/esp-idf/components/app_trace/Kconfig \
	/home/lgabriel/esp/esp-idf/components/aws_iot/Kconfig \
	/home/lgabriel/esp/esp-idf/components/bt/Kconfig \
	/home/lgabriel/esp/esp-idf/components/driver/Kconfig \
	/home/lgabriel/esp/esp-idf/components/efuse/Kconfig \
	/home/lgabriel/esp/esp-idf/components/esp32/Kconfig \
	/home/lgabriel/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/lgabriel/esp/esp-idf/components/esp_event/Kconfig \
	/home/lgabriel/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/lgabriel/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/lgabriel/esp/esp-idf/components/esp_https_ota/Kconfig \
	/home/lgabriel/esp/esp-idf/components/espcoredump/Kconfig \
	/home/lgabriel/esp/esp-idf/components/ethernet/Kconfig \
	/home/lgabriel/esp/esp-idf/components/fatfs/Kconfig \
	/home/lgabriel/esp/esp-idf/components/freemodbus/Kconfig \
	/home/lgabriel/esp/esp-idf/components/freertos/Kconfig \
	/home/lgabriel/esp/esp-idf/components/heap/Kconfig \
	/home/lgabriel/esp/esp-idf/components/libsodium/Kconfig \
	/home/lgabriel/esp/esp-idf/components/log/Kconfig \
	/home/lgabriel/esp/esp-idf/components/lwip/Kconfig \
	/home/lgabriel/esp/esp-idf/components/mbedtls/Kconfig \
	/home/lgabriel/esp/esp-idf/components/mdns/Kconfig \
	/home/lgabriel/esp/esp-idf/components/mqtt/Kconfig \
	/home/lgabriel/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/lgabriel/esp/esp-idf/components/openssl/Kconfig \
	/home/lgabriel/esp/esp-idf/components/pthread/Kconfig \
	/home/lgabriel/esp/esp-idf/components/spi_flash/Kconfig \
	/home/lgabriel/esp/esp-idf/components/spiffs/Kconfig \
	/home/lgabriel/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/lgabriel/esp/esp-idf/components/unity/Kconfig \
	/home/lgabriel/esp/esp-idf/components/vfs/Kconfig \
	/home/lgabriel/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/lgabriel/esp/esp-idf/components/wifi_provisioning/Kconfig \
	/home/lgabriel/esp/esp-idf/components/app_update/Kconfig.projbuild \
	/home/lgabriel/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/lgabriel/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/lgabriel/workarea-Futrobot/Futrobot/firmware/esp-firmware-test/main/Kconfig.projbuild \
	/home/lgabriel/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/lgabriel/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_TARGET)" "esp32"
include/config/auto.conf: FORCE
endif
ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
