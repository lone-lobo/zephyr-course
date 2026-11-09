
#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#include <zephyr/device.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int our_driver_set_toggle_count(
    const struct device *dev,
    uint32_t count
);

int our_driver_get_toggle_count(
    const struct device *dev,
    uint32_t *count
);

#ifdef __cplusplus
}
#endif

#endif
