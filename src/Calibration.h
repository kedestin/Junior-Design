#ifndef JD_BOT_CALLIBRATION
#define JD_BOT_CALLIBRATION

#include <EEPROM.h>

#include "Vector3.h"

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
#define CUMULATIVE_ENUM(name, ...)                                          \
        enum class name {                                                   \
                EXPAND(DEFER(CUMULATE_FIELD)(0, __VA_ARGS__ END_OFFSET, 1)) \
        }

// Utility macros for proper deferal and reevaluation
#define EMPTY()
#define DEFER(id) id EMPTY()
#define EXPAND(...) __VA_ARGS__

// Generates an entry for enum
#define CUMULATE_FIELD(previous, curr, size) curr = (previous) + (size)

// Macro that expects to be placed after "ADD_FIELD",
// "DEFER(CUMULATIVE_FIELD)(name"  , or the first argument of "CUMULATIVE_ENUM"
#define ADD_FIELD(name, size) name, size), DEFER(CUMULATE_FIELD)(name,

namespace JD {

struct Calibration {
        CUMULATIVE_ENUM(Offset,
                        ADD_FIELD(ColorBlack, sizeof(Vector3))
                            ADD_FIELD(ColorYellow, sizeof(Vector3))
                                ADD_FIELD(ColorRed, sizeof(Vector3))
                                    ADD_FIELD(ColorBlue, sizeof(Vector3)));


        // Todo add some error checking to make sure that
        // sizeof(data) == size from ADD_FIELD
        template <class T>
        static void set(Offset o, const T& data){
                EEPROM.put(o, data);
        }

        // Todo add some error checking to make sure that
        // sizeof(data) == size from ADD_FIELD
        template <class T>
        static void get(Offset o, T& data){
                EEPROM.get(o, data);
        }
};
}  // namespace JD
#endif
