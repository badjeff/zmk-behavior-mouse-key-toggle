/*
 * Copyright (c) 2021 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#define DT_DRV_COMPAT zmk_behavior_mouse_key_toggle

#include <zephyr/device.h>
#include <drivers/behavior.h>
#include <zephyr/logging/log.h>

#include <zmk/behavior.h>
#include <zmk/hid.h>
#include <zephyr/input/input.h>
#include <zephyr/dt-bindings/input/input-event-codes.h>

#ifndef ZMK_MOUSE_HID_NUM_BUTTONS
#define ZMK_MOUSE_HID_NUM_BUTTONS 0x05
#endif

LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#if DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT)

struct behavior_mouse_key_toggle_data {
    bool enable;
};

static int behavior_mouse_key_toggle_init(const struct device *dev) { return 0; };

static void process_key_state(const struct device *dev, int32_t val, bool pressed) {
#if IS_ENABLED(CONFIG_ZMK_POINTING)
    for (int i = 0; i < ZMK_MOUSE_HID_NUM_BUTTONS; i++) {
        if (val & BIT(i)) {
            WRITE_BIT(val, i, 0);
            input_report_key(dev, INPUT_BTN_0 + i, pressed ? 1 : 0, val == 0, K_FOREVER);
        }
    }
#endif
}

static int on_keymap_binding_pressed(struct zmk_behavior_binding *binding,
                                     struct zmk_behavior_binding_event event) {
    LOG_DBG("position %d keycode 0x%02X", event.position, binding->param1);

    const struct device *dev = zmk_behavior_get_binding(binding->behavior_dev);
    struct behavior_mouse_key_toggle_data *data = dev->data;

    data->enable = !data->enable;
    process_key_state(zmk_behavior_get_binding(binding->behavior_dev), binding->param1, data->enable);

    return 0;
}

static const struct behavior_driver_api behavior_mouse_key_toggle_driver_api = {
    .binding_pressed = on_keymap_binding_pressed,
};

#define MKTG_INST(n)                                                                               \
    static struct behavior_mouse_key_toggle_data behavior_mouse_key_toggle_data_##n = {};          \
    BEHAVIOR_DT_INST_DEFINE(n, behavior_mouse_key_toggle_init, NULL,                               \
                            &behavior_mouse_key_toggle_data_##n, NULL, POST_KERNEL,                \
                            CONFIG_KERNEL_INIT_PRIORITY_DEFAULT,                                   \
                            &behavior_mouse_key_toggle_driver_api);

DT_INST_FOREACH_STATUS_OKAY(MKTG_INST)

#endif /* DT_HAS_COMPAT_STATUS_OKAY(DT_DRV_COMPAT) */
