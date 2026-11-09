#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, CONFIG_SENSOR_LOG_LEVEL);

/* Function declarations */
static int our_driver_init(const struct device *dev);

static int our_driver_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan);

static int our_driver_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val);

/* Sensor API */
static DEVICE_API(sensor, our_driver_api) = {
    .sample_fetch = our_driver_sample_fetch,
    .channel_get = our_driver_channel_get,
};

/* Driver implementation */
static int our_driver_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan)
{
    LOG_INF("Hello from sample fetch");

    return 0;
}

static int our_driver_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val)
{
    LOG_INF("Hello from channel get: %d", chan);

    val->val1 = 25;
    val->val2 = 500000;

    return 0;
}

/* Initialization */
static int our_driver_init(const struct device *dev)
{
    LOG_INF("DRIVER INITIALIZED");

    return 0;
}

/* Device registration */
#define OUR_DRIVER_INIT(inst)                         \
    DEVICE_DT_INST_DEFINE(inst,                        \
                          our_driver_init,             \
                          NULL,                        \
                          NULL,                        \
                          NULL,                        \
                          POST_KERNEL,                 \
                          CONFIG_SENSOR_INIT_PRIORITY, \
                          &our_driver_api);

DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_INIT)