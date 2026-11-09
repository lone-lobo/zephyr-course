#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <our_driver/our_driver.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

struct our_driver_config {
    struct gpio_dt_spec led;
};

struct our_driver_data {
    bool led_state;
    uint32_t toggle_count;
};

static int our_driver_sample_fetch(
    const struct device *dev,
    enum sensor_channel chan)
{
    const struct our_driver_config *config = dev->config;
    struct our_driver_data *data = dev->data;

    int ret = gpio_pin_set_dt(&config->led, 1);

    if (ret < 0) {
        return ret;
    }

    if (!data->led_state) {
        data->toggle_count++;
    }

    data->led_state = true;

    LOG_INF("LED ON | Toggle count: %u",
            data->toggle_count);

    return 0;
}

static int our_driver_channel_get(
    const struct device *dev,
    enum sensor_channel chan,
    struct sensor_value *val)
{
    const struct our_driver_config *config = dev->config;
    struct our_driver_data *data = dev->data;

    int ret = gpio_pin_set_dt(&config->led, 0);

    if (ret < 0) {
        return ret;
    }

    if (data->led_state) {
        data->toggle_count++;
    }

    data->led_state = false;

    val->val1 = 0;
    val->val2 = 0;

    LOG_INF("LED OFF | Toggle count: %u",
            data->toggle_count);

    return 0;
}

int our_driver_set_toggle_count(
    const struct device *dev,
    uint32_t count)
{
    if (dev == NULL || !device_is_ready(dev)) {
        return -ENODEV;
    }

    struct our_driver_data *data = dev->data;

    data->toggle_count = count;

    return 0;
}

int our_driver_get_toggle_count(
    const struct device *dev,
    uint32_t *count)
{
    if (dev == NULL || count == NULL) {
        return -EINVAL;
    }

    if (!device_is_ready(dev)) {
        return -ENODEV;
    }

    const struct our_driver_data *data = dev->data;

    *count = data->toggle_count;

    return 0;
}

static int our_driver_init(const struct device *dev)
{
    const struct our_driver_config *config = dev->config;
    struct our_driver_data *data = dev->data;

    if (!gpio_is_ready_dt(&config->led)) {
        return -ENODEV;
    }

    int ret = gpio_pin_configure_dt(
        &config->led,
        GPIO_OUTPUT_INACTIVE
    );

    if (ret < 0) {
        return ret;
    }

    data->led_state = false;
    data->toggle_count = 0;

    LOG_INF("Our LED driver initialized");

    return 0;
}

static DEVICE_API(sensor, our_driver_api) = {
    .sample_fetch = our_driver_sample_fetch,
    .channel_get = our_driver_channel_get,
};

#define OUR_DRIVER_DEFINE(inst)                            \
    static struct our_driver_data data_##inst;             \
    static const struct our_driver_config config_##inst = { \
        .led = GPIO_DT_SPEC_GET(                           \
            DT_PHANDLE(DT_DRV_INST(inst), led), gpios),    \
    };                                                     \
    DEVICE_DT_INST_DEFINE(inst,                            \
        our_driver_init,                                   \
        NULL,                                              \
        &data_##inst,                                      \
        &config_##inst,                                    \
        POST_KERNEL,                                       \
        CONFIG_SENSOR_INIT_PRIORITY,                       \
        &our_driver_api);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)
