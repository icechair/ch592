/********************************** (C) COPYRIGHT *******************************
 * File Name          : usb.c
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2020/08/06
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
/* ͷ�ļ����� */
#include <rf.h>
#include "rf_device.h"
#include "CH59x_common.h"
#include "usb.h"
#include "ota.h"
#include "trans.h"
#include "mouse.h"
#include "peripheral.h"
/*********************************************************************
 * GLOBAL TYPEDEFS
 */
// ֧�ֵ����ӿ�����
#define USB_INTERFACE_MAX_NUM       4
// �ӿںŵ����ֵ
#define USB_INTERFACE_MAX_INDEX     3

const uint8_t LangID_StrDescr[ 4 ] =
{
    0x04,
    0x03,                                                                       /* bDescriptorType */
    0x09,
    0x04
};

// ������Ϣ
uint8_t MyManuInfo[64] = {0x0E, 0x03, 'w', 0, 'c', 0, 'h', 0, '.', 0, 'c', 0, 'n', 0};

// ��Ʒ��Ϣ
uint8_t MyProdInfo[64] = {0x0D+11, 0x03,
    '2', 0,
    '.', 0,
    '4', 0,
    'G', 0,
    ' ', 0,
    'D', 0,
    'o', 0,
    'n', 0,
    'g', 0,
    'l', 0,
    'e', 0
};

// ���к�
uint8_t MySerialNum[64] = {0x0D+11, 0x03,
    '2', 0,
    '.', 0,
    '4', 0,
    'G', 0,
    ' ', 0,
    'D', 0,
    'o', 0,
    'n', 0,
    'g', 0,
    'l', 0,
    'e', 0
};


//   -----����������Mouse--------------
const uint8_t HID_ReportDescriptorMouse[]=
{
        0x05,0x01,                  //81    GLOBAL_USAGE_PAGE(Generic Desktop Controls)
        0x09,0x02,                  //83    LOCAL_USAGE(Mouse)
        0xA1,0x01,                  //85    MAIN_COLLECTION(Applicatior)
            0x09,0x01,                  //89    LOCAL_USAGE(Pointer)
            0xA1,0x00,                  //91    MAIN_COLLECTION(Physical)
                0x05,0x09,                  //93    GLOBAL_USAGE_PAGE(Button)
                0x19,0x01,                  //95    LOCAL_USAGE_MINIMUM(1)
                0x29,0x05,                  //97    LOCAL_USAGE_MAXIMUM(5)
                0x15,0x00,                  //99    GLOBAL_LOGICAL_MINIMUM(0)
                0x25,0x01,                  //101   GLOBAL_LOCAL_MAXIMUM(1)
                0x95,0x05,                  //103   GLOBAL_REPORT_COUNT(5)
                0x75,0x01,                  //105   GLOBAL_REPORT_SIZE(1)
                0x81,0x02,                  //107   MAIN_INPUT(data var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )  Input 18.5
                0x95,0x01,                  //109   GLOBAL_REPORT_COUNT(1)
                0x75,0x03,                  //111   GLOBAL_REPORT_SIZE(3)
                0x81,0x01,                  //113   MAIN_INPUT(const array absolute NoWrap linear PreferredState NoNullPosition NonVolatile )   Input 19.0
                0x05,0x01,                  //115   GLOBAL_USAGE_PAGE(Generic Desktop Controls)
                0x09,0x30,                  //117   LOCAL_USAGE(X)
                0x09,0x31,                  //119   LOCAL_USAGE(Y)
    //            0x16, 0x00, 0x80,  //     Logical Minimum (-32768)
    //            0x26, 0xFF, 0x7F,  //     Logical Maximum (32767)
    //            0x75,0x10,                  //127   GLOBAL_REPORT_SIZE(16)
                0x15, 0x80,  //     Logical Minimum (-128)
                0x25, 0x7F,  //     Logical Maximum (128)
                0x75,0x08,                  //127   GLOBAL_REPORT_SIZE(16)
                0x95,0x02,                  //129   GLOBAL_REPORT_COUNT(2)
                0x81, 0x06,        //     Input (Data,Var,Rel,No Wrap,Linear,Preferred State,No Null Position)
                0x09,0x38,                  //121   LOCAL_USAGE(Wheel)
                0x15,0x81,                  //123   GLOBAL_LOGICAL_MINIMUM(-127)
                0x25,0x7F,                  //125   GLOBAL_LOCAL_MAXIMUM(127)
                0x75,0x08,                  //127   GLOBAL_REPORT_SIZE(8)
                0x95,0x01,                  //129   GLOBAL_REPORT_COUNT(1)
                0x81,0x06,                  //131   MAIN_INPUT(data var relative NoWrap linear PreferredState NoNullPosition NonVolatile )  Input 22.0
            0xC0,                       //133   MAIN_COLLECTION_END
        0xC0,

};

//   -----����������Other--------------
const uint8_t HID_ReportDescriptorOther[]=
{
    //���̱���
//      0x05, 0x01,     // Usage Pg (Generic Desktop)
//      0x09, 0x06,     // Usage (Keyboard)
//      0xA1, 0x01,     // Collection: (Application)
//      0x85, 0x01,     //   ReportID(1)
//                      //
//      0x05, 0x07,     // Usage Pg (Key Codes)
//      0x19, 0xE0,     // Usage Min (224)
//      0x29, 0xE7,     // Usage Max (231)
//      0x15, 0x00,     // Log Min (0)
//      0x25, 0x01,     // Log Max (1)
//                      //
//                      // Modifier byte
//      0x75, 0x01,     // Report Size (1)
//      0x95, 0x08,     // Report Count (8)
//      0x81, 0x02,     // Input: (Data, Variable, Absolute)
//                      //
//                      // Reserved byte
//      0x95, 0x01,     // Report Count (1)
//      0x75, 0x08,     // Report Size (8)
//      0x81, 0x01,     // Input: (Constant)
//                      //
//                      // LED report
//      0x95, 0x05,     // Report Count (5)
//      0x75, 0x01,     // Report Size (1)
//      0x05, 0x08,     // Usage Pg (LEDs)
//      0x19, 0x01,     // Usage Min (1)
//      0x29, 0x05,     // Usage Max (5)
//      0x91, 0x02,     // Output: (Data, Variable, Absolute)
//                      //
//                      // LED report padding
//      0x95, 0x01,     // Report Count (1)
//      0x75, 0x03,     // Report Size (3)
//      0x91, 0x01,     // Output: (Constant)
//                      //
//                      // Key arrays (6 bytes)
//      0x95, 0x06,     // Report Count (6)
//      0x75, 0x08,     // Report Size (8)
//      0x15, 0x00,     // Log Min (0)
//      0x25, 0x65,     // Log Max (101)
//      0x05, 0x07,     // Usage Pg (Key Codes)
//      0x19, 0x00,     // Usage Min (0)
//      0x29, 0x65,     // Usage Max (101)
//      0x81, 0x00,     // Input: (Data, Array)
//                      //
//      0xC0 ,           // End Collection

        0x05, 0x01,        // Usage Page (Generic Desktop Ctrls)
        0x09, 0x06,        // Usage (Keyboard)
        0xA1, 0x01,        // Collection (Application)
        0x85, 0x01,        //   Report ID (1)
        0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
        0x19, 0xE0,        //   Usage Minimum (0xE0)
        0x29, 0xE7,        //   Usage Maximum (0xE7)
        0x15, 0x00,        //   Logical Minimum (0)
        0x25, 0x01,        //   Logical Maximum (1)
        0x75, 0x01,        //   Report Size (1)
        0x95, 0x08,        //   Report Count (8)
        0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
        0x95, 0x01,        //   Report Count (1)
        0x75, 0x08,        //   Report Size (8)
        0x81, 0x01,        //   Input (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
        0x95, 0x05,        //   Report Count (5)
        0x75, 0x01,        //   Report Size (1)
        0x05, 0x08,        //   Usage Page (LEDs)
        0x19, 0x01,        //   Usage Minimum (Num Lock)
        0x29, 0x05,        //   Usage Maximum (Kana)
        0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
        0x95, 0x01,        //   Report Count (1)
        0x75, 0x03,        //   Report Size (3)
        0x91, 0x01,        //   Output (Const,Array,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
        0x95, 0x06,        //   Report Count (6)
        0x75, 0x08,        //   Report Size (8)
        0x15, 0x00,        //   Logical Minimum (0)
        0x26, 0xFF, 0x00,  //   Logical Maximum (255)
        0x05, 0x07,        //   Usage Page (Kbrd/Keypad)
        0x19, 0x00,        //   Usage Minimum (0x00)
        0x29, 0xFF,        //   Usage Maximum (0xFF)
        0x81, 0x00,        //   Input (Data,Array,Abs,No Wrap,Linear,Preferred State,No Null Position)
        0x05, 0x0C,        //   Usage Page (Consumer)
        0x75, 0x01,        //   Report Size (1)
        0x95, 0x01,        //   Report Count (1)
        0x09, 0xB8,        //   Usage (Eject)
        0x15, 0x00,        //   Logical Minimum (0)
        0x25, 0x01,        //   Logical Maximum (1)
        0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
        0x05, 0xFF,        //   Usage Page (Reserved 0xFF)
        0x09, 0x03,        //   Usage (0x03)
        0x75, 0x07,        //   Report Size (7)
        0x95, 0x01,        //   Report Count (1)
        0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
        0xC0,              // End Collection

        // 90 bytes


    //ȫ���̣�
    0x05,0x01,                  //0     GLOBAL_USAGE_PAGE(Generic Desktop Controls)
    0x09,0x06,                  //2     LOCAL_USAGE(Keyboard)
    0xA1,0x01,                  //4     MAIN_COLLECTION(Applicatior)
    0x85,0x02,                  //6     GLOBAL_REPORT_ID(2)
    0x05,0x07,                  //8     GLOBAL_USAGE_PAGE(Keyboard/Keypad)
    0x19,0x04,                  //10    LOCAL_USAGE_MINIMUM(4)
    0x29,0x70,                  //12    LOCAL_USAGE_MAXIMUM(112)
    0x15,0x00,                  //14    GLOBAL_LOGICAL_MINIMUM(0)
    0x25,0x01,                  //16    GLOBAL_LOCAL_MAXIMUM(1)
    0x75,0x01,                  //18    GLOBAL_REPORT_SIZE(1)
    0x95,0x78,                  //20    GLOBAL_REPORT_COUNT(120)
    0x81,0x02,                  //22    MAIN_INPUT(data var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )  Input 15.0
    0xC0,                       //24    MAIN_COLLECTION_END


    //��ý����ƣ�
    0x05,0x0C,                  //0     GLOBAL_USAGE_PAGE(Consumer)
    0x09,0x01,                  //2     LOCAL_USAGE(    Consumer Control    )
    0xA1,0x01,                  //4     MAIN_COLLECTION(Applicatior)
    0x85,0x03,                  //6     GLOBAL_REPORT_ID(3)
    0x15,0x00,                  //8     GLOBAL_LOGICAL_MINIMUM(0)
    0x26,0xFF,0x1F,             //10    GLOBAL_LOCAL_MAXIMUM(8191/8191)
    0x19,0x00,                  //13    LOCAL_USAGE_MINIMUM(0)
    0x2A,0xFF,0x1F,             //15    LOCAL_USAGE_MAXIMUM(8191)
    0x75,0x10,                  //18    GLOBAL_REPORT_SIZE(16)
    0x95,0x01,                  //20    GLOBAL_REPORT_COUNT(1)
    0x81,0x00,                  //22    MAIN_INPUT(data array absolute NoWrap linear PreferredState NoNullPosition NonVolatile )    Input 2.0
    0xC0,                       //24    MAIN_COLLECTION_END


    //ϵͳ���ƣ�
    0x05,0x01,                  //0     GLOBAL_USAGE_PAGE(Generic Desktop Controls)
    0x09,0x80,                  //2     LOCAL_USAGE()
    0xA1,0x01,                  //4     MAIN_COLLECTION(Applicatior)
    0x85,0x04,                  //6     GLOBAL_REPORT_ID(4)
    0x05,0x01,                  //8     GLOBAL_USAGE_PAGE(Generic Desktop Controls)
    0x19,0x81,                  //10    LOCAL_USAGE_MINIMUM(129)
    0x29,0x83,                  //12    LOCAL_USAGE_MAXIMUM(131)
    0x15,0x00,                  //14    GLOBAL_LOGICAL_MINIMUM(0)
    0x25,0x01,                  //16    GLOBAL_LOCAL_MAXIMUM(1)
    0x95,0x03,                  //18    GLOBAL_REPORT_COUNT(3)
    0x75,0x01,                  //20    GLOBAL_REPORT_SIZE(1)
    0x81,0x02,                  //22    MAIN_INPUT(data var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )  Input 0.3
    0x95,0x01,                  //24    GLOBAL_REPORT_COUNT(1)
    0x75,0x05,                  //26    GLOBAL_REPORT_SIZE(5)
    0x81,0x01,                  //28    MAIN_INPUT(const array absolute NoWrap linear PreferredState NoNullPosition NonVolatile )   Input 1.0
    0xC0,                       //30    MAIN_COLLECTION_END
};

//   -----����������Manufacturer--------------
const uint8_t HID_ReportDescriptorManufacturer[]=
{
    0x06,0x13,0xFF,             //0     GLOBAL_USAGE_PAGE(Reserved or Other)
    0x09,0x01,                  //3     LOCAL_USAGE()
    0xA1,0x01,                  //5     MAIN_COLLECTION(Applicatior)
    0x15,0x00,                  //7     GLOBAL_LOGICAL_MINIMUM(0)
    0x26,0xFF,0x00,             //9     GLOBAL_LOCAL_MAXIMUM(255/255)
    0x75,0x08,                  //12    GLOBAL_REPORT_SIZE(8)
    0x95,0x40,                  //14    GLOBAL_REPORT_COUNT(64)
    0x09,0x02,                  //16    LOCAL_USAGE()
    0x81,0x02,                  //18    MAIN_INPUT(data var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )  Input 64.0
    0x09,0x03,                  //20    LOCAL_USAGE()
    0x91,0x02,                  //22    MAIN_OUTPUT(data var absolute NoWrap linear PreferredState NoNullPosition NonVolatile ) Output 64.0
    0x0A,0x00,0xFF,             //24    LOCAL_USAGE()
    0xB1,0x02,                  //27    MAIN_FEATURE(data var absolute NoWrap linear PreferredState NoNullPosition NonVolatile )    Feature 64.0
    0xC0,                       //29    MAIN_COLLECTION_END
};

//   -----����������IAP--------------
const uint8_t HID_ReportDescriptorIAP[]=
{

    0x06, 0x60, 0xFF,  // Usage Page (Vendor Defined 0xFF60)
    0x09, 0x61,        // Usage (0x61)
    0xA1, 0x01,        // Collection (Application)
    0x09, 0x62,        //   Usage (0x62)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x95, 0x40,        //   Report Count (63)
    0x75, 0x08,        //   Report Size (8)
    0x81, 0x02,        //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
    0x09, 0x63,        //   Usage (0x63)
    0x15, 0x00,        //   Logical Minimum (0)
    0x26, 0xFF, 0x00,  //   Logical Maximum (255)
    0x95, 0x40,        //   Report Count (63)
    0x75, 0x08,        //   Report Size (8)
    0x91, 0x02,        //   Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    0xC0,              // End Collection
};

const uint8_t HID_ReportDescSizeMouse = sizeof(HID_ReportDescriptorMouse);
const uint8_t HID_ReportDescSizeOther = sizeof(HID_ReportDescriptorOther);
const uint8_t HID_ReportDescSizeManufacturer = sizeof(HID_ReportDescriptorManufacturer);
const uint8_t HID_ReportDescSizeIAP = sizeof(HID_ReportDescriptorIAP);

// �豸������
uint8_t MyDevDescr[] = {
    0x12,   /* bLength */
    0x01,   /* bDescriptorType USB_DEVICE_DESCRIPTOR_TYPE*/
    USB_WBVAL(0x0110),  /* bcdUSB */
    0x00,   /* bDeviceClass */
    0x00,   /* bDeviceSubClass */
    0x00,   /* bDeviceProtocol */
    DevEP0SIZE, /* bMaxPacketSize0 */
    USB_WBVAL(USB_VID), /* idVendor */
    USB_WBVAL(USB_PID), /* idProduct */
    USB_WBVAL(USB_REV), /* bcdDevice */
    0x01,   /* iManufacturer USB_DEVICE_STRING_RESERVED*/
    0x02,   /* iProduct USB_DEVICE_STRING_RESERVED*/
    0x03,   /* iSerialNumber USB_DEVICE_STRING_RESERVED*/
    0x01    /* bNumConfigurations: one possible configuration*/
};
// ����������
const uint8_t MyCfgDescr[] = {
    0x09,   /* bLength */
    0x02,   /* bDescriptorType USB_CONFIGURATION_DESCRIPTOR_TYPE*/
    USB_WBVAL(9+9+9+7+9+9+7+9+7+9+9+9+9+7+7),   /* wTotalLength */
//    USB_WBVAL(9+9+9+7+9+9+7+9+9+7+7+9+9+9+9+7+7),   /* wTotalLength */
    USB_INTERFACE_MAX_NUM,   /* bNumInterfaces */
    0x01,   /* bConfigurationValue */
    0x00,   /* iConfiguration */
    0xA0,
    0x23,    /* bMaxPower */
    //����������

//   -----Interface ������ Mouse --------------
    0x09,   /* bLength */
    0x04,   /* bDescriptorType USB_INTERFACE_DESCRIPTOR_TYPE*/
    0x00,   /* bInterfaceNumber */
    0x00,   /* bAlternateSetting */
    0x01,   /* bNumEndpoints */
    0x03,   /* bInterfaceClass */
    0x01,   /* bInterfaceSubClass */
    0x02,   /* HID Protocol Codes HID_PROTOCOL_NONE*/
    0x00,   /* iInterface */
    //�ӿ�������

    0x09,   /* bLength */
    0x21,   /* bDescriptorType HID_HID_DESCRIPTOR_TYPE*/
    USB_WBVAL(0x0100),/* bcdHID */
    0x00,   /* bCountryCode */
    0x01,   /* bNumDescriptors */
    0x22,   /* bDescriptorType HID_REPORT_DESCRIPTOR_TYPE*/
    USB_WBVAL(HID_ReportDescSizeMouse),/* wDescriptorLength */
    //HID��������

    0x07,   /* bLength */
    0x05,   /* bDescriptorType USB_ENDPOINT_DESCRIPTOR_TYPE*/
    0x81,   /* bEndpointAddress *//* USB_ENDPOINT_IN USB_EP1 */
    0x03,   /* bmAttributes */
    USB_WBVAL(DevEP2SIZE),/* wMaxPacketSize */
    0x01,   /* bInterval *//* 1ms */
    //�˵�2������

//   -----Interface ������ Other --------------
    0x09,   /* bLength */
    0x04,   /* bDescriptorType USB_INTERFACE_DESCRIPTOR_TYPE*/
    0x01,   /* bInterfaceNumber */
    0x00,   /* bAlternateSetting */
    0x01,   /* bNumEndpoints */
    0x03,   /* bInterfaceClass */
    0x01,   /* bInterfaceSubClass */
    0x01,   /* HID Protocol Codes HID_PROTOCOL_NONE*/
    0x00,   /* iInterface */
    //�ӿ�������

    0x09,   /* bLength */
    0x21,   /* bDescriptorType HID_HID_DESCRIPTOR_TYPE*/
    USB_WBVAL(0x0100),/* bcdHID */
    0x00,   /* bCountryCode */
    0x01,   /* bNumDescriptors */
    0x22,   /* bDescriptorType HID_REPORT_DESCRIPTOR_TYPE*/
    USB_WBVAL(HID_ReportDescSizeOther),/* wDescriptorLength */
    //HID��������

    0x07,   /* bLength */
    0x05,   /* bDescriptorType USB_ENDPOINT_DESCRIPTOR_TYPE*/
    0x82,   /* bEndpointAddress *//* USB_ENDPOINT_IN USB_EP2 */
    0x03,   /* bmAttributes */
    USB_WBVAL(DevEP2SIZE),/* wMaxPacketSize */
    0x01,   /* bInterval *//* 1ms */

//   -----Interface ������ Other --------------
    0x09,   /* bLength */
    0x04,   /* bDescriptorType USB_INTERFACE_DESCRIPTOR_TYPE*/
    0x02,   /* bInterfaceNumber */
    0x00,   /* bAlternateSetting */
    0x02,   /* bNumEndpoints */
    0x03,   /* bInterfaceClass */
    0x00,   /* bInterfaceSubClass */
    0x00,   /* HID Protocol Codes HID_PROTOCOL_NONE*/
    0x00,   /* iInterface */
    //�ӿ�������

    0x09,   /* bLength */
    0x21,   /* bDescriptorType HID_HID_DESCRIPTOR_TYPE*/
    USB_WBVAL(0x0100),/* bcdHID */
    0x00,   /* bCountryCode */
    0x01,   /* bNumDescriptors */
    0x22,   /* bDescriptorType HID_REPORT_DESCRIPTOR_TYPE*/
    USB_WBVAL(HID_ReportDescSizeManufacturer),/* wDescriptorLength */
    //HID��������

    0x07,   /* bLength */
    0x05,   /* bDescriptorType USB_ENDPOINT_DESCRIPTOR_TYPE*/
    0x83,   /* bEndpointAddress *//* USB_ENDPOINT_IN USB_EP3 */
    0x03,   /* bmAttributes */
    USB_WBVAL(DevEP3SIZE),/* wMaxPacketSize */
    0x01,   /* bInterval *//* 1ms */

    0x07,   /* bLength */
    0x05,   /* bDescriptorType USB_ENDPOINT_DESCRIPTOR_TYPE*/
    0x03,   /* bEndpointAddress *//* USB_ENDPOINT_OUT USB_EP3 */
    0x03,   /* bmAttributes */
    USB_WBVAL(DevEP3SIZE),/* wMaxPacketSize */
    0x01,   /* bInterval *//* 1ms */
    //�˵�5������

////   -----Interface ������ Other --------------
//    0x09,   /* bLength */
//    0x04,   /* bDescriptorType USB_INTERFACE_DESCRIPTOR_TYPE*/
//    0x03,   /* bInterfaceNumber */
//    0x00,   /* bAlternateSetting */
//    0x00,   /* bNumEndpoints */
//    0x03,   /* bInterfaceClass */
//    0x00,   /* bInterfaceSubClass */
//    0x00,   /* HID Protocol Codes HID_PROTOCOL_NONE*/
//    0x00,   /* iInterface */
//
//    //�ӿ�������
//    0x09,   /* bLength */
//    0x21,   /* bDescriptorType HID_HID_DESCRIPTOR_TYPE*/
//    USB_WBVAL(0x0100),/* bcdHID */
//    0x00,   /* bCountryCode */
//    0x00,   /* bNumDescriptors */
//    0x22,   /* bDescriptorType HID_REPORT_DESCRIPTOR_TYPE*/
//    USB_WBVAL(0),/* wDescriptorLength */
    //HID��������

//   -----Interface ������ Other --------------
    0x09,   /* bLength */
    0x04,   /* bDescriptorType USB_INTERFACE_DESCRIPTOR_TYPE*/
    0x03,   /* bInterfaceNumber */
    0x00,   /* bAlternateSetting */
    0x02,   /* bNumEndpoints */
    0x03,   /* bInterfaceClass */
    0x00,   /* bInterfaceSubClass */
    0x00,   /* HID Protocol Codes HID_PROTOCOL_NONE*/
    0x00,   /* iInterface */
    //�ӿ�������

    0x09,   /* bLength */
    0x21,   /* bDescriptorType HID_HID_DESCRIPTOR_TYPE*/
    USB_WBVAL(0x0100),/* bcdHID */
    0x00,   /* bCountryCode */
    0x01,   /* bNumDescriptors */
    0x22,   /* bDescriptorType HID_REPORT_DESCRIPTOR_TYPE*/
    USB_WBVAL(HID_ReportDescSizeIAP),/* wDescriptorLength */
    //HID��������

    0x07,   /* bLength */
    0x05,   /* bDescriptorType USB_ENDPOINT_DESCRIPTOR_TYPE*/
    0x86,   /* bEndpointAddress *//* USB_ENDPOINT_IN USB_EP6 */
    0x03,   /* bmAttributes */
    USB_WBVAL(DevEP6SIZE),/* wMaxPacketSize */
    0x01,   /* bInterval *//* 1ms */

    0x07,   /* bLength */
    0x05,   /* bDescriptorType USB_ENDPOINT_DESCRIPTOR_TYPE*/
    0x06,   /* bEndpointAddress *//* USB_ENDPOINT_OUT USB_EP6 */
    0x03,   /* bmAttributes */
    USB_WBVAL(DevEP6SIZE),/* wMaxPacketSize */
    0x01,   /* bInterval *//* 1ms */
    //�˵�6������

};

__attribute__((aligned(4)))  uint8_t EP0_Databuf[8]; //ep0(64)+ep4_out(64)+ep4_in(64)
__attribute__((aligned(4)))  uint8_t EP1_Databuf[64 + 64];    //ep1_out(64)+ep1_in(64)
__attribute__((aligned(4)))  uint8_t EP2_Databuf[64 + 64];    //ep2_out(64)+ep2_in(64)
__attribute__((aligned(4)))  uint8_t EP3_Databuf[64 + 64];    //ep3_out(64)+ep3_in(64)
__attribute__((aligned(4)))  uint8_t EP6_Databuf[64 + 64];    //ep5_out(64)+ep5_in(64)

uint8_t        DevConfig= 0;
uint8_t        SetupReqCode;
uint16_t       SetupReqLen;
const uint8_t *pDescr;
uint8_t        Report_Value[USB_INTERFACE_MAX_INDEX+1] = {0x00};
uint8_t        Idle_Value[USB_INTERFACE_MAX_INDEX+1] = {0x00};
volatile uint8_t        USB_SleepStatus = HOST_WAKEUP_ENABLE; /* USB˯��״̬ */
volatile uint8_t    USB_READY_FLAG = 1;
USB_receive_cb_t    USB_receive_cb;
uint8_t         led_val = 0;
uint8_t         USB_receive_buf[64+2];
volatile uint8_t intflag = 0;   //intflag���ڴ�ű�־�Ĵ���ֵ
uint8_t         usb_enum_flag = 0;
uint8_t         usb_enum_success_flag=0;

void USB_receive_process(uint8_t *pData,uint8_t len)
{
    if(pData[0] == USB_DATA_IAP)
    {
        OTA_USB_IAPWriteData(&pData[2], len-2);
    }
    else if(pData[0] == USB_DATA_LED)
    {
        peripheral_pilot_led_receive(pData[2]);
    }
    else if(pData[0] == USB_DATA_MANUFACTURER)
    {
        PRINT("MANUFACTURER %x\n",pData[2]);
    }
}

/*********************************************************************
 * @fn      USB_Init
 *
 * @brief   USB_Init
 *
 * @return  none
 */
void USB_Init( void )
{
    PFIC_DisableIRQ(USB_IRQn);
    pEP0_RAM_Addr = EP0_Databuf;
    pEP1_RAM_Addr = EP1_Databuf;
    pEP2_RAM_Addr = EP2_Databuf;
    pEP3_RAM_Addr = EP3_Databuf;
    pEP6_RAM_Addr = EP6_Databuf;
    USB_DeviceInit();
    USB_receive_cb_register(USB_receive_process);
    PFIC_EnableIRQ( USB_IRQn );
}

/*********************************************************************
 * @fn      USB_Uinit
 *
 * @brief   USB_Uinit
 *
 * @return  none
 */
void USB_Uinit( void )
{
    R16_PIN_ANALOG_IE &= ~(RB_PIN_USB_IE | RB_PIN_USB_DP_PU);
    R8_UDEV_CTRL = 0;
    R8_USB_CTRL = 0;
    R8_USB_INT_EN = 0;
    GPIOB_ModeCfg( GPIO_Pin_10 | GPIO_Pin_11, GPIO_ModeIN_PD);
    PFIC_DisableIRQ(USB_IRQn);
}

/*********************************************************************
 * @fn      USB_cfg_vid_pid
 *
 * @brief   USB_cfg_vid_pid
 *
 * @return  none
 */
void USB_cfg_vid_pid( uint16_t VID, uint16_t PID )
{
    MyDevDescr[8] = VID& 0xFF;
    MyDevDescr[9] = (VID >> 8) & 0xFF;
    MyDevDescr[10] = PID& 0xFF;
    MyDevDescr[11] = (PID >> 8) & 0xFF;
}

/*********************************************************************
 * @fn      USB_cfg_manu_info
 *
 * @brief   USB_cfg_manu_info
 *
 * @return  none
 */
void USB_cfg_manu_info( uint8_t *pData, uint8_t len )
{
    uint8_t i;
    MyManuInfo[0] = len*2+2;
    MyManuInfo[1] = 0x03;
    for(i=0; i<len; i++)
    {
        MyManuInfo[2+i*2] = pData[i];
        MyManuInfo[3+i*2] = 0x00;
    }
}

/*********************************************************************
 * @fn      USB_cfg_prod_info
 *
 * @brief   USB_cfg_prod_info
 *
 * @return  none
 */
void USB_cfg_prod_info( uint8_t *pData, uint8_t len )
{
    uint8_t i;
    MyProdInfo[0] = len*2+2;
    MyProdInfo[1] = 0x03;
    for(i=0; i<len; i++)
    {
        MyProdInfo[2+i*2] = pData[i];
        MyProdInfo[3+i*2] = 0x00;
    }
}

/*********************************************************************
 * @fn      USB_cfg_serial_num
 *
 * @brief   USB_cfg_serial_num
 *
 * @return  none
 */
void USB_cfg_serial_num( uint8_t *pData, uint8_t len )
{
    uint8_t i;
    MySerialNum[0] = len*2+2;
    MySerialNum[1] = 0x03;
    for(i=0; i<len; i++)
    {
        MySerialNum[2+i*2] = pData[i];
        MySerialNum[3+i*2] = 0x00;
    }
}
/*********************************************************************
 * @fn      USB_Wake_up
 *
 * @brief   USB_Wake_up
 *
 * @return  none
 */
void USB_Wake_up( void )
{
    R16_PIN_ANALOG_IE &= ~RB_PIN_USB_DP_PU;         // USB��������
    R8_UDEV_CTRL |= RB_UD_LOW_SPEED;
    mDelaymS(8);
    R8_UDEV_CTRL &= ~RB_UD_LOW_SPEED;
    R16_PIN_ANALOG_IE |= RB_PIN_USB_DP_PU;         // USB��������
}

/*********************************************************************
 * @fn      USB_receive_cb_register
 *
 * @brief   USB_receive_cb_register
 *
 * @return  none
 */
void USB_receive_cb_register(USB_receive_cb_t cback)
{
    USB_receive_cb = cback;
}

/*********************************************************************
 * @fn      USB_mouse_report
 *
 * @brief   �ϱ�mouse����
 *
 * @return  0���ɹ�
 *          1������
 */
uint8_t USB_mouse_report(uint8_t *pData,uint8_t len)
{
    if(!USB_READY_FLAG)
    {
        return 0xFF;
    }
    USB_READY_FLAG = 0;
    tmos_memcpy(pEP1_IN_DataBuf, pData, len);
    DevEP1_IN_Deal(len);
    return 0;
}

/*********************************************************************
 * @fn      USB_class_keyboard_report
 *
 * @brief   �ϱ�class_keyboard����
 *
 * @return  0���ɹ�
 *          1������
 */
uint8_t USB_class_keyboard_report(uint8_t *pData,uint8_t len)
{
    if(!USB_READY_FLAG)
    {
        return 0xFF;
    }
    USB_READY_FLAG = 0;
    pEP2_IN_DataBuf[0] = REPORT_ID_KEYBOARD;
    tmos_memcpy(&pEP2_IN_DataBuf[1], pData, len);
    DevEP2_IN_Deal(len+1);
    return 0;
}

/*********************************************************************
 * @fn      USB_all_keyboard_report
 *
 * @brief   �ϱ�all_keyboard����
 *
 * @return  0���ɹ�
 *          1������
 */
uint8_t USB_all_keyboard_report(uint8_t *pData,uint8_t len)
{
    if(!USB_READY_FLAG)
    {
        return 0xFF;
    }
    USB_READY_FLAG = 0;
    pEP2_IN_DataBuf[0] = REPORT_ID_ALL_KEYBOARD;
    tmos_memcpy(&pEP2_IN_DataBuf[1], pData, len);
    DevEP2_IN_Deal(len+1);
    return 0;
}
/*********************************************************************
 * @fn      USB_manufacturer_report
 *
 * @brief   �ϱ�manufacturer���ݣ��û�ע�������ϲ�Э��
 *
 * @return  0���ɹ�
 *          1������
 */
uint8_t USB_manufacturer_report(uint8_t *pData,uint8_t len)
{
    USB_READY_FLAG = 0;
    tmos_memcpy(pEP3_IN_DataBuf, pData, len);
    DevEP3_IN_Deal(len);
    return 0;
}

/*********************************************************************
 * @fn      USB_consumer_report
 *
 * @brief   �ϱ�consumer����
 *
 * @return  0���ɹ�
 *          1������
 */
uint8_t USB_consumer_report(uint8_t *pData,uint8_t len)
{
    if(!USB_READY_FLAG)
    {
        return 0xFF;
    }
    USB_READY_FLAG = 0;
    pEP2_IN_DataBuf[0] = REPORT_ID_CONSUMER;
    tmos_memcpy(&pEP2_IN_DataBuf[1], pData, len);
    DevEP2_IN_Deal(len+1);
    return 0;
}

/*********************************************************************
 * @fn      USB_sys_ctl_report
 *
 * @brief   �ϱ�sys_ctl����
 *
 * @return  0���ɹ�
 *          1������
 */
uint8_t USB_sys_ctl_report(uint8_t data)
{
    if(!USB_READY_FLAG)
    {
        return 0xFF;
    }
    USB_READY_FLAG = 0;
    pEP2_IN_DataBuf[0] = REPORT_ID_SYS_CTL;
    pEP2_IN_DataBuf[1] = data;
    DevEP2_IN_Deal(2);
    return 0;
}

/*********************************************************************
 * @fn      USB_IAP_report
 *
 * @brief   �ϱ�IAP����
 *
 * @return  0���ɹ�
 *          1������
 */
uint8_t USB_IAP_report(uint8_t *pData,uint8_t len)
{
    USB_READY_FLAG = 0;
    tmos_memcpy(pEP6_IN_DataBuf, pData, len);
    DevEP6_IN_Deal(0x40);
    return 0;
}

/*********************************************************************
 * @fn      USB_IRQ_trans_process
 *
 * @brief   USB�жϺ���
 *
 * @return  none
 */
void USB_IRQ_trans_process( void )
{
    uint8_t len, chtype;        //len���ڿ���������chtype���ڴ�����ݴ��䷽����������͡����յĶ������Ϣ
    uint8_t  errflag = 0;   //errflag���ڱ���Ƿ�֧��������ָ��

    if( intflag & RB_UIF_TRANSFER )   //�ж�_INT_FG�е�USB��������жϱ�־λ�����д�������ж��ˣ���if���
    {
        if( (R8_USB_INT_ST & MASK_UIS_TOKEN) != MASK_UIS_TOKEN ) // �ǿ���   //�ж��ж�״̬�Ĵ����е�5:4λ���鿴���Ƶ�PID��ʶ��������λ����11����ʾ���У�����if���
        {
            switch( R8_USB_INT_ST & (MASK_UIS_TOKEN | MASK_UIS_ENDP) )
            //ȡ�����Ƶ�PID��ʶ���豸ģʽ�µ�3:0λ�Ķ˵�š�����ģʽ��3:0λ��Ӧ��PID��ʶλ
            // �����������ƺͶ˵��
            {//�˵�0���ڿ��ƴ��䡣���µĶ˵�0��IN��OUT������Ӧ���򣬶�Ӧ���ƴ�������ݽ׶κ�״̬�׶Ρ�
                case UIS_TOKEN_IN:      //���ư���PIDΪIN��5:4λΪ10��3:0λ�Ķ˵��Ϊ0��IN���ƣ��豸�����������ݡ�_UIS_��USB�ж�״̬
                {                       //�˵�0Ϊ˫��˵㣬�������ƴ��䡣 ��|0������ʡ����
                    switch( SetupReqCode )
                    //���ֵ�����յ�SETUP��ʱ��ֵ���ں������SETUP��������򣬶�Ӧ���ƴ�������ý׶Ρ�
                    {
                        case USB_GET_DESCRIPTOR:    //USB��׼���������USB�豸��ȡ����
                            len = SetupReqLen >= DevEP0SIZE ? DevEP0SIZE : SetupReqLen; // ���ΰ����䳤�ȡ��Ϊ64�ֽڣ�����64�ֽڵķֶ�δ���ǰ����Ҫ������
                            tmos_memcpy( pEP0_DataBuf, pDescr, len ); //tmos_memcpy:�ڴ濽����������(����λ)��ַ����(����λ)�ַ������ȵ�(һ��λ)��ַ��
                            //DMAֱ�����ڴ����������⵽�ڴ�ĸ�д�������õ�Ƭ�����ƾͿ��Խ��ڴ��е����ݷ��ͳ�ȥ�����ֻ���������黥�ำֵ�����漰��DMAƥ��������ڴ棬���޷�����DMA��
                            SetupReqLen -= len;     //��¼ʣ�µ���Ҫ���͵����ݳ���
                            pDescr += len;          //���½�������Ҫ���͵����ݵ���ʼ��ַ,����������
                            R8_UEP0_T_LEN = len;    //�˵�0���ͳ��ȼĴ�����д�뱾�ΰ����䳤��
                            R8_UEP0_CTRL ^= RB_UEP_T_TOG;   // ͬ���л���IN���򣨶��ڵ�Ƭ������T���򣩵�PID�е�DATA0��DATA1�л�
                            break;                  //��ֵ��˵���ƼĴ��������ְ���Ӧ��ACK��NAK��STALL������Ӳ������ɷ��Ϲ淶�İ���DMA�Զ�����
                        case USB_SET_ADDRESS:       //USB��׼�������Ϊ�豸����һ��Ψһ��ַ����Χ0��127��0ΪĬ�ϵ�ַ
                            R8_USB_DEV_AD = (R8_USB_DEV_AD & RB_UDA_GP_BIT) | SetupReqLen;
                            //7λ��ַ+���λ���û��Զ����ַ��Ĭ��Ϊ1�������ϡ������䳤�ȡ�������ġ������䳤�ȡ��ں��渳ֵ���˵�ַλ��
                            R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                            //R��ӦOUT����ACK��T��ӦIN����NAK�����CASE��֧����IN���򣬵�DMA��Ӧ�ڴ��У���Ƭ��û�����ݸ���ʱ����NAK���ְ���
                            break;                                                  //һ��������OUT�����豸��ذ�������������ӦNAK��

                        case USB_SET_FEATURE:       //USB��׼�������Ҫ������һ�����豸���ӿڻ�˵��ϵ�����
                            break;

                        default:
                            R8_UEP0_T_LEN = 0;      //״̬�׶�����жϻ�����ǿ���ϴ�0�������ݰ��������ƴ��䣨�����ֶγ���Ϊ0�����ݰ�������SYNC��PID��EOP�ֶζ��У�
                            R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
                            //R��ӦOUT����ACK��T��ӦIN����NAK�����CASE��֧����OUT���򣬵�DMA��Ӧ�ڴ��и����������ҵ�Ƭ����������ʱ����ACK���ְ���
                            USB_READY_FLAG = 1;
                            if(usb_enum_flag)
                            {
                                usb_enum_success_flag = 1;
                            }
#if CONFIG_USB_DEBUG
                            PRINT( "Ready_STATUS = %d\n", USB_READY_FLAG );
#endif
                            break;
                    }
                }
                    break;

                case UIS_TOKEN_OUT:     //���ư���PIDΪOUT��5:4λΪ00��3:0λ�Ķ˵��Ϊ0��OUT���ƣ��������豸�����ݡ�
                {                       //�˵�0Ϊ˫��˵㣬�������ƴ��䡣 ��|0������ʡ����
                    len = R8_USB_RX_LEN;    //��ȡ��ǰUSB���ճ��ȼĴ����д洢�Ľ��յ������ֽ��� //���ճ��ȼĴ���Ϊ�����˵㹲�ã����ͳ��ȼĴ������и���
                    if((pEP0_DataBuf[0]==REPORT_ID_KEYBOARD)&&USB_receive_cb&&(len==2))
                    {
                        USB_receive_buf[0] = USB_DATA_LED;
                        USB_receive_buf[1] = 1;
                        USB_receive_buf[2] = pEP0_DataBuf[1];
                        led_val = pEP0_DataBuf[1];
                        USB_receive_cb(USB_receive_buf, USB_receive_buf[1]+2);
#if CONFIG_USB_DEBUG
                        PRINT( "OUT 0  %x len %d\n", pEP0_DataBuf[1],len );
#endif
                    }
                }
                    break;

                case UIS_TOKEN_OUT | 3:
                {
                    R8_UEP3_CTRL ^= RB_UEP_R_TOG;       //IN�����DATA�л�һ�¡��趨��Ҫ���͵İ���PID��
                    len = R8_USB_RX_LEN;    //��ȡ��ǰUSB���ճ��ȼĴ����д洢�Ľ��յ������ֽ��� //���ճ��ȼĴ���Ϊ�����˵㹲�ã����ͳ��ȼĴ������и���
                    if(USB_receive_cb)
                    {
                        USB_receive_buf[0] = USB_DATA_MANUFACTURER;
                        USB_receive_buf[1] = len;
                        tmos_memcpy(&USB_receive_buf[2], &pEP3_OUT_DataBuf[0], len);
                        USB_receive_cb(USB_receive_buf, len+2);
#if CONFIG_USB_DEBUG
                        PRINT( "OUT 3 %x len %d\n", pEP3_OUT_DataBuf[0],len );
#endif
                    }
                }
                    break;

                case UIS_TOKEN_OUT | 6:
                {
                    R8_UEP6_CTRL ^= RB_UEP_R_TOG;       //IN�����DATA�л�һ�¡��趨��Ҫ���͵İ���PID��
                    len = R8_USB_RX_LEN;    //��ȡ��ǰUSB���ճ��ȼĴ����д洢�Ľ��յ������ֽ��� //���ճ��ȼĴ���Ϊ�����˵㹲�ã����ͳ��ȼĴ������и���
                    if(USB_receive_cb)
                    {
                        USB_receive_buf[0] = USB_DATA_IAP;
                        USB_receive_buf[1] = len;
                        tmos_memcpy(&USB_receive_buf[2], &pEP6_OUT_DataBuf[0], len);
                        USB_receive_cb(USB_receive_buf, len+2);
#if CONFIG_USB_DEBUG
                        PRINT( "OUT 6 %x len %d\n", pEP6_OUT_DataBuf[0],len );
#endif
                    }
                }
                    break;

                case UIS_TOKEN_IN | 1: //���ư���PIDΪIN���˵��Ϊ1
                    R8_UEP1_CTRL ^= RB_UEP_T_TOG;       //IN�����DATA�л�һ�¡��趨��Ҫ���͵İ���PID��
                    R8_UEP1_CTRL = (R8_UEP1_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK; //��DMA��û���ɵ�Ƭ����������ʱ����T��ӦIN������ΪNAK�������˾ͷ������ݡ�
                    USB_READY_FLAG = 1;
#if CONFIG_USB_DEBUG
                    PRINT( "Ready_IN_EP1 = %d\n", USB_READY_FLAG );
#endif
                    break;

                case UIS_TOKEN_IN | 2: //���ư���PIDΪIN���˵��Ϊ2
                    R8_UEP2_CTRL ^= RB_UEP_T_TOG;       //IN�����DATA�л�һ�¡��趨��Ҫ���͵İ���PID��
                    R8_UEP2_CTRL = (R8_UEP2_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK; //��DMA��û���ɵ�Ƭ����������ʱ����T��ӦIN������ΪNAK�������˾ͷ������ݡ�
                    USB_READY_FLAG = 1;
#if CONFIG_USB_DEBUG
                    PRINT( "Ready_IN_EP2 = %d\n", USB_READY_FLAG );
#endif
                    break;

                case UIS_TOKEN_IN | 3: //���ư���PIDΪIN���˵��Ϊ3
                    R8_UEP3_CTRL ^= RB_UEP_T_TOG;       //IN�����DATA�л�һ�¡��趨��Ҫ���͵İ���PID��
                    R8_UEP3_CTRL = (R8_UEP3_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK; //��DMA��û���ɵ�Ƭ����������ʱ����T��ӦIN������ΪNAK�������˾ͷ������ݡ�
                    USB_READY_FLAG = 1;
#if CONFIG_USB_DEBUG
                    PRINT( "Ready_IN_EP3 = %d\n", USB_READY_FLAG );
#endif
                    break;

                case UIS_TOKEN_IN | 5: //���ư���PIDΪIN���˵��Ϊ5
                    R8_UEP5_CTRL ^= RB_UEP_T_TOG;       //IN�����DATA�л�һ�¡��趨��Ҫ���͵İ���PID��
                    R8_UEP5_CTRL = (R8_UEP5_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK; //��DMA��û���ɵ�Ƭ����������ʱ����T��ӦIN������ΪNAK�������˾ͷ������ݡ�
                    USB_READY_FLAG = 1;
#if CONFIG_USB_DEBUG
                    PRINT( "Ready_IN_EP5 = %d\n", USB_READY_FLAG );
#endif
                    break;

                case UIS_TOKEN_IN | 6: //���ư���PIDΪIN���˵��Ϊ3
                    R8_UEP6_CTRL ^= RB_UEP_T_TOG;       //IN�����DATA�л�һ�¡��趨��Ҫ���͵İ���PID��
                    R8_UEP6_CTRL = (R8_UEP6_CTRL & ~MASK_UEP_T_RES) | UEP_T_RES_NAK; //��DMA��û���ɵ�Ƭ����������ʱ����T��ӦIN������ΪNAK�������˾ͷ������ݡ�
                    USB_READY_FLAG = 1;
#if CONFIG_USB_DEBUG
                    PRINT( "Ready_IN_EP6 = %d\n", USB_READY_FLAG );
#endif
                    break;
            }
            R8_USB_INT_FG = RB_UIF_TRANSFER;    //д1�����жϱ�־
        }

        if( R8_USB_INT_ST & RB_UIS_SETUP_ACT ) // Setup������
        {
//            R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
            //R��ӦOUT�����ڴ�ΪDATA1��DMA�յ������ݰ���PIDҪΪDATA1�����������ݴ���Ҫ�ش�����ACK��DMA��Ӧ�ڴ����յ������ݣ���Ƭ������������
            //T��ӦIN�����趨ΪDATA1����Ƭ������������DMA��Ӧ�ڴ棬��DATA1���ͳ�ȥ����NAK����Ƭ��û��׼�������ݣ���
            SetupReqLen = pSetupReqPak->wLength;    //���ݽ׶ε��ֽ���      //pSetupReqPak�����˵�0��RAM��ַǿ��ת����һ����Žṹ��ĵ�ַ���ṹ���Ա���ν�������
            SetupReqCode = pSetupReqPak->bRequest;  //��������
            chtype = pSetupReqPak->bRequestType;    //�������ݴ��䷽����������͡����յĶ������Ϣ

            len = 0;
            errflag = 0;
            if( (pSetupReqPak->bRequestType & USB_REQ_TYP_MASK) != USB_REQ_TYP_STANDARD ) //�ж���������ͣ�������Ǳ�׼���󣬽�if���
            {
                /* �Ǳ�׼���� */
                /* ��������,�������󣬲�������� */
                if( pSetupReqPak->bRequestType & 0x40 )   //ȡ�������е�ĳһλ���ж��Ƿ�Ϊ0����Ϊ���if���
                {
                    /* �������� */
                }
                else if( pSetupReqPak->bRequestType & 0x20 )  //ȡ�������е�ĳһλ���ж��Ƿ�Ϊ0����Ϊ���if���
                {   //�ж�ΪHID������
                    switch( SetupReqCode )
                    //�ж���������
                    {
                        case DEF_USB_SET_IDLE: /* 0x0A: SET_IDLE */         //����������HID�豸�ض����뱨��Ŀ���ʱ����
                            Idle_Value[pSetupReqPak->wIndex] = (uint8_t)(pSetupReqPak->wValue>>8);
                            break; //���һ��Ҫ��

                        case DEF_USB_SET_REPORT: /* 0x09: SET_REPORT */     //����������HID�豸�ı���������
                            break;

                        case DEF_USB_SET_PROTOCOL: /* 0x0B: SET_PROTOCOL */ //����������HID�豸��ǰ��ʹ�õ�Э��
                            Report_Value[pSetupReqPak->wIndex] = (uint8_t)(pSetupReqPak->wValue);
                            break;

                        case DEF_USB_GET_IDLE: /* 0x02: GET_IDLE */         //�������ȡHID�豸�ض����뱨��ĵ�ǰ�Ŀ��б���
                            EP0_Databuf[0] = Idle_Value[pSetupReqPak->wIndex];
                            len = 1;
                            break;

                        case DEF_USB_GET_PROTOCOL: /* 0x03: GET_PROTOCOL */     //��������HID�豸��ǰ��ʹ�õ�Э��
                            EP0_Databuf[0] = Report_Value[pSetupReqPak->wIndex];
                            len = 1;
                            break;

                        default:
                            errflag = 0xFF;
                    }
                }
            }
            else    //�ж�Ϊ��׼����
            {
                switch( SetupReqCode )
                //�ж���������
                {
                    case USB_GET_DESCRIPTOR:    //�������ñ�׼������
                    {
                        switch( ((pSetupReqPak->wValue) >> 8) )
                        //����8λ����ԭ���ĸ�8λ�Ƿ�Ϊ0��Ϊ1��ʾ����ΪIN�������s-case���
                        {
                            case USB_DESCR_TYP_DEVICE:  //��ͬ��ֵ����ͬ��������������豸������
                            {
                                tmos_stop_task(tran_taskID, SBP_ENTER_SLEEP_EVT);
                                pDescr = MyDevDescr;    //���豸�������ַ�������pDescr��ַ�У�����ñ�׼�����������caseĩβ���ÿ�����������
                                len = MyDevDescr[0];    //Э��涨�豸�����������ֽڴ���ֽ������ȡ������������õ�len����
                            }
                                break;

                            case USB_DESCR_TYP_CONFIG:  //������������������
                            {
                                pDescr = MyCfgDescr;    //�������������ַ�������pDescr��ַ�У�֮��ᷢ��
                                len = MyCfgDescr[2];    //Э��涨�����������ĵ������ֽڴ��������Ϣ���ܳ�
                            }
                                break;

                            case USB_DESCR_TYP_HID:     //���������˻��ӿ������������˴��ṹ���е�wIndex��������������ͬ����Ϊ�ӿںš�
                                switch( (pSetupReqPak->wIndex) & 0xff )
                                //ȡ�Ͱ�λ���߰�λĨȥ
                                {
                                    /* ѡ��ӿ� */
                                    case 0:
                                        pDescr = (uint8_t *) (&MyCfgDescr[18]);  //�ӿ�1�������������λ�ã�������
                                        len = 9;
                                        break;

                                    /* ѡ��ӿ� */
                                    case 1:
                                        pDescr = (uint8_t *) (&MyCfgDescr[43]);  //�ӿ�1�������������λ�ã�������
                                        len = 9;
                                        break;

                                    /* ѡ��ӿ� */
                                    case 2:
                                        pDescr = (uint8_t *) (&MyCfgDescr[68]);  //�ӿ�1�������������λ�ã�������
                                        len = 9;
                                        break;

                                    /* ѡ��ӿ� */
                                    case 3:
                                        pDescr = (uint8_t *) (&MyCfgDescr[68+25+7]);  //�ӿ�1�������������λ�ã�������
                                        len = 9;
                                        break;

//                                    /* ѡ��ӿ� */
//                                    case 4:
//                                        pDescr = (uint8_t *) (&MyCfgDescr[68+25+9+9]);  //�ӿ�1�������������λ�ã�������
//                                        len = 9;
//                                        break;

                                    default:
                                        /* ��֧�ֵ��ַ��������� */
                                        errflag = 0xff;
                                        break;
                                }
                                break;

                            case USB_DESCR_TYP_REPORT:  //���������豸����������
                            {
                                if( ((pSetupReqPak->wIndex) & 0xff) == 0 ) //�ӿ�0����������
                                {
                                    pDescr = HID_ReportDescriptorMouse; //����׼���ϴ�
                                    len = HID_ReportDescSizeMouse;
                                }
                                else if( ((pSetupReqPak->wIndex) & 0xff) == 1 ) //�ӿ�1����������
                                {
                                    pDescr = HID_ReportDescriptorOther; //����׼���ϴ�
                                    len = HID_ReportDescSizeOther;
                                }
                                else if( ((pSetupReqPak->wIndex) & 0xff) == 2 ) //�ӿ�2����������
                                {
                                    pDescr = HID_ReportDescriptorManufacturer; //����׼���ϴ�
                                    len = HID_ReportDescSizeManufacturer;
                                }
                                else if( ((pSetupReqPak->wIndex) & 0xff) == 3 ) //�ӿ�3����������
                                {
                                    usb_enum_flag = 1;
                                    pDescr = HID_ReportDescriptorIAP; //����׼���ϴ�
                                    len = HID_ReportDescSizeIAP;
                                }
                                else
                                    len = 0xff; //������ֻ��4���ӿڣ���仰����������ִ��
                            }
                                break;

                            case USB_DESCR_TYP_STRING:  //���������豸�ַ���������
                            {
                                switch( (pSetupReqPak->wValue) & 0xff )
                                //����wValue��ֵ�����ַ�����Ϣ
                                {
                                    case 0:
                                        pDescr = LangID_StrDescr;
                                        len = LangID_StrDescr[0];
                                        break;
                                    case 1:
                                        pDescr = MyManuInfo;
                                        len = MyManuInfo[0];
                                        break;
                                    case 2:
                                        pDescr = MyProdInfo;
                                        len = MyProdInfo[0];
                                        break;
                                    case 3:
                                        pDescr = MySerialNum;
                                        len = MySerialNum[0];
                                        break;
                                    default:
                                        errflag = 0xFF; // ��֧�ֵ��ַ���������
                                        break;
                                }
                            }
                                break;

                            default:
                                errflag = 0xff;
                                break;
                        }
                        if( SetupReqLen > len )
                            SetupReqLen = len;      //ʵ�����ϴ��ܳ���
                        len = (SetupReqLen >= DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;   //��󳤶�Ϊ64�ֽ�
                        tmos_memcpy( pEP0_DataBuf, pDescr, len );  //��������
                        pDescr += len;
                    }
                        break;

                    case USB_SET_ADDRESS:       //�����������豸��ַ
                        SetupReqLen = (pSetupReqPak->wValue) & 0xff;    //�������ַ���λ�豸��ַ�ݴ���SetupReqLen��
                        break;                                          //���ƽ׶λḳֵ���豸��ַ����

                    case USB_GET_CONFIGURATION: //���������豸��ǰ����
                        pEP0_DataBuf[0] = DevConfig;    //���豸���÷Ž�RAM
                        if( SetupReqLen > 1 )
                            SetupReqLen = 1;    //�����ݽ׶ε��ֽ�����1����ΪDevConfigֻ��һ���ֽ�
                        break;

                    case USB_SET_CONFIGURATION: //�����������豸��ǰ����
                        DevConfig = (pSetupReqPak->wValue) & 0xff;  //ȡ�Ͱ�λ���߰�λĨȥ
                        break;

                    case USB_CLEAR_FEATURE:     //�ر�USB�豸������/���ܡ��������豸���Ƕ˵�����ϵġ�
                    {
                        if( (pSetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP ) //�ж��ǲ��Ƕ˵�����������˵�ֹͣ������״̬��
                        {
                            switch( (pSetupReqPak->wIndex) & 0xff )
                            //ȡ�Ͱ�λ���߰�λĨȥ���ж�����
                            {//16λ�����λ�ж����ݴ��䷽��0ΪOUT��1ΪIN����λΪ�˵�š�
                                case 0x81:      //����_TOG��_T_RES����λ����������д��_NAK����ӦIN����NAK��ʾ�����ݷ���
                                    R8_UEP1_CTRL = (R8_UEP1_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                                    break;
                                case 0x01:      //����_TOG��_R_RES����λ����������д��_ACK����ӦOUT����ACK��ʾ��������
                                    R8_UEP1_CTRL = (R8_UEP1_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                                    break;
                                case 0x82:      //����_TOG��_T_RES����λ����������д��_NAK����ӦIN����NAK��ʾ�����ݷ���
                                    R8_UEP2_CTRL = (R8_UEP2_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                                    break;
                                case 0x02:      //����_TOG��_R_RES����λ����������д��_ACK����ӦOUT����ACK��ʾ��������
                                    R8_UEP2_CTRL = (R8_UEP2_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                                    break;
                                case 0x83:      //����_TOG��_T_RES����λ����������д��_NAK����ӦIN����NAK��ʾ�����ݷ���
                                    R8_UEP3_CTRL = (R8_UEP3_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                                    break;
                                case 0x03:      //����_TOG��_R_RES����λ����������д��_ACK����ӦOUT����ACK��ʾ��������
                                    R8_UEP3_CTRL = (R8_UEP3_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                                    break;
                                case 0x85:      //����_TOG��_T_RES����λ����������д��_NAK����ӦIN����NAK��ʾ�����ݷ���
                                    R8_UEP5_CTRL = (R8_UEP5_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                                    break;
                                case 0x05:      //����_TOG��_R_RES����λ����������д��_ACK����ӦOUT����ACK��ʾ��������
                                    R8_UEP5_CTRL = (R8_UEP5_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                                    break;
                                case 0x86:      //����_TOG��_T_RES����λ����������д��_NAK����ӦIN����NAK��ʾ�����ݷ���
                                    R8_UEP6_CTRL = (R8_UEP6_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_NAK;
                                    break;
                                case 0x06:      //����_TOG��_R_RES����λ����������д��_ACK����ӦOUT����ACK��ʾ��������
                                    R8_UEP6_CTRL = (R8_UEP6_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_ACK;
                                    break;
                                default:
                                    errflag = 0xFF; // ��֧�ֵĶ˵�
                                    break;
                            }
                        }
                        else if( (pSetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_DEVICE ) //�ж��ǲ����豸�����������豸���ѣ�
                        {
#if CONFIG_USB_DEBUG
                            PRINT( "��˯��\n" );
#endif
                            if( pSetupReqPak->wValue == 1 )   //���ѱ�־λΪ1
                            {
                                USB_SleepStatus &= ~HOST_SET_FEATURE;   //���λ����
                                USB_SleepStatus |= HOST_WAKEUP_ENABLE;
                            }
                        }
                        else
                        {
                            errflag = 0xFF;
                        }
                    }
                        break;

                    case USB_SET_FEATURE:       //����USB�豸������/���ܡ��������豸���Ƕ˵�����ϵġ�
                        if( (pSetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP ) //�ж��ǲ��Ƕ˵�������ʹ�˵�ֹͣ������
                        {
                            /* �˵� */
                            switch( pSetupReqPak->wIndex )
                            //�ж�����
                            {//16λ�����λ�ж����ݴ��䷽��0ΪOUT��1ΪIN����λΪ�˵�š�
                                case 0x81:      //����_TOG��_T_RES��λ����������д��_STALL����������ָ��ֹͣ�˵�Ĺ���
                                    R8_UEP1_CTRL = (R8_UEP1_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_STALL;
                                    break;
                                case 0x01:      //����_TOG��_R_RES��λ����������д��_STALL����������ָ��ֹͣ�˵�Ĺ���
                                    R8_UEP1_CTRL = (R8_UEP1_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_STALL;
                                    break;
                                case 0x82:      //����_TOG��_T_RES��λ����������д��_STALL����������ָ��ֹͣ�˵�Ĺ���
                                    R8_UEP2_CTRL = (R8_UEP2_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_STALL;
                                    break;
                                case 0x02:      //����_TOG��_R_RES��λ����������д��_STALL����������ָ��ֹͣ�˵�Ĺ���
                                    R8_UEP2_CTRL = (R8_UEP2_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_STALL;
                                    break;
                                case 0x83:      //����_TOG��_T_RES��λ����������д��_STALL����������ָ��ֹͣ�˵�Ĺ���
                                    R8_UEP3_CTRL = (R8_UEP3_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_STALL;
                                    break;
                                case 0x03:      //����_TOG��_R_RES��λ����������д��_STALL����������ָ��ֹͣ�˵�Ĺ���
                                    R8_UEP3_CTRL = (R8_UEP3_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_STALL;
                                    break;
                                case 0x85:      //����_TOG��_T_RES��λ����������д��_STALL����������ָ��ֹͣ�˵�Ĺ���
                                    R8_UEP5_CTRL = (R8_UEP5_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_STALL;
                                    break;
                                case 0x05:      //����_TOG��_R_RES��λ����������д��_STALL����������ָ��ֹͣ�˵�Ĺ���
                                    R8_UEP5_CTRL = (R8_UEP5_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_STALL;
                                    break;
                                case 0x86:      //����_TOG��_T_RES��λ����������д��_STALL����������ָ��ֹͣ�˵�Ĺ���
                                    R8_UEP6_CTRL = (R8_UEP6_CTRL & ~(RB_UEP_T_TOG | MASK_UEP_T_RES)) | UEP_T_RES_STALL;
                                    break;
                                case 0x06:      //����_TOG��_R_RES��λ����������д��_STALL����������ָ��ֹͣ�˵�Ĺ���
                                    R8_UEP6_CTRL = (R8_UEP6_CTRL & ~(RB_UEP_R_TOG | MASK_UEP_R_RES)) | UEP_R_RES_STALL;
                                    break;
                                default:
                                    /* ��֧�ֵĶ˵� */
                                    errflag = 0xFF; // ��֧�ֵĶ˵�
                                    break;
                            }
                        }
                        else if( (pSetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_DEVICE ) //�ж��ǲ����豸������ʹ�豸���ߣ�
                        {
#if CONFIG_USB_DEBUG
                            PRINT( "����˯��\n" );
#endif
                            if( pSetupReqPak->wValue == 1 )
                            {
                                USB_SleepStatus |= HOST_SET_FEATURE;    //����˯��
                            }
                        }
                        else
                        {
                            errflag = 0xFF;
                        }
                        break;

                    case USB_GET_INTERFACE:     //�������ýӿڵ�ǰ������ѡ������ֵ
                        pEP0_DataBuf[0] = 0x00;
                        if( SetupReqLen > 1 )
                            SetupReqLen = 1;    //�����ݽ׶ε��ֽ�����1����Ϊ��������ֻ��һ���ֽ�
                        break;

                    case USB_SET_INTERFACE:     //�����뼤���豸��ĳ���ӿ�
                        break;

                    case USB_GET_STATUS:        //���������豸���ӿڻ��Ƕ˵��״̬
                        if( (pSetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP ) //�ж��Ƿ�Ϊ�˵�״̬
                        {
                            /* �˵� */
                            pEP0_DataBuf[0] = 0x00;
                            switch( pSetupReqPak->wIndex )
                            {       //16λ�����λ�ж����ݴ��䷽��0ΪOUT��1ΪIN����λΪ�˵�š�
                                case 0x81:      //�ж�_TOG��_T_RES��λ��������STALL״̬����if���
                                    if( (R8_UEP1_CTRL & (RB_UEP_T_TOG | MASK_UEP_T_RES)) == UEP_T_RES_STALL )
                                    {
                                        pEP0_DataBuf[0] = 0x01; //����D0Ϊ1����ʾ�˵㱻ֹͣ�����ˡ���λ��SET_FEATURE��CLEAR_FEATURE�������á�
                                    }
                                    break;

                                case 0x01:      //�ж�_TOG��_R_RES��λ��������STALL״̬����if���
                                    if( (R8_UEP1_CTRL & (RB_UEP_R_TOG | MASK_UEP_R_RES)) == UEP_R_RES_STALL )
                                    {
                                        pEP0_DataBuf[0] = 0x01;
                                    }
                                    break;
                                case 0x82:      //�ж�_TOG��_T_RES��λ��������STALL״̬����if���
                                    if( (R8_UEP2_CTRL & (RB_UEP_T_TOG | MASK_UEP_T_RES)) == UEP_T_RES_STALL )
                                    {
                                        pEP0_DataBuf[0] = 0x01; //����D0Ϊ1����ʾ�˵㱻ֹͣ�����ˡ���λ��SET_FEATURE��CLEAR_FEATURE�������á�
                                    }
                                    break;

                                case 0x02:      //�ж�_TOG��_R_RES��λ��������STALL״̬����if���
                                    if( (R8_UEP2_CTRL & (RB_UEP_R_TOG | MASK_UEP_R_RES)) == UEP_R_RES_STALL )
                                    {
                                        pEP0_DataBuf[0] = 0x01;
                                    }
                                    break;
                                case 0x83:      //�ж�_TOG��_T_RES��λ��������STALL״̬����if���
                                    if( (R8_UEP3_CTRL & (RB_UEP_T_TOG | MASK_UEP_T_RES)) == UEP_T_RES_STALL )
                                    {
                                        pEP0_DataBuf[0] = 0x01; //����D0Ϊ1����ʾ�˵㱻ֹͣ�����ˡ���λ��SET_FEATURE��CLEAR_FEATURE�������á�
                                    }
                                    break;

                                case 0x03:      //�ж�_TOG��_R_RES��λ��������STALL״̬����if���
                                    if( (R8_UEP3_CTRL & (RB_UEP_R_TOG | MASK_UEP_R_RES)) == UEP_R_RES_STALL )
                                    {
                                        pEP0_DataBuf[0] = 0x01;
                                    }
                                    break;
                                case 0x85:      //�ж�_TOG��_T_RES��λ��������STALL״̬����if���
                                    if( (R8_UEP5_CTRL & (RB_UEP_T_TOG | MASK_UEP_T_RES)) == UEP_T_RES_STALL )
                                    {
                                        pEP0_DataBuf[0] = 0x01; //����D0Ϊ1����ʾ�˵㱻ֹͣ�����ˡ���λ��SET_FEATURE��CLEAR_FEATURE�������á�
                                    }
                                    break;

                                case 0x05:      //�ж�_TOG��_R_RES��λ��������STALL״̬����if���
                                    if( (R8_UEP5_CTRL & (RB_UEP_R_TOG | MASK_UEP_R_RES)) == UEP_R_RES_STALL )
                                    {
                                        pEP0_DataBuf[0] = 0x01;
                                    }
                                    break;
                                case 0x86:      //�ж�_TOG��_T_RES��λ��������STALL״̬����if���
                                    if( (R8_UEP6_CTRL & (RB_UEP_T_TOG | MASK_UEP_T_RES)) == UEP_T_RES_STALL )
                                    {
                                        pEP0_DataBuf[0] = 0x01; //����D0Ϊ1����ʾ�˵㱻ֹͣ�����ˡ���λ��SET_FEATURE��CLEAR_FEATURE�������á�
                                    }
                                    break;

                                case 0x06:      //�ж�_TOG��_R_RES��λ��������STALL״̬����if���
                                    if( (R8_UEP6_CTRL & (RB_UEP_R_TOG | MASK_UEP_R_RES)) == UEP_R_RES_STALL )
                                    {
                                        pEP0_DataBuf[0] = 0x01;
                                    }
                                    break;
                            }
                        }
                        else if( (pSetupReqPak->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_DEVICE ) //�ж��Ƿ�Ϊ�豸״̬
                        {
                            pEP0_DataBuf[0] = 0x00;
                            if( USB_SleepStatus&HOST_SET_FEATURE )     //����豸����˯��״̬
                            {
                                pEP0_DataBuf[0] = 0x02;     //���λD0Ϊ0����ʾ�豸�����߹��磬Ϊ1��ʾ�豸�Թ��硣 D1λΪ1��ʾ֧��Զ�̻��ѣ�Ϊ0��ʾ��֧�֡�
                            }
                            else
                            {
                                pEP0_DataBuf[0] = 0x00;
                            }
                        }
                        pEP0_DataBuf[1] = 0;    //����״̬��Ϣ�ĸ�ʽΪ16λ�����߰�λ����Ϊ0
                        if( SetupReqLen >= 2 )
                        {
                            SetupReqLen = 2;    //�����ݽ׶ε��ֽ�����2����Ϊ��������ֻ��2���ֽ�
                        }
                        break;

                    default:
                        errflag = 0xff;
                        break;
                }
            }
            if( errflag == 0xff ) // �����֧��
            {
                //                  SetupReqCode = 0xFF;
                R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL; // STALL
                USB_READY_FLAG = 1;
#if CONFIG_USB_DEBUG
                PRINT( "Ready_Stall = %d\n", USB_READY_FLAG );
#endif
            }
            else
            {
                if( chtype & 0x80 )   // �ϴ������λΪ1�����ݴ��䷽��Ϊ�豸���������䡣
                {
                    len = (SetupReqLen > DevEP0SIZE) ? DevEP0SIZE : SetupReqLen;
                    SetupReqLen -= len;
                }
                else
                    len = 0;        // �´������λΪ0�����ݴ��䷽��Ϊ�������豸���䡣
                R8_UEP0_T_LEN = len;
                R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;     // Ĭ�����ݰ���DATA1
            }

            R8_USB_INT_FG = RB_UIF_TRANSFER;    //д1���жϱ�ʶ
        }
    }

    else if( intflag & RB_UIF_BUS_RST )   //�ж�_INT_FG�е����߸�λ��־λ��Ϊ1����
    {
        USB_READY_FLAG = 1;
        tmos_stop_task(tran_taskID, SBP_ENTER_SLEEP_EVT);
        R8_USB_DEV_AD = 0;      //�豸��ַд��0�����������·�����豸һ���µ�ַ
        R8_UEP0_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;   //�Ѷ˵�0�Ŀ��ƼĴ�����д�ɣ�������Ӧ��ӦACK��ʾ�����յ���������ӦNAK��ʾû������Ҫ����
        R8_UEP1_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        R8_UEP2_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        R8_UEP3_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        R8_UEP5_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        R8_UEP6_CTRL = UEP_R_RES_ACK | UEP_T_RES_NAK;
        R8_USB_INT_FG = RB_UIF_BUS_RST; //д1���жϱ�ʶ
    }
    else if( intflag & RB_UIF_SUSPEND )   //�ж�_INT_FG�е����߹�������¼��жϱ�־λ������ͻ��Ѷ��ᴥ�����ж�
    {
        if( R8_USB_MIS_ST & RB_UMS_SUSPEND )  //ȡ������״̬�Ĵ����еĹ���״̬λ��Ϊ1��ʾUSB���ߴ��ڹ���̬��Ϊ0��ʾ���ߴ��ڷǹ���̬
        {
            USB_READY_FLAG = 0;
            USB_SleepStatus |= HOST_SET_SUSPEND;
            if(USB_SleepStatus & HOST_SET_FEATURE)
            {
                tmos_set_event(tran_taskID, SBP_ENTER_SLEEP_EVT);
            }
#if CONFIG_USB_DEBUG
            PRINT( "Ready_Sleep = %x\n", USB_SleepStatus );
#endif
        } // ����     //���豸���ڿ���״̬����3ms��������Ҫ���豸���������ڵ������ߣ�
        else    //��������жϱ���������û�б��ж�Ϊ����
        {
            USB_READY_FLAG = 1;
            USB_SleepStatus &= ~HOST_SET_SUSPEND;
            USB_SleepStatus |= HOST_WAKEUP_ENABLE;
            tmos_stop_task(tran_taskID, SBP_ENTER_SLEEP_EVT);
#if CONFIG_USB_DEBUG
            PRINT( "Ready_WeakUp = %x\n", USB_SleepStatus );
#endif
        } // ����
        R8_USB_INT_FG = RB_UIF_SUSPEND; //д1���жϱ�־
    }
    else
    {
        R8_USB_INT_FG = intflag;    //_INT_FG��û���жϱ�ʶ���ٰ�ԭֵд��ԭ���ļĴ���
    }
}

/*********************************************************************
 * @fn      USB_IRQHandler
 *
 * @brief   USB�жϺ���
 *
 * @return  none
 */
__attribute__((interrupt("WCH-Interrupt-fast")))
__attribute__((section(".highcode")))
void USB_IRQHandler( void ) /* USB�жϷ������,ʹ�üĴ�����1 */
{
    intflag = R8_USB_INT_FG;        //ȡ���жϱ�ʶ�Ĵ�����ֵ

    if( intflag & RB_UIF_TRANSFER )   //�ж�_INT_FG�е�USB��������жϱ�־λ�����д�������ж��ˣ���if���
    {
        if( R8_USB_INT_ST & RB_UIS_SETUP_ACT ) // Setup������
        {
            R8_UEP0_CTRL = RB_UEP_R_TOG | RB_UEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_NAK;
        }
    }
    USB_IRQ_trans_process();

}

/******************************** endfile @ usb ******************************/
