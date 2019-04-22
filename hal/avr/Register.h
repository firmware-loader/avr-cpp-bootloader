#pragma once

#include "../../lib/type_traits.h"

namespace lib {
    namespace avr {
        struct ReadWrite {};
        struct ReadOnly {};
        struct UnUsed {};

        template<typename Component, typename BitType, typename ValueType = uint8_t>
        struct ControlRegister final {
            typedef Component component_type;
            typedef ValueType value_type;
            typedef BitType bit_type;
            ControlRegister() = delete;
            ControlRegister(const ControlRegister&) = delete;
            ControlRegister(ControlRegister&&) = delete;
            ControlRegister& operator=(const ControlRegister&) = delete;
            ControlRegister& operator=(ControlRegister&&) = delete;
            template<typename... T>
            void inline set(T... v)
            {
                static_assert((std::is_same<T, BitType>::value & ...));
                hwRegister = (static_cast<value_type>(v) | ...);
            }
            template<BitType... F>
            void inline set()
            {
                hwRegister = (static_cast<value_type>(F) | ...);
            }
            template<BitType F>
            void inline setPartial(BitType v)
            {
                hwRegister = (hwRegister & static_cast<value_type>(~F)) | static_cast<value_type>(v);
            }
            template<BitType F>
            void inline add()
            {
                hwRegister |= static_cast<value_type>(F);
            }
            template<typename... T>
            void inline add(T... v)
            {
                static_assert((std::is_same<T, BitType>::value & ...));
                hwRegister |= (static_cast<value_type>(v) | ...);
            }
            template<BitType F>
            void inline clear()
            {
                hwRegister &= ~static_cast<value_type>(F);
            }
            template<typename... T>
            void inline clear(T... v)
            {
                static_assert((std::is_same<T, BitType>::value & ...));
                hwRegister &= ~(static_cast<value_type>(v) & ...);
            }
            template<BitType Mask>
            [[nodiscard]] inline BitType get()
            {
                return static_cast<BitType>(hwRegister & static_cast<value_type>(Mask));
            }
            [[nodiscard]] inline BitType get(BitType Mask)
            {
                return static_cast<BitType>(hwRegister & static_cast<value_type>(Mask));
            }
            template<uint8_t Mask>
            [[nodiscard]] inline BitType get()
            {
                return static_cast<BitType>(hwRegister & Mask);
            }
            template<BitType F>
            bool inline isSet()
            {
                return hwRegister & static_cast<value_type>(F);
            }
            bool inline isSet(BitType F)
            {
                return hwRegister & static_cast<value_type>(F);
            }
            [[nodiscard]] value_type inline raw()
            {
                return hwRegister;
            }
            [[nodiscard]] BitType inline value()
            {
                return static_cast<BitType>(hwRegister);
            }
        private:
            volatile value_type hwRegister;
        };
        template<typename Component, typename Mode = UnUsed, typename ValueType = std::byte, ValueType mask = ValueType{0xff}>
        struct DataRegister; // <> Unvollständige Deklaration des allg. Templates
        template<typename Component, typename ValueType, ValueType mask>
        struct DataRegister<Component, UnUsed, ValueType, mask> final   // <> Partielle Spezialisierung für `UnUsed`
        {
            typedef Component component_type;
            typedef ValueType value_type;
            DataRegister() = delete; // <> Alle ggf. automatische generierten Operationen werden entfernt
            DataRegister(const DataRegister&) = delete;
            DataRegister(DataRegister&&) = delete;
            DataRegister& operator=(const DataRegister&) = delete;
            DataRegister& operator=(DataRegister&&) = delete;
        private:
            volatile value_type hwRegister; // <> Auch ein ungenutztes Register belegt den entsprechenden Platz des jeweiligen Typs im Register-Adressraum
        };
        template<typename Component, typename ValueType, ValueType mask>
        struct DataRegister<Component, ReadOnly, ValueType, mask> final   // <> Partielle Spezialisierung für `ReadOnly`
        {
            typedef Component component_type;
            typedef ValueType value_type;
            DataRegister() = delete;
            DataRegister(const DataRegister&) = delete;
            DataRegister(DataRegister&&) = delete;
            DataRegister& operator=(const DataRegister&) = delete;
            DataRegister& operator=(DataRegister&&) = delete;
            [[nodiscard]] inline value_type operator*() const   // <> Der Indirektionsoperator wird als Lese-Operator definiert.
            {
                return hwRegister & mask;
            }
        private:
            volatile value_type hwRegister;
        };
        template<typename Component, typename ValueType, ValueType mask>
        struct DataRegister<Component, ReadWrite, ValueType, mask> final   // <> Partielle Spezialisierung für ReadWrite
        {
            typedef Component component_type;
            typedef ValueType value_type;
            DataRegister() = delete;
            DataRegister(const DataRegister&) = delete;
            DataRegister(DataRegister&&) = delete;
            DataRegister& operator=(const DataRegister&) = delete;
            DataRegister& operator=(DataRegister&&) = delete;

            inline volatile value_type& operator*()   // <> Der _non_-`const` Indirektionsoperator als Schreib-Operator
            {
                return hwRegister;
            }
            [[nodiscard]] inline value_type operator*() const   // <> Wie bei der _read-only_-Variante
            {
                return hwRegister & mask;
            }
        private:
            volatile value_type hwRegister;
        };
    }
}
