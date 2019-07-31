//
// Created by Sebastian on 31.07.2019.
//

#pragma once

#include "AbstractSoftwareUPDI.h"
#include "constants/UPDIConstants.h"

using namespace lib::software::literals;
namespace lib::software {
    struct UPDIStore {
        uint16_t ptr = 0;
        uint16_t repeats = 0;
    };

    template<typename mcu, auto pinNumber>
    class UPDIProtocol {
    private:
        using softUPDI = lib::software::AbstractSoftwareUPDI<mcu, pinNumber, lib::software::SoftUPDIMethod::UPDITimingBased>;

        [[noreturn]] static constexpr void (*start)() = 0x0000;
        static UPDIStore store;
    public:
        static void init() {

            softUPDI::template init<9600_baud, 9600_baud>();
            // initial 0x0
            if (!softUPDI::template gotSignalBeforeTimout<uint32_t>()) {
                start();
            }

            while(true) {
                uint8_t data = softUPDI::getByte();
                switch (data & 0xF0) {
                    case UPDI_STCS: //ignore next
                        softUPDI::getByteWithoutSync();
                        break;
                    case UPDI_LDCS:
                        switch (data & 0x0F) {
                            case UPDI_CS_STATUSA:
                                softUPDI::sendChar(0x30); // is this ack?
                                break;
                            case UPDI_ASI_SYS_STATUS:
                                // In Prog Mode |
                                //TODO: Fully add status Info
                                constexpr uint8_t status = (1 << UPDI_ASI_SYS_STATUS_NVMPROG);
                                softUPDI::sendChar(status);
                                break;
                        }
                        break;
                    case UPDI_KEY:
                        switch (data & 0x0F) {
                            case UPDI_KEY_SIB | UPDI_SIB_16BYTES:
                                softUPDI::sendString("_ATmega P:0D:1-");
                                softUPDI::sendChar(0x10);
                        }
                        break;
                    case UPDI_ST:
                        if(data & UPDI_PTR_ADDRESS) {
                            if(data & UPDI_DATA_16) {
                                //16 Bit Data
                                store.ptr = getWordValue();
                                store.repeats = 1;
                            } else {
                                // 8 Bit
                                store.ptr = softUPDI::getByte();
                            }
                            softUPDI::sendChar(UPDI_PHY_ACK);
                        } else if(data & UPDI_PTR_INC) {
                            if(data & UPDI_DATA_16) {
                                if(store.ptr >= UPDI_ADDRESS_OFFSET) {
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

                        uint8_t flash_cmd = softUPDI::getByteWithoutSync();
                        if (flash_cmd == UPDI_NVMCTRL_CTRLA_CHIP_ERASE) {
                            //TODO: Erase Chip
                        } else if (flash_cmd == UPDI_NVMCTRL_CTRLA_PAGE_BUFFER_CLR) {
                            //TODO: Clear Buffer
                        } else if(flash_cmd == UPDI_NVMCTRL_CTRLA_WRITE_PAGE) {
                            //TODO: Write Page
                        }
                        softUPDI::sendChar(UPDI_PHY_ACK);
                        break;
                    }
                    case UPDI_LD:
                        if(data & UPDI_PTR_INC) {
                            if(!(data & UPDI_DATA_16)) {
                                auto ptr_data = lookup_ptr();
                                for(uint8_t i = 0; i < store.repeats; i++) {
                                    softUPDI::sendChar(ptr_data[i]);
                                }
                            } else {
                                if(store.ptr >= UPDI_ADDRESS_OFFSET) {
                                    // TODO: send back flash
                                    for(uint8_t i = 0; i < store.repeats * 2; i++) {
                                        softUPDI::sendChar(i);
                                    }
                                }
                            }
                        }
                        break;
                    case UPDI_LDS:
                        if(data & UPDI_ADDRESS_16) {
                            getWordValue();
                        }
                        softUPDI::sendChar(0x0);
                        break;
                    case UPDI_REPEAT:
                        if(data & UPDI_REPEAT_WORD) {
                            // data is word
                            store.repeats = getWordValue() + 1;
                        } else {
                            // data is byte
                            store.repeats = softUPDI::getByteWithoutSync() + 1u;
                        }
                        break;
                }
            }
        }

        static uint16_t getWordValue() {
            const uint8_t lsb = softUPDI::getByteWithoutSync();
            const uint8_t msb = softUPDI::getByteWithoutSync();
            return static_cast<uint16_t>(msb << 8u) | lsb;
        }

        static void writeToFlashBuffer() {
            //TODO: write data to tmp flash page register
            for (uint8_t i = 0; i < store.repeats * 2; i++) {
                uint16_t realAddr = store.ptr - UPDI_ADDRESS_OFFSET;
                softUPDI::getByteWithoutSync();
            }
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


