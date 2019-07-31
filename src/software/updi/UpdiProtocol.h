//
// Created by Sebastian on 31.07.2019.
//

#pragma once

#include "AbstractSoftwareUPDI.h"
#include "constants/UPDIConstants.h"

using namespace lib::software::literals;
namespace lib::software {
    struct UPDIStore {
        int16_t ptr = 0;
        int16_t repeats = 0;
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
                                uint8_t lsb = softUPDI::getByteWithoutSync();
                                uint8_t msb = softUPDI::getByteWithoutSync();
                                store.ptr = (msb << 8) | lsb;
                                store.repeats = 1;
                            } else {
                                // 8 Bit
                                store.ptr = softUPDI::getByte();
                            }
                        }
                        softUPDI::sendChar(UPDI_PHY_ACK);
                        break;
                    case UPDI_STS: {
                        if (data & UPDI_ADDRESS_16) {
                            uint8_t lsb = softUPDI::getByteWithoutSync();
                            uint8_t msb = softUPDI::getByteWithoutSync();
                        }
                        softUPDI::sendChar(UPDI_PHY_ACK);

                        uint8_t flash_cmd = softUPDI::getByteWithoutSync();
                        if (flash_cmd == UPDI_NVMCTRL_CTRLA_CHIP_ERASE) {
                            //TODO: Erase Chip
                            softUPDI::sendChar(UPDI_PHY_ACK);
                        } else if (flash_cmd == UPDI_NVMCTRL_CTRLA_PAGE_BUFFER_CLR) {
                            //TODO: Clear Buffer
                            softUPDI::sendChar(UPDI_PHY_ACK);
                        }
                        break;
                    }
                    case UPDI_LD:
                        if(data & UPDI_PTR_INC) {
                            if(!(data & UPDI_DATA_16)) {
                                auto ptr_data = lookup_ptr();
                                for(uint8_t i = 0; i < store.repeats; i++) {
                                    softUPDI::sendChar(ptr_data[i]);
                                }
                            }
                        }
                        break;
                    case UPDI_LDS:
                        if(data & UPDI_ADDRESS_16) {
                            uint8_t lsb = softUPDI::getByteWithoutSync();
                            uint8_t msb = softUPDI::getByteWithoutSync();
                        }
                        softUPDI::sendChar(0x0);
                        break;
                    case UPDI_REPEAT:
                        if(data & UPDI_REPEAT_WORD) {
                            // data is word
                            uint8_t lsb = softUPDI::getByteWithoutSync();
                            uint8_t msb = softUPDI::getByteWithoutSync();
                            store.repeats = ((msb << 8) | lsb) + 1;
                        } else {
                            // data is byte
                            store.repeats = softUPDI::getByteWithoutSync() + 1;
                        }
                        break;
                }
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


