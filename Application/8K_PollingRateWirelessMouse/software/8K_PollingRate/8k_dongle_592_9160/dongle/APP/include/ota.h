/********************************** (C) COPYRIGHT *******************************
 * File Name          : OTA.h
 * Author             : WCH
 * Version            : V1.10
 * Date               : 2018/12/14
 * Description        : oad������ö���
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/******************************************************************************/
#ifndef __OTA_H
#define __OTA_H

#define OTA_FLASH_ERASE_EVT     0x0004  //OTA Flash��������
#define OTA_IAP_END_EVT         0x0008  //OTA IAP_END
#define OTA_RESTART_EVT         0x0010  //OTA RESTART

#define CHIP_ID             0x9160

#define IAP_SAFE_FLAG       0x0d0251e0

/* ------------------------------------------------------------------------------------------------
 *                                             OTA FLASH
 * ------------------------------------------------------------------------------------------------
 */

/* �����û�code���ֳ��Ŀ飬4K��84K��84K��12K������������ֱ����imageA���û�����������imageB�����ݴ���������imageIAP */

/* FLASH���� */
#define FLASH_BLOCK_SIZE       EEPROM_BLOCK_SIZE
#define IMAGE_SIZE             84 * 1024

/* imageA���� */
#define IMAGE_A_FLAG           0x01
#define IMAGE_A_START_ADD      4 * 1024
#define IMAGE_A_SIZE           IMAGE_SIZE

/* imageB���� */
#define IMAGE_B_FLAG           0x02
#define IMAGE_B_START_ADD      (IMAGE_A_START_ADD + IMAGE_SIZE)
#define IMAGE_B_SIZE           IMAGE_SIZE

/* imageIAP���� */
#define IMAGE_IAP_FLAG         0x03
#define IMAGE_IAP_START_ADD    (IMAGE_B_START_ADD + IMAGE_SIZE)
#define IMAGE_IAP_SIZE         12 * 1024

#define IAP_STATE_SUCCESS      0x00               // IAP�ɹ�
#define IAP_STATE_RETRAN       0xFE               // IAP�ش�
#define IAP_STATE_FAILURE      0xFF               // IAPʧ��

/* IAP���� */
/* ����ΪIAP��������� */
#define CMD_HAND_SHAKE         0x5A               // ��������
#define CMD_HAND_SHAKE_ACK     0xA5               // ����ACK

#define CMD_IAP_PROM           0x80               // IAP�������
#define CMD_IAP_ERASE          0x81               // IAP��������
#define CMD_IAP_VERIFY         0x82               // IAPУ������
#define CMD_IAP_END            0x83               // IAP������־
#define CMD_IAP_INFO           0x84               // IAPѡ��̼���ȡ�豸��Ϣ
#define CMD_IAP_INFO_ACK       0x04               // IAPѡ��̼�ACK
#define CMD_SINGLE_CHANNEL     0xC0               // ���ز�����
#define CMD_SINGLE_POWER       0xC1               // ���ز�����
#define CMD_CAPACITANCE        0xC2               // ���ص���
#define CMD_RESTART            0xE0               // ��������
#define CMD_CONFIG_VID_PID     0xE1               // VID_PID����
#define CMD_CONFIG_MANU_INFO    0xE2               // MANU_INFO����
#define CMD_CONFIG_PROD_INFO    0xE3               // PROD_INFO����
#define CMD_CONFIG_SERIAL_NUM   0xE4               // SERIAL_NUM����

#define CMD_STATE_ACK          0x0F               // ״̬ACK

#define HEX_NOT_LOCAL          0x01

/* ����֡���ȶ��� */
#define IAP_LEN                68

/* �����DataFlash��ַ������ռ��������λ�� */
#define OTA_DATAFLASH_ADD      0x00077000 - FLASH_ROM_MAX_SIZE

/* �����DataFlash���OTA��Ϣ */
typedef struct
{
    unsigned char ImageFlag; //��¼�ĵ�ǰ��image��־
    unsigned char Revd[3];
} OTADataFlashInfo_t;

/* OTA IAPͨѶЭ�鶨�� */
/* ��ַʹ��4��ƫ�� */
typedef union
{
    struct
    {
        unsigned char cmd;          /* ������ 0x5A */
        unsigned char len;          /* �������ݳ��� */
        unsigned char string[7];    /* WCH@IAP */
    } handshake; /* �������� */
    struct
    {
        unsigned char cmd;          /* ������ 0x81 */
        unsigned char len;          /* �������ݳ��� */
        unsigned char addr[2];      /* ������ַ */
        unsigned char block_num[2]; /* �������� */

    } erase; /* �������� */
    struct
    {
        unsigned char cmd;       /* ������ 0x83 */
        unsigned char len;       /* �������ݳ��� */
        unsigned char status[2]; /* ���ֽ�״̬������ */
    } end;                       /* �������� */
    struct
    {
        unsigned char cmd;              /* ������ 0x82 */
        unsigned char len;              /* �������ݳ��� */
        unsigned char addr[2];          /* У���ַ */
        unsigned char buf[IAP_LEN - 4]; /* У������ */
    } verify;                           /* У������ */
    struct
    {
        unsigned char cmd;              /* ������ 0x80 */
        unsigned char len;              /* �������ݳ��� */
        unsigned char addr[2];          /* ��ַ */
        unsigned char buf[IAP_LEN - 4]; /* �������� */
    } program;                          /* ������� */
    struct
    {
        unsigned char cmd;              /* ������ 0x84 */
        unsigned char len;              /* �������ݳ��� */
        unsigned char hex[4];          /* �̼��� */
    } info;                             /* ������� */
    struct
    {
        unsigned char buf[IAP_LEN]; /* �������ݰ�*/
    } other;
} OTA_IAP_CMD_t;

/* ��¼��ǰ��Image */
extern unsigned char CurrImageFlag;
extern uint8_t ota_send_buf[];
void OTA_Init(void);

void OTA_Enable(void);

uint8_t OTA_IAPWriteData(unsigned char *p_data, unsigned char w_len);

void OTA_IAP_SendData(void);

#endif
