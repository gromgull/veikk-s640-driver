/**
 * Module parameters -- see the README for more details
 * @author Jonathan Lam
 */

#include "veikk.h"

// initialize parms with default values
struct veikk_parms veikk_parms = {
    .orientation = 0,
    .da_x = 0, .da_y = 0, .da_width = 0, .da_height = 0,
    .screen_width = 0, .screen_height = 0,
    .pressure_map = 0
};

// parm ops
static const struct kernel_param_ops orientation_ops = {
    .set = &param_set_uint,
    .get = &param_get_uint,
};
static const struct kernel_param_ops screen_map_ops = {
    .set = &param_set_uint,
    .get = &param_get_uint
};
static const struct kernel_param_ops pressure_map_ops = {
        .set = &param_set_uint,
        .get = &param_get_uint
};

// create module parms
module_param_cb(orientation, &orientation_ops, &veikk_parms.orientation, 0660);
module_param_cb(da_x, &screen_map_ops, &veikk_parms.da_x, 0660);
module_param_cb(da_y, &screen_map_ops, &veikk_parms.da_y, 0660);
module_param_cb(da_width, &screen_map_ops, &veikk_parms.da_width, 0660);
module_param_cb(da_height, &screen_map_ops, &veikk_parms.da_height, 0660);
module_param_cb(screen_width, &screen_map_ops, &veikk_parms.screen_width, 0660);
module_param_cb(screen_height, &screen_map_ops, &veikk_parms.screen_height,
                                                0660);
module_param_cb(pressure_map, &pressure_map_ops, &veikk_parms.pressure_map,
                                                0660);

MODULE_LICENSE("GPL");
