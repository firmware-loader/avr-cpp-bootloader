# cmake_minimum_required(VERSION <specify CMake version here>)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8.4)
SET(CMAKE_SYSTEM_NAME Generic)
# SET(CMAKE_SYSTEM Generic)
# SET(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS )
# SET(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS)
SET(CMAKE_CXX_STANDARD 20)

#============================================================================================
PROJECT(Bootloader CXX ASM)		  		# <-- Prjektname

SET(FILES main.cpp src/hal/avr/mcus/mega328/Mega328.h src/hal/avr/Register.h src/lib/type_traits.h src/abstraction/uart/AbstractUart.h src/concepts/McuCheck.h src/hal/avr/pin/Control.h src/hal/avr/utils/AvrUtils.h src/hal/avr/utils/Uart.h src/hal/MCUFamilies.h src/software/uart/implementation/SoftwareUart.h src/hal/avr/utils/Timer16.h src/hal/avr/mcus/mega328/PinMap.h src/abstraction/pins/PinControl.h src/hal/avr/utils/Timer8.h src/abstraction/timer/AbstractTimer.h src/utils/custom_limits.h src/utils/TypeTraits.h src/concepts/TypeCheck.h src/hal/avr/utils/bootloader/mega/Boot.h src/hal/avr/utils/bootloader/mega/UpdiBootloader src/utils/Utility.h src/software/uart/implementation/TimerSoftwareUart.h src/software/uart/implementation/vendorspecific/AssemblerSoftwareUart.h src/hal/concepts/Pin.h src/software/Literals.h src/software/uart/implementation/vendorspecific/external/Sync.h src/software/uart/implementation/vendorspecific/external/Sync.S src/software/uart/implementation/vendorspecific/InlineAssemblerSoftwareUart.h src/utils/Array.cpp src/utils/Array.h src/software/uart/AbstractSoftwareUart.h src/software/uart/implementation/TimingBasedUart.h src/utils/TimingUtils.h src/software/updi/implementation/TimingBasedUPDI.h src/software/updi/AbstractSoftwareUPDI.h src/software/updi/implementation/SoftwareUPDI.h src/software/updi/constants/UPDIConstants.h src/software/updi/UpdiProtocol.h src/hal/avr/mcus/tiny2313/tiny2313.h src/hal/avr/mcus/tiny2313/PinMap.h)		  	# <-- Quelldatei(en)

#SET(DEVICE atmega328p)			# <-- MCU
SET(DEVICE attiny2313)
SET(FREQ 8000000)			# <-- MCU-Frequenz

SET(AVRBIN "/bin/gcc9/linux/bin/")		# <-- Toolchain
SET(AVROBJCOPY avr-objcopy)		# <--     ""
SET(AVRSIZE avr-size)			# <--     ""
SET(AVRDUDE avrdude)			# <--     ""

SET(PROGRAMMER usbasp)		# <-- Programmertype
SET(PORT usb)		         	# <-- USB bzw. Schnittstelle z.B. /dev/cu.usbserial....
# SET(BAUD 57600)	        	# <-- Übertragungsrate

SET(EEPROM NO)	     	        	# <-- soll eine x.eep Datei erstellt werden?
SET(FLASH NO)		        	# <-- soll Geflasht werden?
SET(ASM YES)

#SET(BOOTPOSITION 0x7800)
SET(BOOTPOSITION 0x00)

SET(UPDI 1)
SET(UNIDIRECTIONAL 2)
SET(DEBUG 3)
SET(PROTOCOL ${UPDI})

#============================================================================================

if(ASM)
    SET(ASM_FLAG "-save-temps -g0 -ggdb0") #-fverbose-asm
endif(ASM)

#set_source_files_properties(src/software/uart/external/blink_test.S PROPERTIES COMPILE_FLAGS "-x assembler-with-cpp ")

SET(CMAKE_C_COMPILER ${AVRBIN}/avr-gcc)
SET(CMAKE_CXX_COMPILER ${AVRBIN}/avr-g++)

SET(CMAKE_C_FLAGS  "-Os -mmcu=${DEVICE} ${ASM_FLAG} -DBOOT_BAUD_RATE=9600 -DF_CPU=${FREQ}UL -std=gnu99 -Wl,--gc-sections -Ttext=${BOOTPOSITION}")
SET(CMAKE_CXX_FLAGS "-save-temps -Os -mmcu=${DEVICE} ${ASM_FLAG} -DBOOT_BAUD_RATE=9600 -DF_CPU=${FREQ}UL -Wl,--gc-sections -fconcepts -Wall -Wextra -Wshadow -Wfloat-conversion -Wsign-conversion -Wsign-compare -Wmissing-field-initializers -fno-threadsafe-statics -Ttext=${BOOTPOSITION}")
SET(CMAKE_ASM_FLAGS  "-mmcu=${DEVICE} -save-temps")

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
SET(CMAKE_PREFIX_PATH  "/bin/gcc9/linux/")
SET(CMAKE_EXECUTABLE_SUFFIX ".elf")
SET(CMAKE_VERBOSE_MAKEFILE ON)


add_compile_definitions(FLAG_UPDI=${UPDI})
add_compile_definitions(FLAG_UNIDIRECTIONAL=${UNIDIRECTIONAL})
add_compile_definitions(FLAG_DEBUG=${DEBUG})


if(PROTOCOL MATCHES ${UPDI} OR PROTOCOL MATCHES ${UNIDIRECTIONAL} OR PROTOCOL MATCHES ${DEBUG})
    add_compile_definitions(PROTOCOL=${PROTOCOL})
elseif()
    message(FATAL_ERROR "Only 'UPDI', 'DEBUG' or 'UNIDIRECTIONAL' are supported protocols!")
endif()


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