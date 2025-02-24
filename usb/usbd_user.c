#include "usbd_user.h"
#include "fezui.h"
#include "fezui_var.h"
#include "command.h"
#include "keyboard.h"
#include "mouse.h"
#include "ch32v30x.h"

#define ENABLE_MOUSE


const uint8_t hid_descriptor[] = {
    USB_DEVICE_DESCRIPTOR_INIT(USB_2_0, 0x00, 0x00, 0x00, USBD_VID, USBD_PID, 0x0002, 0x01),
    USB_CONFIG_DESCRIPTOR_INIT(USB_HID_CONFIG_DESC_SIZ, 0x03, 0x01, USB_CONFIG_BUS_POWERED, USBD_MAX_POWER),
    //Keyboard
    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,                          /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
    0x00,                          /* bInterfaceNumber: Number of Interface */
    0x00,                          /* bAlternateSetting: Alternate setting */
    0x01,                          /* bNumEndpoints */
    0x03,                          /* bInterfaceClass: HID */
    0x01,                          /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x01,                          /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,                    /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID, /* bDescriptorType: HID */
    0x11,                    /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                          /* bCountryCode: Hardware target country */
    0x01,                          /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                          /* bDescriptorType */
    HID_KEYBOARD_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Mouse endpoint ********************/
    /* 27 */
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HID_KEYBOARD_INT_EP,                   /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    HID_KEYBOARD_INT_EP_SIZE,              /* wMaxPacketSize: 4 Byte max */
    0x00,
    HID_KEYBOARD_INT_EP_INTERVAL, /* bInterval: Polling Interval */
    /* 34 */
    //Mouse
    /************** Descriptor of Joystick Mouse interface ****************/
    /* 09 */
    0x09,                          /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
    0x01,                          /* bInterfaceNumber: Number of Interface */
    0x00,                          /* bAlternateSetting: Alternate setting */
    0x01,                          /* bNumEndpoints */
    0x03,                          /* bInterfaceClass: HID */
    0x01,                          /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x02,                          /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Joystick Mouse HID ********************/
    /* 18 */
    0x09,                    /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID, /* bDescriptorType: HID */
    0x11,                    /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                       /* bCountryCode: Hardware target country */
    0x01,                       /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                       /* bDescriptorType */
    HID_MOUSE_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Mouse endpoint ********************/
    /* 27 */
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HID_MOUSE_INT_EP,                   /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    HID_MOUSE_INT_EP_SIZE,              /* wMaxPacketSize: 4 Byte max */
    0x00,
    HID_MOUSE_INT_EP_INTERVAL, /* bInterval: Polling Interval */
    /* 34 */

    //HID_RAW
    /************** Descriptor of Custom interface *****************/
    0x09,                          /* bLength: Interface Descriptor size */
    USB_DESCRIPTOR_TYPE_INTERFACE, /* bDescriptorType: Interface descriptor type */
    0x02,                          /* bInterfaceNumber: Number of Interface */
    0x00,                          /* bAlternateSetting: Alternate setting */
    0x02,                          /* bNumEndpoints */
    0x03,                          /* bInterfaceClass: HID */
    0x00,                          /* bInterfaceSubClass : 1=BOOT, 0=no boot */
    0x00,                          /* nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse */
    0,                             /* iInterface: Index of string descriptor */
    /******************** Descriptor of Custom HID ********************/
    0x09,                    /* bLength: HID Descriptor size */
    HID_DESCRIPTOR_TYPE_HID, /* bDescriptorType: HID */
    0x11,                    /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,                        /* bCountryCode: Hardware target country */
    0x01,                        /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,                        /* bDescriptorType */
    HID_RAW_REPORT_DESC_SIZE, /* wItemLength: Total length of Report descriptor */
    0x00,
    /******************** Descriptor of Custom in endpoint ********************/
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HIDRAW_IN_EP,                 /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    WBVAL(HIDRAW_IN_EP_SIZE),        /* wMaxPacketSize: 4 Byte max */
    HIDRAW_IN_INTERVAL,           /* bInterval: Polling Interval */
    /******************** Descriptor of Custom out endpoint ********************/
    0x07,                         /* bLength: Endpoint Descriptor size */
    USB_DESCRIPTOR_TYPE_ENDPOINT, /* bDescriptorType: */
    HIDRAW_OUT_EP,                /* bEndpointAddress: Endpoint Address (IN) */
    0x03,                         /* bmAttributes: Interrupt endpoint */
    WBVAL(HIDRAW_OUT_EP_SIZE),    /* wMaxPacketSize: 4 Byte max */
    HIDRAW_OUT_EP_INTERVAL,       /* bInterval: Polling Interval */
    /* 73 */
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
};

const uint8_t hid_keyboard_report_desc[HID_KEYBOARD_REPORT_DESC_SIZE] = {
    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x06, // USAGE (Keyboard)
    0xa1, 0x01, // COLLECTION (Application)
    0x05, 0x07, // USAGE_PAGE (Keyboard)
    0x19, 0xe0, // USAGE_MINIMUM (Keyboard LeftControl)
    0x29, 0xe7, // USAGE_MAXIMUM (Keyboard Right GUI)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0x01, // LOGICAL_MAXIMUM (1)
    0x75, 0x01, // REPORT_SIZE (1)
    0x95, 0x08, // REPORT_COUNT (8)
    0x81, 0x02, // INPUT (Data,Var,Abs)
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x08, // REPORT_SIZE (8)
    0x81, 0x03, // INPUT (Cnst,Var,Abs)
    0x95, 0x05, // REPORT_COUNT (5)
    0x75, 0x01, // REPORT_SIZE (1)
    0x05, 0x08, // USAGE_PAGE (LEDs)
    0x19, 0x01, // USAGE_MINIMUM (Num Lock)
    0x29, 0x05, // USAGE_MAXIMUM (Kana)
    0x91, 0x02, // OUTPUT (Data,Var,Abs)
    0x95, 0x01, // REPORT_COUNT (1)
    0x75, 0x03, // REPORT_SIZE (3)
    0x91, 0x03, // OUTPUT (Cnst,Var,Abs)
    0x95, 0x06, // REPORT_COUNT (6)
    0x75, 0x08, // REPORT_SIZE (8)
    0x15, 0x00, // LOGICAL_MINIMUM (0)
    0x25, 0xFF, // LOGICAL_MAXIMUM (255)
    0x05, 0x07, // USAGE_PAGE (Keyboard)
    0x19, 0x00, // USAGE_MINIMUM (Reserved (no event indicated))
    0x29, 0xA4, // USAGE_MAXIMUM (Keyboard Application)
    0x81, 0x00, // INPUT (Data,Ary,Abs)
    0xc0        // END_COLLECTION
};

#ifdef ENABLE_MOUSE
/*!< hid mouse report descriptor */
const uint8_t hid_mouse_report_desc[HID_MOUSE_REPORT_DESC_SIZE] = {

    0x05, 0x01, // USAGE_PAGE (Generic Desktop)
    0x09, 0x02, // USAGE (Mouse)
    0xA1, 0x01, // COLLECTION (Application)
    //0x85, 0x01, //   REPORT ID (0x01)
    0x09, 0x01, //   USAGE (Pointer)

    0xA1, 0x00, //   COLLECTION (Physical)
    0x05, 0x09, //     USAGE_PAGE (Button)
    0x19, 0x01, //     USAGE_MINIMUM (Button 1)
    0x29, 0x05, //     USAGE_MAXIMUM (Button 5)

    0x15, 0x00, //     LOGICAL_MINIMUM (0)
    0x25, 0x01, //     LOGICAL_MAXIMUM (1)
    0x95, 0x05, //     REPORT_COUNT (5)
    0x75, 0x01, //     REPORT_SIZE (1)

    0x81, 0x02, //     INPUT (Data,Var,Abs)
    0x95, 0x01, //     REPORT_COUNT (1)
    0x75, 0x03, //     REPORT_SIZE (3)
    0x81, 0x01, //     INPUT (Cnst,Var,Abs)

    0x05, 0x01, //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30, //     USAGE (X)
    0x09, 0x31, //     USAGE (Y)
    0x09, 0x38,

    0x15, 0x81, //     LOGICAL_MINIMUM (-127)
    0x25, 0x7F, //     LOGICAL_MAXIMUM (127)
    0x75, 0x08, //     REPORT_SIZE (8)
    0x95, 0x03, //     REPORT_COUNT (2)

    0x81, 0x06, //     INPUT (Data,Var,Rel)
    0xC0, 0xc0 //   END_COLLECTION
};
#endif
/*!< custom hid report descriptor */
const uint8_t hid_raw_report_desc[HID_RAW_REPORT_DESC_SIZE] = {
    /* USER CODE BEGIN 0 */
    0x06, 0x00, 0xff, /* USAGE_PAGE (Vendor Defined Page 1) */
    0x09, 0x01,       /* USAGE (Vendor Usage 1) */
    0xa1, 0x01,       /* COLLECTION (Application) */
    0x85, 0x02,       /*   REPORT ID (0x02) */
    0x09, 0x01,       /*   USAGE (Vendor Usage 1) */
    0x15, 0x00,       /*   LOGICAL_MINIMUM (0) */
    0x26, 0xff, 0x00, /*   LOGICAL_MAXIMUM (255) */
    0x95, 0x40 - 1,   /*   REPORT_COUNT (63) */
    0x75, 0x08,       /*   REPORT_SIZE (8) */
    0x81, 0x02,       /*   INPUT (Data,Var,Abs) */
    /* <___________________________________________________> */
    0x85, 0x01,       /*   REPORT ID (0x01) */
    0x09, 0x01,       /*   USAGE (Vendor Usage 1) */
    0x15, 0x00,       /*   LOGICAL_MINIMUM (0) */
    0x26, 0xff, 0x00, /*   LOGICAL_MAXIMUM (255) */
    0x95, 0x40 - 1,   /*   REPORT_COUNT (63) */
    0x75, 0x08,       /*   REPORT_SIZE (8) */
    0x91, 0x02,       /*   OUTPUT (Data,Var,Abs) */
    /* USER CODE END 0 */
    0xC0 /*     END_COLLECTION	             */
};


enum {
    HID_STATE_IDLE = 0,
    HID_STATE_BUSY
};

/*!< hid state ! Data can be sent only when state is idle  */
static volatile uint8_t custom_state;
static volatile uint8_t hid_keyboard_state = HID_STATE_IDLE;
static volatile uint8_t hid_mouse_state = HID_STATE_IDLE;

uint32_t g_usb_keyboard_interval;
uint32_t g_usb_mouse_interval;


uint32_t g_usb_report_count;
uint32_t g_usb_report_count1;
uint32_t g_usb_mouse_report_count;
uint32_t g_usb_mouse_report_count1;
uint32_t g_usb_raw_report_count;
uint32_t g_usb_raw_report_count1;

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t read_buffer[HIDRAW_OUT_EP_SIZE];
USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t send_buffer[HIDRAW_IN_EP_SIZE];
void usbd_event_handler(uint8_t busid, uint8_t event)
{
    switch (event) {
        case USBD_EVENT_RESET:
            custom_state=HID_STATE_IDLE;
            hid_keyboard_state=HID_STATE_IDLE;
            hid_mouse_state=HID_STATE_IDLE;
            //fezui_notification_begin(&fezui,&fezui_notification,"USBD_EVENT_RESET",500,0.1);
            break;
        case USBD_EVENT_CONNECTED:
            //fezui_notification_begin(&fezui,&fezui_notification,"USBD_EVENT_CONNECTED",500,0.1);
            break;
        case USBD_EVENT_DISCONNECTED:
            //fezui_notification_begin(&fezui,&fezui_notification,"USBD_EVENT_DISCONNECTED",500,0.1);
            break;
        case USBD_EVENT_RESUME:
            //fezui_notification_begin(&fezui,&fezui_notification,"USBD_EVENT_RESUME",500,0.1);
            break;
        case USBD_EVENT_SUSPEND:
            //fezui_notification_begin(&fezui,&fezui_notification,"USBD_EVENT_SUSPEND",500,0.1);
            break;
        case USBD_EVENT_CONFIGURED:
            /* setup first out ep read transfer */
            usbd_ep_start_read(0, HIDRAW_OUT_EP, read_buffer, HIDRAW_OUT_EP_SIZE);
            //fezui_notification_begin(&fezui,&fezui_notification,"USBD_EVENT_CONFIGURED",500,0.1);
            break;
        case USBD_EVENT_SET_REMOTE_WAKEUP:
            //fezui_notification_begin(&fezui,&fezui_notification,"USBD_EVENT_SET_REMOTE_WAKEUP",500,0.1);
            break;
        case USBD_EVENT_CLR_REMOTE_WAKEUP:
            //fezui_notification_begin(&fezui,&fezui_notification,"USBD_EVENT_CLR_REMOTE_WAKEUP",500,0.1);
            break;
        default:
            break;
    }
}

void usbd_hid_kayboard_int_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    hid_keyboard_state = HID_STATE_IDLE;
    g_usb_report_count++;
    //usbd_ep_start_write(HID_KEYBOARD_INT_EP, g_keyboard_6kro_buffer.buffer, 8);
    //usbd_ep_start_write(HID_MOUSE_INT_EP, (uint8_t*)&g_mouse, 4);
}
void usbd_hid_mouse_int_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    hid_mouse_state = HID_STATE_IDLE;
    //usbd_ep_start_write(HID_MOUSE_INT_EP, (uint8_t*)&g_mouse, 4);
    //usbd_ep_start_write(HID_KEYBOARD_INT_EP, g_keyboard_6kro_buffer.buffer, 8);
}

static void usbd_hid_custom_in_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    //USB_LOG_RAW("actual in len:%ld\r\n", nbytes);
    g_usb_raw_report_count++;
    custom_state = HID_STATE_IDLE;
}

static void usbd_hid_custom_out_callback(uint8_t busid, uint8_t ep, uint32_t nbytes)
{
    //USB_LOG_RAW("actual out len:%ld\r\n", nbytes);
    usbd_ep_start_read(0, HIDRAW_OUT_EP, read_buffer, 64);
    //char out_buf[32];
    //sprintf(out_buf,"%x",read_buffer[1]);
    command_prase(read_buffer+1,sizeof(read_buffer)-1);
    //fezui_notification_begin(&fezui,&fezui_notification,"message received",500,0.1);
    //read_buffer[0] = 0x02; /* IN: report id */
    //usbd_ep_start_write(HIDRAW_IN_EP, read_buffer, nbytes);
}

static struct usbd_endpoint hid_keyboard_in_ep = {
    .ep_cb = usbd_hid_kayboard_int_callback,
    .ep_addr = HID_KEYBOARD_INT_EP
};
#ifdef ENABLE_MOUSE
static struct usbd_endpoint hid_mouse_in_ep = {
    .ep_cb = usbd_hid_mouse_int_callback,
    .ep_addr = HID_MOUSE_INT_EP
};
#endif
static struct usbd_endpoint custom_in_ep = {
    .ep_cb = usbd_hid_custom_in_callback,
    .ep_addr = HIDRAW_IN_EP
};

static struct usbd_endpoint custom_out_ep = {
    .ep_cb = usbd_hid_custom_out_callback,
    .ep_addr = HIDRAW_OUT_EP
};

struct usbd_interface intf0;//Keyboard
struct usbd_interface intf1;//Mouse
struct usbd_interface intf2;//INOUT

void hid_init(void)
{
    usbd_desc_register(0, hid_descriptor);
    usbd_add_interface(0, usbd_hid_init_intf(0, &intf0, hid_keyboard_report_desc, HID_KEYBOARD_REPORT_DESC_SIZE));
    usbd_add_endpoint(0, &hid_keyboard_in_ep);
    usbd_add_interface(0, usbd_hid_init_intf(0, &intf1, hid_mouse_report_desc, HID_MOUSE_REPORT_DESC_SIZE));
    usbd_add_endpoint(0, &hid_mouse_in_ep);
    usbd_add_interface(0, usbd_hid_init_intf(0, &intf2, hid_raw_report_desc, HID_RAW_REPORT_DESC_SIZE));
    usbd_add_endpoint(0, &custom_in_ep);
    usbd_add_endpoint(0, &custom_out_ep);

    usbd_initialize(0, USBHS_BASE, usbd_event_handler); 
}

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t keyboard_write_buffer[64];

int hid_keyboard_send(uint8_t*buffer)
{
    if (hid_keyboard_state == HID_STATE_BUSY) {
        return 1;
    }
    else
    {
        //g_usb_keyboard_interval = 0;
    }
    memcpy(keyboard_write_buffer, buffer, 8);
    int ret = usbd_ep_start_write(0, HID_KEYBOARD_INT_EP, keyboard_write_buffer, 8);
    if (ret < 0) {
        return ret;
    }
    hid_keyboard_state = HID_STATE_BUSY;
    return 0;
}

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t mouse_write_buffer[64];
int hid_mouse_send(uint8_t*buffer)
{
    //static uint8_t hid_mouse_buzy_count = 0;
    if (hid_mouse_state == HID_STATE_BUSY) {
        //g_usb_mouse_report_count++;
        return 1;
    }
    memcpy(mouse_write_buffer, buffer, 4);
    int ret = usbd_ep_start_write(0, HID_MOUSE_INT_EP, mouse_write_buffer, 4);
    if (ret < 0) {
        return 1;
    }
    hid_mouse_state = HID_STATE_BUSY;
    return 0;
}

USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX uint8_t hid_raw_write_buffer[64];
int hid_raw_send(uint8_t*buffer,int size)
{
    if (custom_state == HID_STATE_BUSY) {
        return 1;
    }
    memcpy(hid_raw_write_buffer, buffer, size);
    hid_raw_write_buffer[0] = 0x02;
    int ret = usbd_ep_start_write(0, HIDRAW_IN_EP, hid_raw_write_buffer, 64);
    if (ret < 0) {
        return 1;
    }
    custom_state = HID_STATE_BUSY;
    return 0;
}

struct hid_mouse {
    uint8_t buttons;
    int8_t x;
    int8_t y;
    int8_t wheel;
};
/*!< mouse report */
static USB_NOCACHE_RAM_SECTION USB_MEM_ALIGNX struct hid_mouse mouse_cfg;
/**
  * @brief            hid mouse test
  * @pre              none
  * @param[in]        none
  * @retval           none
  */
void hid_mouse_test(void)
{
    int counter = 0;
    while (counter < 1000) {
        /*!< move mouse pointer */
        mouse_cfg.x += 40;
        mouse_cfg.y += 0;

        memcpy(keyboard_write_buffer, (uint8_t *)&mouse_cfg, 4);
        int ret = usbd_ep_start_write(0, HID_MOUSE_INT_EP, keyboard_write_buffer, 4);
        if (ret < 0) {
            return;
        }
        hid_mouse_state = HID_STATE_BUSY;
        while (hid_mouse_state == HID_STATE_BUSY) {
        }

        counter++;
    }
}