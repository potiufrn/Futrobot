deps_config := \
	/home/poti/esp/esp-idf/components/app_trace/Kconfig \
	/home/poti/esp/esp-idf/components/aws_iot/Kconfig \
	/home/poti/esp/esp-idf/components/bt/Kconfig \
	/home/poti/esp/esp-idf/components/driver/Kconfig \
	/home/poti/esp/esp-idf/components/esp32/Kconfig \
	/home/poti/esp/esp-idf/components/esp_adc_cal/Kconfig \
	/home/poti/esp/esp-idf/components/esp_event/Kconfig \
	/home/poti/esp/esp-idf/components/esp_http_client/Kconfig \
	/home/poti/esp/esp-idf/components/esp_http_server/Kconfig \
	/home/poti/esp/esp-idf/components/ethernet/Kconfig \
	/home/poti/esp/esp-idf/components/fatfs/Kconfig \
	/home/poti/esp/esp-idf/components/freemodbus/Kconfig \
	/home/poti/esp/esp-idf/components/freertos/Kconfig \
	/home/poti/esp/esp-idf/components/heap/Kconfig \
	/home/poti/esp/esp-idf/components/libsodium/Kconfig \
	/home/poti/esp/esp-idf/components/log/Kconfig \
	/home/poti/esp/esp-idf/components/lwip/Kconfig \
	/home/poti/esp/esp-idf/components/mbedtls/Kconfig \
	/home/poti/esp/esp-idf/components/mdns/Kconfig \
	/home/poti/esp/esp-idf/components/mqtt/Kconfig \
	/home/poti/esp/esp-idf/components/nvs_flash/Kconfig \
	/home/poti/esp/esp-idf/components/openssl/Kconfig \
	/home/poti/esp/esp-idf/components/pthread/Kconfig \
	/home/poti/esp/esp-idf/components/spi_flash/Kconfig \
	/home/poti/esp/esp-idf/components/spiffs/Kconfig \
	/home/poti/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/home/poti/esp/esp-idf/components/vfs/Kconfig \
	/home/poti/esp/esp-idf/components/wear_levelling/Kconfig \
	/home/poti/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/poti/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/poti/LGabriel/Futrobot/firmware/esp-firmware/main/Kconfig.projbuild \
	/home/poti/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/poti/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)

ifneq "$(IDF_CMAKE)" "n"
include/config/auto.conf: FORCE
endif

$(deps_config): ;
