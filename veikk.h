#ifndef VEIKK_S640_DRIVER_VEIKK_H
#define VEIKK_S640_DRIVER_VEIKK_H

// TODO: remove unneeded imports
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/mod_devicetable.h>
#include <linux/hid.h>
#include <linux/kfifo.h>
#include <linux/leds.h>
#include <linux/usb/input.h>
#include <linux/power_supply.h>
#include <asm/unaligned.h>
#include "llist.h"

// currently arbitrary (imitating wacom), change later
#define VEIKK_PKGLEN_MAX  361

// struct for user interface
struct veikk_vei {
    struct input_dev *pen_input;
    struct input_dev *touch_input;
    struct input_dev *pad_input;
    // TODO: can this be deleted? what is pen_fifo for?
    //  will delete when able to test
    struct kfifo_rec_ptr_2 pen_fifo;
    unsigned char data[VEIKK_PKGLEN_MAX];
};

// struct for hardware interface
struct veikk {
    struct usb_device *usbdev;
    struct usb_interface *intf;
    struct veikk_vei veikk_vei;
    struct hid_device *hdev;
};

struct veikk_parms {
    unsigned int orientation,
                 da_x, da_y, da_width, da_height,
                 screen_width, screen_height,
                 pressure_map;
};
extern struct veikk_parms veikk_parms;

// very simple implementation of linked list for input devices
struct input_dev_llnode {
    struct input_dev_llnode *next;
    struct input_dev *dev;
};
extern struct input_dev_llnode input_dev_llnode_start;

// list for veikks
extern struct llist *veikk_list;

// prototypes
int veikk_allocate_inputs(struct veikk *);
int veikk_register_inputs(struct veikk *);

#endif //VEIKK_S640_DRIVER_VEIKK_H
