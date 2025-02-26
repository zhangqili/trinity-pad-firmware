#include "usbd_user.h"
#include "fezui.h"
#include "fezui_var.h"
#include "command.h"
#include "keyboard.h"
#include "mouse.h"
#include "ch32v30x.h"
#include "usb_descriptor.h"

typedef struct __USBDescriptor
{
    USB_Descriptor_Device_t device;
    USB_Descriptor_Configuration_t config;
    uint8_t strings[];
} USBDescriptor;

const USBDescriptor usb_descriptor =
{
    .device = DeviceDescriptor,
    .config = ConfigurationDescriptor,
    .strings = {
    ///////////////////////////////////////
    /// string0 descriptor
    ///////////////////////////////////////
    USB_LANGID_INIT(USBD_LANGID_STRING),
    ///////////////////////////////////////
    /// string1 descriptor
    ///////////////////////////////////////
    0x12,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'L', 0x00,                  /* wcChar0 */
    'z', 0x00,                  /* wcChar1 */
    'q', 0x00,                  /* wcChar2 */
    '1', 0x00,                  /* wcChar3 */
    '2', 0x00,                  /* wcChar4 */
    '3', 0x00,                  /* wcChar5 */
    '4', 0x00,                  /* wcChar6 */
    '5', 0x00,                  /* wcChar7 */
    ///////////////////////////////////////
    /// string2 descriptor
    ///////////////////////////////////////
    0x1e,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    'T', 0x00,                  /* wcChar0 */
    'r', 0x00,                  /* wcChar1 */
    'i', 0x00,                  /* wcChar2 */
    'n', 0x00,                  /* wcChar3 */
    'i', 0x00,                  /* wcChar4 */
    't', 0x00,                  /* wcChar5 */
    'y', 0x00,                  /* wcChar6 */
    ' ', 0x00,                  /* wcChar7 */
    'K', 0x00,                  /* wcChar8 */
    'e', 0x00,                  /* wcChar9 */
    'y', 0x00,                  /* wcChar10 */
    'p', 0x00,                  /* wcChar11 */
    'a', 0x00,                  /* wcChar12 */
    'd', 0x00,                  /* wcChar13 */
    ///////////////////////////////////////
    /// string3 descriptor
    ///////////////////////////////////////
    0x16,                       /* bLength */
    USB_DESCRIPTOR_TYPE_STRING, /* bDescriptorType */
    '2', 0x00,                  /* wcChar0 */
    '0', 0x00,                  /* wcChar1 */
    '2', 0x00,                  /* wcChar2 */
    '2', 0x00,                  /* wcChar3 */
    '1', 0x00,                  /* wcChar4 */
    '2', 0x00,                  /* wcChar5 */
    '3', 0x00,                  /* wcChar6 */
    '4', 0x00,                  /* wcChar7 */
    '5', 0x00,                  /* wcChar8 */
    '6', 0x00,                  /* wcChar9 */
#ifdef CONFIG_USB_HS
    ///////////////////////////////////////
    /// device qualifier descriptor
    ///////////////////////////////////////
    0x0a,
    USB_DESCRIPTOR_TYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
#endif
    0x00
    }
};

enum
{
    HID_STATE_IDLE = 0,
    HID_STATE_BUSY
};

typedef struct __HIDBuffer
{
    uint8_t read_buffer[64];
    uint8_t send_buffer[64];
    uint8_t state;
} HIDBuffer;

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX HIDBuffer keyboard_buffer;
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX HIDBuffer mouse_buffer;
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX HIDBuffer raw_buffer;
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX HIDBuffer shared_buffer;
/*!< hid state ! Data can be sent only when state is idle  */

static void usbd_event_handler(uint8_t busid, uint8_t event)
{
    UNUSED(busid);
    switch (event)
    {
    case USBD_EVENT_RESET:
        keyboard_buffer.state = HID_STATE_IDLE;
        raw_buffer.state = HID_STATE_IDLE;
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
        usbd_ep_start_read(0, KEYBOARD_EPOUT_ADDR, keyboard_buffer.read_buffer, 64);
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

static void usbd_hid_keyboard_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    UNUSED(busid);
    UNUSED(ep);
    UNUSED(nbytes);
    keyboard_buffer.state = HID_STATE_IDLE;
}

static void usbd_hid_keyboard_out_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    // USB_LOG_RAW("actual out len:%ld\r\n", nbytes);
    UNUSED(busid);
    UNUSED(ep);
    UNUSED(nbytes);
    usbd_ep_start_read(0, KEYBOARD_EPOUT_ADDR, keyboard_buffer.read_buffer, 64);
    g_keyboard_led_state = keyboard_buffer.read_buffer[0];
    fezui.screensaver_countdown = fezui.screensaver_timeout;
}



static void usbd_hid_raw_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    UNUSED(busid);
    UNUSED(ep);
    UNUSED(nbytes);
    raw_buffer.state = HID_STATE_IDLE;
}

static void usbd_hid_raw_out_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    // USB_LOG_RAW("actual out len:%ld\r\n", nbytes);
    UNUSED(busid);
    UNUSED(ep);
    UNUSED(nbytes);
    usbd_ep_start_read(0, RAW_EPOUT_ADDR, raw_buffer.read_buffer, 64);
    command_prase(raw_buffer.read_buffer, sizeof(raw_buffer.read_buffer));
}

static void usbd_hid_shared_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    UNUSED(busid);
    UNUSED(ep);
    UNUSED(nbytes);
    shared_buffer.state = HID_STATE_IDLE;
}

__unused static void usbd_hid_mouse_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    UNUSED(busid);
    UNUSED(ep);
    UNUSED(nbytes);
    mouse_buffer.state = HID_STATE_IDLE;
}

static struct usbd_endpoint keyboard_in_ep = {
    .ep_cb = usbd_hid_keyboard_in_callback,
    .ep_addr = KEYBOARD_EPIN_ADDR};

static struct usbd_endpoint keyboard_out_ep = {
    .ep_cb = usbd_hid_keyboard_out_callback,
    .ep_addr = KEYBOARD_EPOUT_ADDR};

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

static struct usbd_interface intf0;
static struct usbd_interface intf1;
static struct usbd_interface intf2;
//static struct usbd_interface intf3;

void usb_init(void)
{
    usbd_desc_register(0, (uint8_t*)&usb_descriptor);
    usbd_add_interface(0, usbd_hid_init_intf(0, &intf0, KeyboardReport, sizeof(KeyboardReport)));
    usbd_add_endpoint(0, &keyboard_in_ep);
    usbd_add_endpoint(0, &keyboard_out_ep);
    usbd_add_interface(0, usbd_hid_init_intf(0, &intf1, RawReport, sizeof(RawReport)));
    usbd_add_endpoint(0, &raw_in_ep);
    usbd_add_endpoint(0, &raw_out_ep);
    //usbd_add_interface(0, usbd_hid_init_intf(0, &intf2, MouseReport, sizeof(MouseReport)));
    //usbd_add_endpoint(0, &mouse_in_ep);
    usbd_add_interface(0, usbd_hid_init_intf(0, &intf2, SharedReport, sizeof(SharedReport)));
    usbd_add_endpoint(0, &shared_in_ep);
    usbd_initialize(0, USBHS_BASE, usbd_event_handler);
}

int hid_keyboard_send(uint8_t *buffer, uint8_t size)
{
    if (size <= 8)
    {
        if (keyboard_buffer.state == HID_STATE_BUSY)
        {
            return 1;
        }
        memcpy(keyboard_buffer.send_buffer, buffer, KEYBOARD_EPSIZE);
        int ret = usbd_ep_start_write(0, KEYBOARD_EPIN_ADDR, keyboard_buffer.send_buffer, KEYBOARD_EPSIZE);
        if (ret < 0)
        {
            return 1;
        }
        keyboard_buffer.state = HID_STATE_BUSY;
    }
    else
    {
        if (shared_buffer.state == HID_STATE_BUSY)
        {
            return 1;
        }
        memcpy(shared_buffer.send_buffer + 1, buffer, 31);
        shared_buffer.send_buffer[0] = REPORT_ID_NKRO;
        int ret = usbd_ep_start_write(0, SHARED_EPIN_ADDR, shared_buffer.send_buffer, SHARED_EPSIZE);
        if (ret < 0)
        {
            return 1;
        }
        shared_buffer.state = HID_STATE_BUSY;
    }
    return 0;
}

int hid_mouse_send(uint8_t *buffer)
{
    if (shared_buffer.state == HID_STATE_BUSY)
    {
        return 1;
    }
    else
    {
    }
    memcpy(shared_buffer.send_buffer + 1, buffer, SHARED_EPSIZE);
    shared_buffer.send_buffer[0] = REPORT_ID_MOUSE;
    int ret = usbd_ep_start_write(0, SHARED_EPIN_ADDR, shared_buffer.send_buffer, SHARED_EPSIZE);
    if (ret < 0)
    {
        return 1;
    }
    shared_buffer.state = HID_STATE_BUSY;
    return 0;
}

int hid_raw_send(uint8_t *buffer, int size)
{
    if (raw_buffer.state == HID_STATE_BUSY)
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
    raw_buffer.state = HID_STATE_BUSY;
    return 0;
}

int hid_extra_send(uint8_t report_id, uint16_t usage)
{
    if (shared_buffer.state == HID_STATE_BUSY)
    {
        return 1;
    }
    else
    {
    }
    memcpy(shared_buffer.send_buffer + 1, &usage, sizeof(usage));
    shared_buffer.send_buffer[0] = report_id;
    int ret = usbd_ep_start_write(0, SHARED_EPIN_ADDR, shared_buffer.send_buffer, SHARED_EPSIZE);
    if (ret < 0)
    {
        return 1;
    }
    shared_buffer.state = HID_STATE_BUSY;
    return 0;
}