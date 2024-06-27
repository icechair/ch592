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
#define OTA_IAP_END_EVT         0x0010  //OTA_IAP_END_EVT
#define OTA_FLASH_PROM_EVT      0x0020  //OTA Flash
#define OTA_FLASH_VRIF_EVT      0x0040  //OTA Flash
#define OTA_RESTART_EVT         1<<8  //OTA RESTART

#define IAP_SAFE_FLAG       0x91600592

#define CHIP_ID             0x92
/* ------------------------------------------------------------------------------------------------
 *                                             OTA FLASH
 * ------------------------------------------------------------------------------------------------
 */

/* �����û�code���ֳ��Ŀ飬4K��216K��216K��12K������������ֱ����imageA��APP����imageB��OTA����imageIAP */

/* FLASH���� */
#define FLASH_BLOCK_SIZE       EEPROM_BLOCK_SIZE
#define IMAGE_SIZE             216 * 1024

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

/* IAP���� */
#define IAP_STATE_SUCCESS      0x00               // IAP�ɹ�
#define IAP_STATE_RETRAN       0x01               // IAP�ش�
#define IAP_STATE_FAILURE      0xFF               // IAPʧ��

/* ����ΪIAP��������� */
#define CMD_HAND_SHAKE         0x5A               // ��������
#define CMD_HAND_SHAKE_ACK     0xA5               // ����ACK

#define CMD_IAP_PROM           0x80               // IAP�������
#define CMD_IAP_ERASE          0x81               // IAP��������
#define CMD_IAP_VERIFY         0x82               // IAPУ������
#define CMD_IAP_END            0x83               // IAP������־
#define CMD_IAP_INFO           0x84               // IAP��ȡ�豸��Ϣ
#define CMD_IAP_INFO_ACK       0x04               // IAPѡ��̼�ACK

#define CMD_STATE_ACK          0x0F               // ״̬ACK

/* ����֡���ȶ��� */
#define IAP_LEN                64//36

#define IAP_SRC_RF             0x01
#define IAP_SRC_BLE            0x02
#define IAP_SRC_USB            0x03

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
        unsigned char buf[IAP_LEN - 2]; /* �������� */
    } info;                             /* ������� */
    struct
    {
        unsigned char buf[IAP_LEN]; /* �������ݰ�*/
    } other;
} OTA_IAP_CMD_t;

/* ��¼��ǰ��Image */
extern unsigned char CurrImageFlag;
extern uint8_t ota_taskID;
extern __attribute__((aligned(4))) uint8_t flash_buf[256];

void OTA_Init(void);

void OTA_Enable(void);
void OTA_USB_IAPWriteData(unsigned char *p_data, unsigned char w_len);

void OTA_RF_IAPWritData(unsigned char *p_data, unsigned char w_len);
#endif
