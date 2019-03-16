#ifndef JD_BOT_CALLIBRATION
#define JD_BOT_CALLIBRATION

#include <EEPROM.h>

#include "Vector.h"

/**
 * @brief A macro for defining an enum who's values increment by specified size
 *        Reduces potential for errors, and is more clear.
 *
 *
 *
 * @details ADD_FIELD      Sets up a list of macro invocations that are defered
 *                        using defer
 *          CUMULATE_FIELD
 *          EXPAND    Forces the preprocessor to make one more pass, to invoke
 *                    all the macro invocations, without painting them blue
 *
 *
 * @params name     The name of the enum
 *         ...      A list of ADD_FIELD macros
 *
 */

#define CUMULATIVE_ENUM(name, ...)                                           \
        enum name {                                                          \
                EXPAND(DEFER(CUMULATE_FIELD)(0, 0, __VA_ARGS__, END_OFFSET)) \
        }

// Utility macros for proper deferal and reevaluation
#define EMPTY()
#define DEFER(id) id EMPTY()
#define EXPAND(...) __VA_ARGS__

// Generates an entry for enum
#define CUMULATE_FIELD(previous, size, curr) curr = (previous) + (size)

// Macro that expects to be placed after "ADD_FIELD",
// "DEFER(CUMULATIVE_FIELD)(name"  , or the first argument of "CUMULATIVE_ENUM"
#define ADD_FIELD(name, size) name), DEFER(CUMULATE_FIELD)(name,size

namespace JD {

struct Calibration {
        // Enum of values stored in EEPROM
        // Each field is offset by the size of the previous one
        CUMULATIVE_ENUM(Offset, ADD_FIELD(ColorBlack, sizeof(Vector<3>)),
                        ADD_FIELD(ColorYellow, sizeof(Vector<3>)),
                        ADD_FIELD(ColorRed, sizeof(Vector<3>)),
                        ADD_FIELD(ColorBlue, sizeof(Vector<3>)));


        /**
         * @brief Sets the requested callibration value in the EEPROM
         * 
         * @note  This function does no error checking. Be warying that the
         *        of data is the same size as expected in the enum
         * @param o     The enum identifier of  data
         * @param data  The data to set
         */
        template <class T>
        static void set(Offset o, const T& data) {
                EEPROM.put(o, data);
        }

        /**
         * @brief Gets the requested callibration value in the EEPROM
         * 
         * @note  This function does no error checking. Be warying that the
         *        of data is the same size as expected in the enum
         * @param o     The enum identifier of  data
         * @param data  The data to get
         */
        template <class T>
        static void get(Offset o, T& data) {
                EEPROM.get(o, data);
        }
};
}  // namespace JD
#endif
