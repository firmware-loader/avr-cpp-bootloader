# cmake_minimum_required(VERSION <specify CMake version here>)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
SET(CMAKE_SYSTEM_NAME Generic)
# SET(CMAKE_SYSTEM Generic)
# SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS )
# SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
SET(CMAKE_CXX_STANDARD 20)

#============================================================================================
PROJECT(Bootloader)		  		# <-- Prjektname

SET(FILES main.cpp hal/avr/mcus/mega328/Mega328.h hal/avr/Register.h lib/type_traits.h software/uart/SoftwareUart.h concepts/McuCheck.h hal/avr/pin/Control.h hal/avr/utils/AvrUtils.h hal/avr/utils/Uart.h hal/AdcFamilies.h)		  	# <-- Quelldatei(en)

SET(DEVICE atmega328p)			# <-- MCU
SET(FREQ 1000000)			# <-- MCU-Frequenz

SET(AVRBIN "/home/sebastian/avr-gcc/avr-gcc-8.3.0-x64-linux/bin/")		# <-- Toolchain
SET(AVROBJCOPY avr-objcopy)		# <--     ""
SET(AVRSIZE avr-size)			# <--     ""
SET(AVRDUDE avrdude)			# <--     ""

SET(PROGRAMMER usbasp)		# <-- Programmertype
SET(PORT usb)		         	# <-- USB bzw. Schnittstelle z.B. /dev/cu.usbserial....
# SET(BAUD 57600)	        	# <-- Übertragungsrate

SET(EEPROM NO)	     	        	# <-- soll eine x.eep Datei erstellt werden?
SET(FLASH NO)		        	# <-- soll Geflasht werden?
SET(ASM NO)

#============================================================================================

if(ASM)
    SET(ASM_FLAG "-S -fverbose-asm")
endif(ASM)

SET(CMAKE_C_COMPILER ${AVRBIN}/avr-gcc)
SET(CMAKE_CXX_COMPILER ${AVRBIN}/avr-g++)

SET(CMAKE_C_FLAGS  "-Os -mmcu=${DEVICE} ${ASM_FLAG} -DBOOT_BAUD_RATE=9600 -DF_CPU=${FREQ}UL -std=gnu99 -Wl,--gc-sections")
SET(CMAKE_CXX_FLAGS "-Os -mmcu=${DEVICE} ${ASM_FLAG} -DBOOT_BAUD_RATE=9600 -DF_CPU=${FREQ}UL -Wl,--gc-sections -fconcepts -Wall -Wextra")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(CMAKE_PREFIX_PATH  "/home/sebastian/avr-gcc/avr-gcc-8.3.0-x64-linux/")
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.hex"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.eep"
        )

include_directories("/home/sebastian/avr-gcc/avr-gcc-8.3.0-x64-linux/bin//include")

SET(SOURCE_FILES ${FILES})

#============================================================================================
#SET_TARGET_PROPERTIES(${TARGET} PROPERTIES COMPILE_FLAGS "-save-temps -S")
ADD_EXECUTABLE(${CMAKE_PROJECT_NAME} ${FILES})

ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${AVRBIN}/${AVROBJCOPY} -O ihex -R .eeprom ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.hex)

#if(EEPROM)
#    ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${AVRBIN}/${AVROBJCOPY} -O ihex -j .eeprom --set-section-flags=.eeprom="alloc,load"  --change-section-lma .eeprom=0 ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.elf ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.eep)
#endif(EEPROM)

ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND ${AVRBIN}/${AVRSIZE} --target=ihex ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.hex)


if(FLASH)
#    ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND bash -c "${AVRDUDE} -p${DEVICE} -c${PROGRAMMER} -P${PORT} -U flash:w:${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.hex")
# ADD_CUSTOM_COMMAND(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD COMMAND cmd.exe /C ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/flash.bat )
endif(FLASH)