#include "freertos/FreeRTOS.h"
#include "esp_wifi.h"
#include "esp_system.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "esp_log.h"

#include "RV8803_ESP32.h"

#define I2C_MASTER_SCL_IO 35        /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO 36        /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_1    /*!< I2C port number for master dev */
#define I2C_MASTER_FREQ_HZ 400000   /*!< I2C master clock frequency */

static char *getUTCTime(time_t now, char *buffer, int len) {
	struct tm timeinfo;

	gmtime_r(&now, &timeinfo);
	strftime(buffer, len, "%FT%T-00:00", &timeinfo);
	return buffer;
}

extern "C" void app_main(void)
{
	char strftime_buf[64];
    setenv("TZ", "MST7MDT", 1);
    tzset();
	struct timeval rtc_tv;

	i2c_config_t conf;
    conf.mode = I2C_MODE_MASTER;
    conf.sda_io_num = (gpio_num_t)I2C_MASTER_SDA_IO;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_io_num = (gpio_num_t)I2C_MASTER_SCL_IO;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
    conf.clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL;

    i2c_param_config(I2C_MASTER_NUM, &conf);
    i2c_driver_install(I2C_MASTER_NUM, conf.mode, 0, 0, 0);

    RV8803 *rtc = new RV8803(I2C_MASTER_NUM, 0x32);
	rtc->set24Hour();
	rtc->getTimeval(&rtc_tv);

	ESP_LOGI("RTC", "%s", getUTCTime(rtc_tv.tv_sec, strftime_buf, sizeof(strftime_buf)));


}

