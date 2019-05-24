# cmake_minimum_required(VERSION <specify CMake version here>)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
SET(CMAKE_SYSTEM_NAME Generic)
# SET(CMAKE_SYSTEM Generic)
# SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS )
# SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
SET(CMAKE_CXX_STANDARD 20)

#============================================================================================
PROJECT(Bootloader CXX ASM)		  		# <-- Prjektname

SET(FILES main.cpp src/hal/avr/mcus/mega328/Mega328.h src/hal/avr/Register.h src/lib/type_traits.h src/abstraction/uart/AbstractUart.h src/concepts/McuCheck.h src/hal/avr/pin/Control.h src/hal/avr/utils/AvrUtils.h src/hal/avr/utils/Uart.h src/hal/MCUFamilies.h src/software/uart/SoftwareUart.h src/hal/avr/utils/Timer16.h src/hal/avr/mcus/mega328/PinMap.h src/abstraction/pins/PinControl.h src/hal/avr/utils/Timer8.h src/abstraction/timer/AbstractTimer.h src/utils/custom_limits.h src/utils/TypeTraits.h src/concepts/TypeCheck.h src/hal/avr/utils/bootloader/mega/Boot.h src/utils/Utility.h src/software/uart/TimerSoftwareUart.h src/software/uart/AssemblerSoftwareUart.h src/hal/concepts/Pin.h src/software/Literals.h src/software/uart/external/blink_test.S src/software/uart/external/BlinkTest.h src/software/uart/InlineAssemblerSoftwareUart.h)		  	# <-- Quelldatei(en)

SET(DEVICE atmega328p)			# <-- MCU
SET(FREQ 16000000)			# <-- MCU-Frequenz

SET(AVRBIN "/bin/gcc9/linux/bin/")		# <-- Toolchain
SET(AVROBJCOPY avr-objcopy)		# <--     ""
SET(AVRSIZE avr-size)			# <--     ""
SET(AVRDUDE avrdude)			# <--     ""

SET(PROGRAMMER usbasp)		# <-- Programmertype
SET(PORT usb)		         	# <-- USB bzw. Schnittstelle z.B. /dev/cu.usbserial....
# SET(BAUD 57600)	        	# <-- Übertragungsrate

SET(EEPROM NO)	     	        	# <-- soll eine x.eep Datei erstellt werden?
SET(FLASH NO)		        	# <-- soll Geflasht werden?
SET(ASM NO)

SET(BOOTPOSITION 0x7C00)

#============================================================================================

if(ASM)
    SET(ASM_FLAG "-S -fverbose-asm")
endif(ASM)

set_source_files_properties(src/software/uart/external/blink_test.S PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp ")

SET(CMAKE_C_COMPILER ${AVRBIN}/avr-gcc)
SET(CMAKE_CXX_COMPILER ${AVRBIN}/avr-g++)

SET(CMAKE_C_FLAGS  "-Os -mmcu=${DEVICE} ${ASM_FLAG} -DBOOT_BAUD_RATE=9600 -DF_CPU=${FREQ}UL -std=gnu99 -Wl,--gc-sections -Ttext=${BOOTPOSITION}")
SET(CMAKE_CXX_FLAGS "-Os -mmcu=${DEVICE} ${ASM_FLAG} -save-temps -DBOOT_BAUD_RATE=9600 -DF_CPU=${FREQ}UL -Wl,--gc-sections -fconcepts -Wall -Wextra -Wshadow -Wfloat-conversion -Wsign-conversion -Wsign-compare -Wmissing-field-initializers -fno-threadsafe-statics -Ttext=${BOOTPOSITION}")
SET(CMAKE_ASM_FLAGS  "-mmcu=${DEVICE} -save-temps")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
SET(CMAKE_PREFIX_PATH  "/bin/gcc9/linux/")
SET(CMAKE_EXECUTABLE_SUFFIX ".elf")
SET(CMAKE_VERBOSE_MAKEFILE ON)

set_property(DIRECTORY PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.hex"
        "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_PROJECT_NAME}.eep"
        )

include_directories("/bin/gcc9/linux/bin/include")

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