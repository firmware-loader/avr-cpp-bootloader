//
// Created by Sebastian on 31.07.2019.
//

#pragma once
#include <stdint.h>

static constexpr uint8_t UPDI_DEVICE_ID[3] = {0x1e, 0x95, 0x0f}; //atmega328p
static constexpr uint8_t UPDI_DEVICE_REVISION[1] = {0x01}; //Rev. B

static constexpr uint16_t UPDI_ADDRESS_OFFSET = 0x2000;

static constexpr uint8_t UPDI_BREAK = 0x00;

static constexpr uint8_t UPDI_LDS = 0x00;
static constexpr uint8_t UPDI_STS = 0x40;
static constexpr uint8_t UPDI_STCS = 0xC0; // (Store a value to Control/Status space)
static constexpr uint8_t UPDI_LDCS = 0x80; // (Load data from Control/Status space)
static constexpr uint8_t UPDI_KEY = 0xE0;
static constexpr uint8_t UPDI_LD = 0x20;
static constexpr uint8_t UPDI_ST = 0x60;
static constexpr uint8_t UPDI_REPEAT = 0xA0;

static constexpr uint8_t UPDI_KEY_SIB = 0x04;

static constexpr uint8_t UPDI_KEY_64 = 0x00;
static constexpr uint8_t UPDI_KEY_128 = 0x01;

static constexpr uint8_t UPDI_SIB_8BYTES = UPDI_KEY_64;
static constexpr uint8_t UPDI_SIB_16BYTES = UPDI_KEY_128;

// CS and ASI Register Address map
static constexpr uint8_t UPDI_CS_CTRLA = 0x02;
static constexpr uint8_t UPDI_CS_CTRLB = 0x03;

static constexpr uint8_t UPDI_CS_STATUSA = 0x00;
static constexpr uint8_t UPDI_CS_STATUSB = 0x01;
static constexpr uint8_t UPDI_ASI_SYS_STATUS = 0x0B;

static constexpr uint8_t UPDI_ASI_SYS_STATUS_RSTSYS = 5;
static constexpr uint8_t UPDI_ASI_SYS_STATUS_INSLEEP = 4;
static constexpr uint8_t UPDI_ASI_SYS_STATUS_NVMPROG = 3;
static constexpr uint8_t UPDI_ASI_SYS_STATUS_UROWPROG = 2;
static constexpr uint8_t UPDI_ASI_SYS_STATUS_LOCKSTATUS = 0;

static constexpr uint8_t UPDI_PHY_SYNC = 0x55;
static constexpr uint8_t UPDI_PHY_ACK = 0x40;

static constexpr uint8_t UPDI_PTR = 0x00;
static constexpr uint8_t UPDI_PTR_INC = 0x04;
static constexpr uint8_t UPDI_PTR_ADDRESS = 0x08;

static constexpr uint8_t UPDI_ADDRESS_8 = 0x00;
static constexpr uint8_t UPDI_ADDRESS_16 = 0x04;

static constexpr uint8_t UPDI_DATA_8 = 0x00;
static constexpr uint8_t UPDI_DATA_16 = 0x01;

static constexpr uint8_t UPDI_REPEAT_BYTE = 0x00;
static constexpr uint8_t UPDI_REPEAT_WORD = 0x01;


//CTRLA
static constexpr uint8_t UPDI_NVMCTRL_CTRLA_NOP = 0x00;
static constexpr uint8_t UPDI_NVMCTRL_CTRLA_WRITE_PAGE = 0x01;
static constexpr uint8_t UPDI_NVMCTRL_CTRLA_ERASE_PAGE = 0x02;
static constexpr uint8_t UPDI_NVMCTRL_CTRLA_ERASE_WRITE_PAGE = 0x03;
static constexpr uint8_t UPDI_NVMCTRL_CTRLA_PAGE_BUFFER_CLR = 0x04;
static constexpr uint8_t UPDI_NVMCTRL_CTRLA_CHIP_ERASE = 0x05;
static constexpr uint8_t UPDI_NVMCTRL_CTRLA_ERASE_EEPROM = 0x06;
static constexpr uint8_t UPDI_NVMCTRL_CTRLA_WRITE_FUSE = 0x07;