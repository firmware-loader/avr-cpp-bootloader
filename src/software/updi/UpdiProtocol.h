//
// Created by Sebastian on 31.07.2019.
//

#pragma once

#include <avr/pgmspace.h>

#include "AbstractSoftwareUPDI.h"
#include "constants/UPDIConstants.h"

using namespace lib::software::literals;
namespace lib::software {
    struct UPDIStore {
        uint16_t ptr = 0;
        uint16_t repeats = 0;
        bool flash_write_error = false;
    };


    template<typename mcu, auto pinNumber>
    class UPDIProtocol {
    private:
        using softUPDI = lib::software::AbstractSoftwareUPDI<mcu, pinNumber, lib::software::SoftUPDIMethod::UPDITimingBased>;

        [[noreturn]] static constexpr void (*start_pgm)() = 0x0000;
        static UPDIStore store;
    public:
        static void start() {

            softUPDI::template init<9600_baud, 9600_baud>();
            // initial 0x0
            if (!softUPDI::template gotSignalBeforeTimout<uint32_t>()) {
                start_pgm();
            }

            while(true) {
                uint8_t data = softUPDI::getByte();
                updi_state_machine(data);
            }
        }

        static void updi_state_machine(uint8_t data) {
            switch (data & 0xF0) {
                case UPDI_STCS: { //ignore next
                    uint8_t value = softUPDI::getByteWithoutSync();
                    switch (data & 0x0F) {
                        case UPDI_ASI_RESET_REQ:
                            if (value == UPDI_RESET_REQ_VALUE) {
                                start_pgm();
                            }
                    }
                    break;
                }
                case UPDI_LDCS:
                    switch (data & 0x0F) {
                        case UPDI_CS_STATUSA:
                            softUPDI::sendChar(0x30); // is this ack?
                            break;
                        case UPDI_ASI_SYS_STATUS:
                            sendStatusInfo();
                            break;
                    }
                    break;
                case UPDI_KEY:
                    switch (data & 0x0F) {
                        case UPDI_KEY_SIB | UPDI_SIB_16BYTES:
                            softUPDI::sendString(DEVICE_STRING);
                    }
                    break;
                case UPDI_ST:
                    if (data & UPDI_PTR_ADDRESS) {
                        if (data & UPDI_DATA_16) {
                            //16 Bit Data
                            store.ptr = getWordValue();
                            store.repeats = 1;
                        } else {
                            // 8 Bit
                            store.ptr = softUPDI::getByte();
                        }
                        softUPDI::sendChar(UPDI_PHY_ACK);
                    } else if (data & UPDI_PTR_INC) {
                        if (data & UPDI_DATA_16) {
                            if (store.ptr >= UPDI_ADDRESS_OFFSET) {
                                writeToFlashBuffer();
                            }
                        }
                    }
                    break;
                case UPDI_STS: {
                    if (data & UPDI_ADDRESS_16) {
                        getWordValue();
                    }
                    softUPDI::sendChar(UPDI_PHY_ACK);
                    executeSTSControlCommand();
                    break;
                }
                case UPDI_LD:
                    if (data & UPDI_PTR_INC) {
                        if (!(data & UPDI_DATA_16)) {
                            auto ptr_data = lookup_ptr();
                            for (uint8_t i = 0; i < store.repeats; i++) {
                                softUPDI::sendChar(ptr_data[i]);
                            }
                        } else {
                            if (store.ptr >= UPDI_ADDRESS_OFFSET) {
                                uint16_t start_address = store.ptr - UPDI_ADDRESS_OFFSET;
                                for (uint8_t i = 0; i < store.repeats * 2; i++) {
                                    softUPDI::sendChar((uint8_t)pgm_read_byte(start_address + i)); 
                                }
                            }
                        }
                    }
                    break;
                case UPDI_LDS:
                    if (data & UPDI_ADDRESS_16) {
                        store.ptr = getWordValue();
                        store.repeats = 1;
                        if(store.ptr == NVMCTRL_ADDRESS + UPDI_NVMCTRL_STATUS) {
                            if(store.flash_write_error) {
                                softUPDI::sendChar(1 << UPDI_NVM_STATUS_WRITE_ERROR);
                            } else {
                                if(boot_spm_busy()) {
                                    softUPDI::sendChar(1 << UPDI_NVM_STATUS_FLASH_BUSY);
                                } else {
                                    softUPDI::sendChar(UPDI_NVM_STATUS_READY);
                                    boot_rww_enable();
                                }
                            }
                        } else {
                            softUPDI::sendChar(0x0);
                        }
                    }
                    break;
                case UPDI_REPEAT:
                    if (data & UPDI_REPEAT_WORD) {
                        store.repeats = getWordValue() + 1;
                    } else {
                        store.repeats = softUPDI::getByteWithoutSync() + 1u;
                    }
                    break;
            }
        }

        void executeSTSControlCommand() {
            uint8_t flash_cmd = softUPDI::getByteWithoutSync();
            if (flash_cmd == UPDI_NVMCTRL_CTRLA_CHIP_ERASE) {
                //TODO: Erase Chip
            } else if (flash_cmd == UPDI_NVMCTRL_CTRLA_PAGE_BUFFER_CLR) {
                //TODO: Clear Page Buffer
            } else if (flash_cmd == UPDI_NVMCTRL_CTRLA_WRITE_PAGE) {
                uint16_t startAddress = store.ptr - UPDI_ADDRESS_OFFSET;
                boot_page_write(startAddress);
            }
            softUPDI::sendChar(UPDI_PHY_ACK);
        }

        static uint16_t getWordValue() {
            const uint8_t lsb = softUPDI::getByteWithoutSync();
            const uint8_t msb = softUPDI::getByteWithoutSync();
            return static_cast<uint16_t>(msb << 8u) | lsb;
        }

        static void writeToFlashBuffer() {
            //TODO: write data to tmp flash page register
            uint16_t startAddress = store.ptr - UPDI_ADDRESS_OFFSET;
            boot_page_erase(startAddress);
            boot_spm_busy_wait();
            for (uint8_t i = 0; i < store.repeats * 2; i+=2) {
                boot_page_fill(startAddress + i, getWordValue());
            }
        }

        static void sendStatusInfo() {
            //TODO: Fully add status Info
            //We are already in prog mode... or at least we say so
            constexpr uint8_t status = (1 << UPDI_ASI_SYS_STATUS_NVMPROG);
            softUPDI::sendChar(status);
        }

        static constexpr const uint8_t* lookup_ptr() {
            switch(store.ptr) {
                case 0x1100:
                    return UPDI_DEVICE_ID;
                case 0x0F01:
                    return UPDI_DEVICE_REVISION;
            }
            return nullptr;
        }
    };


    template<typename mcu, auto pinNumber>
    UPDIStore UPDIProtocol<mcu, pinNumber>::store;
}


