//
// Created by Sebastian on 31.07.2019.
//

#pragma once

#include "AbstractSoftwareUPDI.h"
#include "constants/UPDIConstants.h"

using namespace lib::software::literals;
namespace lib::software {
    template<typename mcu, auto pinNumber>
    class UPDIProtocol {
    private:
        using softUPDI = lib::software::AbstractSoftwareUPDI<mcu, pinNumber, lib::software::SoftUPDIMethod::UPDITimingBased>;

        [[noreturn]] static constexpr void (*start)() = 0x0000;

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
                        softUPDI::getByte();
                        break;
                    case UPDI_LDCS:
                        switch (data & 0x0F) {
                            case UPDI_CS_STATUSA:
                                softUPDI::sendChar(0x30); // ?
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
                }
            }
        }
    };
}



