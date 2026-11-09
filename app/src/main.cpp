
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/device.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static int our_board_driver_test(void)
{
    const struct device *driver =
        DEVICE_DT_GET(DT_NODELABEL(our_driver0));

    if (!device_is_ready(driver)) {
        LOG_ERR("Our driver is not ready");
        return -ENODEV;
    }

    struct sensor_value val;

    // Task 1: Turn LED ON
    int ret = sensor_sample_fetch(driver);

    if (ret < 0) {
        LOG_ERR("Sensor sample fetch failed: %d", ret);
        return ret;
    }

    LOG_INF("LED turned ON");

    k_msleep(1000);

    // Task 1: Turn LED OFF
    ret = sensor_channel_get(
        driver,
        SENSOR_CHAN_AMBIENT_TEMP,
        &val
    );

    if (ret < 0) {
        LOG_ERR("Sensor channel get failed: %d", ret);
        return ret;
    }

    LOG_INF("LED turned OFF");

    return 0;
}

int main(void)
{
    while (1) {
        our_board_driver_test();

        k_msleep(1000);
    }

    return 0;
}
