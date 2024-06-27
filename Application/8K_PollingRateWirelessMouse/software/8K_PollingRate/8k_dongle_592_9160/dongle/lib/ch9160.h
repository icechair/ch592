/********************************** (C) COPYRIGHT *******************************
 * File Name          : ch9160.h
 * Author             : WCH
 * Version            : V1.0
 * Date               : 2018/11/12
 * Description        :
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

#ifndef ch9160_H
#define ch9160_H

#include "stdint.h"
#ifdef __cplusplus
"C" {
#endif

// ACCESS_CMD��ʱ10ms
#define ACCESS_CMD_TIMEOUT      16

// ACCESS_STATE
#define STATE_SUCCESS           0x00
#define STATE_PENDING           0xFF

#define INTERFAVE_RECV_BUF_LEN  256
#define INTERFAVE_TRAN_BUF_LEN  256

#define CMD_ACK                 0x80
#define CMD_ACK_LEN             0x07

#define CHK_DATA                0x55
#define CHK_ACK                 0xAA

// ��������
#define CMD_CHK_CONNECT         0x01
#define CMD_GET_INFO            0x02
#define CMD_SET_INFO            0x03
#define CMD_GET_USB_DESC        0x04
#define CMD_SET_USB_DESC        0x05
#define CMD_SNED_ENDP_DATA1     0x06
#define CMD_SNED_ENDP_DATA2     0x07
#define CMD_RESET               0x08
#define CMD_AUTO_SEND_STATUS    0x89
#define CMD_AUTO_SEND_PC_DATA   0x8A

#define CMD_MRAK_MAX            0x8F
#define CMD_MRAK_ENDP           0x70

#define STATUS_ENUM_SUCCESS         0x00
#define STATUS_SET_REPORT_CHANGE    0x01
#define STATUS_RECV_SET_REPORT      0x02
#define STATUS_RECV_GET_REPORT      0x03
#define STATUS_RECV_BUF_FULL        0x04
#define STATUS_SLEEP                0x05

#define ENTER_SLEEP                 0x55
#define EXIT_SLEEP                  0xAA

#define USB_WEAK_UP_IO              (GPIO_Pin_12)
#define USB_WEAK_UP()               GPIOB_SetBits(USB_WEAK_UP_IO)
#define USB_SLEEP()                 GPIOB_ResetBits(USB_WEAK_UP_IO)

#define  VER_CH9160_FILE            "CH9160_LIB_V1.0"
extern const uint8_t VER_CH9160_LIB[];

typedef void (*access_cmd_cb_t)(uint8_t state, uint8_t cmd, uint8_t *pData, uint16_t len);

/**
 * @brief   ע������������ջص�
 *
 * @param   cb - �ص�����
 */
void access_register_cmd_cb(access_cmd_cb_t cb);

/**
 * @brief   ���USB����״̬
 *
 * @return  @ACCESS_STATE.
 */
uint8_t access_chk_connect(void);

/**
 * @brief   ���USB����״̬
 *
 * @return  @ACCESS_STATE.
 */
uint8_t access_get_info(void);

/**
 * @brief   ����USB
 *
 * @param   usb_enable  : 1 - ʹ��USB; 0 - �ر�USB
 *          io_dir      : IO���� 0��ʾ����; 1��ʾ���
 *          io_pin      : IO��ƽ 0��ʾ�͵�ƽ; 1��ʾ�ߵ�ƽ
 *          endpx_size  : 0~9��Ӧ�˵㳤��Ϊ2��0~9���ݣ�����2��Ӧ����Ϊ4��6��Ӧ����Ϊ64��9��Ӧ����Ϊ512
 *          sleep_off   : 2 - USB���˯��; 1 - �ر�USB˯��; 0 - USBǳ˯��
 *
 * @return  @ACCESS_STATE.
 */
uint8_t access_set_info(uint8_t usb_enable, uint8_t io_dir, uint8_t io_pin,
    uint8_t endp1_size, uint8_t endp2_size, uint8_t endp3_size, uint8_t endp4_size, uint8_t sleep_off);

/**
 * @brief   ��ȡUSB������
 *
 * @param   desc_type   : 0x00    USB�豸������
 *                        0x01    USB����������
 *                        0x02    USB HID1����������
 *                        0x03    USB HID2����������
 *                        0x04    USB HID3����������
 *                        0x05    USB HID4����������
 *                        0x06    USB HID5����������
 *                        0x07    USB�ַ���0(����)������
 *                        0x08    USB�ַ���1(����)������
 *                        0x09    USB�ַ���2(��Ʒ)������
 *                        0x0A    USB�ַ���3(���к�)������
 *                        0x0B    USB�ַ���4������
 *          offset      : USB������ƫ�Ƶ�ַ
 *          length      : USB���������ݳ���
 *
 * @return  @ACCESS_STATE.
 */
uint8_t access_get_usb_desc(uint8_t desc_type, uint16_t offset, uint16_t length);

/**
 * @brief   ����USB������
 *
 * @param   desc_type   : 0x00    USB�豸������
 *                        0x01    USB����������
 *                        0x02    USB HID1����������
 *                        0x03    USB HID2����������
 *                        0x04    USB HID3����������
 *                        0x05    USB HID4����������
 *                        0x06    USB HID5����������
 *                        0x07    USB�ַ���0(����)������
 *                        0x08    USB�ַ���1(����)������
 *                        0x09    USB�ַ���2(��Ʒ)������
 *                        0x0A    USB�ַ���3(���к�)������
 *                        0x0B    USB�ַ���4������
 *          offset      : USB������ƫ�Ƶ�ַ
 *          length      : USB���������ݳ���
 *          pData       : USB����������
 *
 * @return  @ACCESS_STATE.
 */
uint8_t access_set_usb_desc(uint8_t desc_type, uint16_t offset, uint16_t length, uint8_t *pData);

/**
 * @brief   ָ���˵㷢�����ݣ����ͺ���ȴ��յ�Ӧ����ٷ����µ�����
 *
 * @param   endp    : �˵�ţ�֧��1~4
 *          pData   : ����ָ��
 *          length  : ���ݳ���
 *
 * @return  @ACCESS_STATE.
 */
uint8_t access_send_endp_data( uint8_t endp, uint8_t *pData, uint16_t length);

/**
 * @brief   ָ���˵㷢�����ݣ�����ȴ�Ӧ��
 *
 * @param   endp    : �˵�ţ�֧��1~4
 *          pData   : ����ָ��
 *          length  : ���ݳ���
 *
 * @return  @ACCESS_STATE.
 */
uint8_t access_send_endp_data_without_ack( uint8_t endp, uint8_t *pData, uint16_t length);

/**
 * @brief   ��λUSB
 *
 * @param   reset_type :  0x00��оƬ���帴λ
 *                        0x01��оƬ��USB��λ
 *
 * @return  @ACCESS_STATE.
 */
uint8_t access_usb_reset(uint8_t reset_type);

/**
 * @brief   �յ�USB�����ϱ����������Ҫ����Ӧ��
 *
 * @param   cmd :  ���յ���USB�����ϱ���������
 *
 * @return  @ACCESS_STATE.
 */
uint8_t access_send_ack(uint8_t cmd);

/**
 * @brief   USB���ݴ���
 */
void trans_process(void);

/**
 * @brief   �ش���һ�����͸�USB������
 *
 * @return  @ACCESS_STATE.
 */
uint8_t trans_retran_last_data(void );

/**
 * @brief   ������ʼ��
 */
void access_Init( void );

/**
 * @brief   CH9160��ʼ��
 */
void ch9160_Init( void );


#ifdef __cplusplus
}
#endif

#endif
