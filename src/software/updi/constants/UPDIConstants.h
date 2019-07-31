//
// Created by Sebastian on 31.07.2019.
//

#pragma once
#include <stdint.h>

static constexpr uint8_t UPDI_BREAK = 0x00;

static constexpr uint8_t UPDI_STCS = 0xC0; // (Store a value to Control/Status space)
static constexpr uint8_t UPDI_LDCS = 0x80; // (Load data from Control/Status space)
static constexpr uint8_t UPDI_KEY = 0xE0;

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