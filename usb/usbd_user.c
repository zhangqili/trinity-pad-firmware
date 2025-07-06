#include "usbd_user.h"
#include "fezui.h"
#include "fezui_var.h"
#include "command.h"
#include "keyboard.h"
#include "mouse.h"
#include "ch32v30x.h"
#include "usb_descriptor.h"
#include "qmk_midi.h"
#ifdef MIDI_ENABLE
#include "usb_midi.h"
#endif

static const uint8_t *device_descriptor_callback(uint8_t speed)
{
    UNUSED(speed);

    return (const uint8_t *)&DeviceDescriptor;
}

static const uint8_t *config_descriptor_callback(uint8_t speed)
{
    UNUSED(speed);

    return (const uint8_t *)&ConfigurationDescriptor;
}

static const uint8_t *device_quality_descriptor_callback(uint8_t speed)
{
    UNUSED(speed);

#ifdef CONFIG_USB_HS
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    static const uint8_t device_quality_descriptor[] = {0x0a,
        USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
        0x00,
        0x02,
        0x00,
        0x00,
        0x00,
        0x40,
        0x01,
        0x00};
    return device_quality_descriptor; 
#else
    return NULL;
#endif
}

static const uint8_t *other_speed_config_descriptor_callback(uint8_t speed)
{
    UNUSED(speed);

    return NULL;
}

static const char *string_descriptors[] = {
    (const char[]){ 0x09, 0x04 }, /* Langid */
    MANUFACTURER,                    /* Manufacturer */
    PRODUCT,           /* Product */
    SERIAL_NUMBER,                 /* Serial Number */
};

static const char *string_descriptor_callback(uint8_t speed, uint8_t index)
{
    (void)speed;

    if (index >= (sizeof(string_descriptors) / sizeof(char *))) {
        return NULL;
    }
    return string_descriptors[index];
}


const struct usb_descriptor usb_descriptor = {
    .device_descriptor_callback = device_descriptor_callback,
    .config_descriptor_callback = config_descriptor_callback,
    .device_quality_descriptor_callback = device_quality_descriptor_callback,
    .other_speed_descriptor_callback = other_speed_config_descriptor_callback,
    .string_descriptor_callback = string_descriptor_callback,
};

enum
{
    USB_STATE_IDLE = 0,
    USB_STATE_BUSY
};

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX USBBuffer keyboard_buffer;
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX USBBuffer mouse_buffer;
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX USBBuffer raw_buffer;
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX USBBuffer shared_buffer;
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX USBBuffer midi_buffer;
/*!< hid state ! Data can be sent only when state is idle  */

static void usbd_event_handler(uint8_t busid, uint8_t event)
{
    UNUSED(busid);
    switch (event)
    {
    case USBD_EVENT_RESET:
        keyboard_buffer.state = USB_STATE_IDLE;
        raw_buffer.state = USB_STATE_IDLE;
        break;
    case USBD_EVENT_CONNECTED:
        break;
    case USBD_EVENT_DISCONNECTED:
        break;
    case USBD_EVENT_RESUME:
        break;
    case USBD_EVENT_SUSPEND:
        break;
    case USBD_EVENT_CONFIGURED:
        memset(keyboard_buffer.read_buffer, 0, sizeof(keyboard_buffer.read_buffer));
        memset(raw_buffer.read_buffer, 0, sizeof(raw_buffer.read_buffer));
        usbd_ep_start_read(0, RAW_EPOUT_ADDR, raw_buffer.read_buffer, 64);
        break;
    case USBD_EVENT_SET_REMOTE_WAKEUP:
        break;
    case USBD_EVENT_CLR_REMOTE_WAKEUP:
        break;
    default:
        break;
    }
}

void usbd_hid_set_report(uint8_t busid, uint8_t intf, uint8_t report_id, uint8_t report_type, uint8_t *report, uint32_t report_len)
{
    (void)busid;
    (void)intf;
    (void)report_type;
    if (report_len == 2)
    {
        if (report_id == REPORT_ID_KEYBOARD || report_id == REPORT_ID_NKRO) {
            g_keyboard_led_state = report[1];
        }
    }
    else
    {
        g_keyboard_led_state = report[0];
    }
}


static void usbd_hid_keyboard_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    UNUSED(busid);
    UNUSED(ep);
    UNUSED(nbytes);
    keyboard_buffer.state = USB_STATE_IDLE;
}

static void usbd_hid_raw_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    UNUSED(busid);
    UNUSED(ep);
    UNUSED(nbytes);
    raw_buffer.state = USB_STATE_IDLE;
}

static void usbd_hid_raw_out_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    // USB_LOG_RAW("actual out len:%ld\r\n", nbytes);
    UNUSED(busid);
    UNUSED(ep);
    UNUSED(nbytes);
    usbd_ep_start_read(0, RAW_EPOUT_ADDR, raw_buffer.read_buffer, 64);
    command_parse(raw_buffer.read_buffer, sizeof(raw_buffer.read_buffer));
}

static void usbd_hid_shared_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    UNUSED(busid);
    UNUSED(ep);
    UNUSED(nbytes);
    shared_buffer.state = USB_STATE_IDLE;
}

//static void usbd_hid_mouse_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
//{
//    UNUSED(busid);
//    UNUSED(ep);
//    UNUSED(nbytes);
//    mouse_buffer.state = USB_STATE_IDLE;
//}

/* Store example melody as an array of note values */
void usbd_midi_bulk_out(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    (void)busid;
    (void)ep;
    (void)nbytes;
}

void usbd_midi_bulk_in(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    (void)busid;
    (void)ep;
    (void)nbytes;

    midi_buffer.state = USB_STATE_IDLE;
}

static struct usbd_endpoint keyboard_in_ep = {
    .ep_cb = usbd_hid_keyboard_in_callback,
    .ep_addr = KEYBOARD_EPIN_ADDR};

static struct usbd_endpoint raw_in_ep = {
    .ep_cb = usbd_hid_raw_in_callback,
    .ep_addr = RAW_EPIN_ADDR};

static struct usbd_endpoint raw_out_ep = {
    .ep_cb = usbd_hid_raw_out_callback,
    .ep_addr = RAW_EPOUT_ADDR};

//static struct usbd_endpoint mouse_in_ep = {
//    .ep_cb = usbd_hid_mouse_in_callback,
//    .ep_addr = MOUSE_EPIN_ADDR};

static struct usbd_endpoint shared_in_ep = {
    .ep_cb = usbd_hid_shared_in_callback,
    .ep_addr = SHARED_EPIN_ADDR};


struct usbd_endpoint midi_out_ep = {
    .ep_addr = MIDI_EPOUT_ADDR,
    .ep_cb = usbd_midi_bulk_out
};

struct usbd_endpoint midi_in_ep = {
    .ep_addr = MIDI_EPIN_ADDR,
    .ep_cb = usbd_midi_bulk_in
};

static struct usbd_interface intf0;
static struct usbd_interface intf1;
static struct usbd_interface intf2;
static struct usbd_interface intf3;
static struct usbd_interface intf4;
//static struct usbd_interface intf3;

void usb_init(void)
{
    usbd_desc_register(0, &usb_descriptor);
    usbd_add_interface(0, usbd_hid_init_intf(0, &intf0, KeyboardReport, sizeof(KeyboardReport)));
    usbd_add_endpoint(0, &keyboard_in_ep);
    usbd_add_interface(0, usbd_hid_init_intf(0, &intf1, RawReport, sizeof(RawReport)));
    usbd_add_endpoint(0, &raw_in_ep);
    usbd_add_endpoint(0, &raw_out_ep);
    //usbd_add_interface(0, usbd_hid_init_intf(0, &intf2, MouseReport, sizeof(MouseReport)));
    //usbd_add_endpoint(0, &mouse_in_ep);
    usbd_add_interface(0, usbd_hid_init_intf(0, &intf2, SharedReport, sizeof(SharedReport)));
    usbd_add_endpoint(0, &shared_in_ep);
    usbd_add_interface(0, &intf3);
    usbd_add_interface(0, &intf4);
    usbd_add_endpoint(0, &midi_out_ep);
    usbd_add_endpoint(0, &midi_in_ep);

    usbd_initialize(0, USBHS_BASE, usbd_event_handler);
}

int usb_send_shared_ep(uint8_t *buffer, uint8_t size)
{
    if (shared_buffer.state == USB_STATE_BUSY)
    {
        return 1;
    }
    memcpy(shared_buffer.send_buffer, buffer, size);
    int ret = usbd_ep_start_write(0, SHARED_EPIN_ADDR, shared_buffer.send_buffer, SHARED_EPSIZE);
    if (ret < 0)
    {
        return 1;
    }
    shared_buffer.state = USB_STATE_BUSY;
    return 0;
}

int usb_send_keyboard(uint8_t *buffer, uint8_t size)
{
    UNUSED(size);
    if (keyboard_buffer.state == USB_STATE_BUSY)
    {
        return 1;
    }
    memcpy(keyboard_buffer.send_buffer, buffer, KEYBOARD_EPSIZE);
    int ret = usbd_ep_start_write(0, KEYBOARD_EPIN_ADDR, keyboard_buffer.send_buffer, KEYBOARD_EPSIZE);
    if (ret < 0)
    {
        return 1;
    }
    keyboard_buffer.state = USB_STATE_BUSY;
    return 0;
}

int usb_send_raw(uint8_t *buffer, uint8_t size)
{
    UNUSED(size);
    if (raw_buffer.state == USB_STATE_BUSY)
    {
        return 1;
    }
    else
    {
    }
    if (size > 0 && size <= 64)
    {
        memset(raw_buffer.send_buffer, 0, 64);
        memcpy(raw_buffer.send_buffer, buffer, size);
    }
    else
    {
        return 1;
    }
    int ret = usbd_ep_start_write(0, RAW_EPIN_ADDR, raw_buffer.send_buffer, 64);
    if (ret < 0)
    {
        return 1;
    }
    raw_buffer.state = USB_STATE_BUSY;
    return 0;

}

int usb_send_midi(uint8_t *buffer, uint8_t size)
{
    if (midi_buffer.state == USB_STATE_BUSY)
    {
        return 1;
    }
    else
    {
    }
    memcpy(midi_buffer.send_buffer, buffer, size);
    int ret = usbd_ep_start_write(0, MIDI_EPIN_ADDR, midi_buffer.send_buffer, 4);
    if (ret < 0)
    {
        return 1;
    }
    midi_buffer.state = USB_STATE_BUSY;
    return 0;
}

//		0x05, 0x01,   //Usage Page (Generic Desktop)
//		0x09, 0x05,   //Usage (Game Pad)
//		0xA1, 0x01,   //Collection (Application)
//		0x85, 0x03,   //REPORT_ID (3)
//		0x05, 0x01,   //Usage Page (Generic Desktop)
//		0x09, 0x01,   //Usage (Pointer)
//		0xA0,         //Collection (Physical)
//		0x14,         //Logical Minimum (0)
//		0x25, 0xFF,   //Logical Maximum (255)
//		0x75, 0x08,   //Report Size (8)
//		0x95, 0x04,   //Report Count (4)
//		0x09, 0x34,   //Usage (Ry)
//		0x09, 0x33,   //Usage (Rx)
//		0x09, 0x31,   //Usage (Y)
//		0x09, 0x30,   //Usage (X)
//		0x81, 0x02,   //Input (Data, Variable, Absolute)
//		0x14,         //Logical Minimum (0)
//		0x25, 0x01,   //Logical Maximum (1)
//		0x75, 0x01,   //Report Size (1)
//		0x95, 0x02,   //Report Count (2)
//		0x05, 0x09,   //Usage Page (Button)
//		0x19, 0x01,   //Usage Minimum (Button 1)
//		0x29, 0x02,   //Usage Maximum (Button 2)
//		0x81, 0x02,   //Input (Data, Variable, Absolute)
//		0x75, 0x06,   //Report Size (6)
//		0x95, 0x01,   //Report Count (1)
//		0x81, 0x03,   //Input (Constant, Variable, Absolute)
//		0xC0,         //END Collection
//		0xC0,         //END Collection 142
