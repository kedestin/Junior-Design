#ifndef JD_PROTOTHREAD_H
#define JD_PROTOTHREAD_H

#include <stdint.h>

/**
 * In this file
 *
 * * A C++ style implementation of protothreads using a class
 *
 *  " Protothreads are extremely lightweight stackless threads designed for
 *    severely memory constrained systems, such as small embedded systems or
 *    wireless sensor network nodes. Protothreads provide linear code execution
 *    for event-driven systems implemented in C. Protothreads can be used with
 *    or without an underlying operating system to provide blocking
 *    event-handlers. Protothreads provide sequential flow of control without
 *    complex state machines or full multi-threading. "
 *     -  http://dunkels.com/adam/pt/
 *
 *  * C style macros to actually write a protothread function
 * 
 * tldr: Protothreads are basically functions written within a switch
 * statement, where the switch variable is a line number (__LINE__) that is
 * stored between function calls 
 */
namespace JD {
class Protothread {
public:
        // Local continuation type (stores line numbers)
        using lc_t = uint16_t;
        // Protothread function type
        using protothread_t = void (*)(lc_t&);
        enum : lc_t { notRunning = (lc_t)-1 };

protected:
        // "Pointer to thread function for optional constructor"
        protothread_t _thread;

        // "Local continuation"
        // "Stores the location to resume thread"
        lc_t _lc;

public:
        // Construct a new protothread that will start from the beginning
        // of its run() function.
        Protothread() : _lc(0), _thread(nullptr) {}

        // Construct a protothread from a thread function
        Protothread(protothread_t new_thread) : Protothread() {
                _thread = new_thread;
        }

        // Disallow copy construction
        Protothread(const Protothread& p) = delete;

        // Restart the thread
        void restart() { _lc = 0; }
        // Stop the thread
        void stop() { _lc = notRunning; }
        // Check if thread is running
        bool isRunning() { return _lc != notRunning; }

        /**
         * @brief Allow shorthand to run thread and check if running
         *        i.e
         *
         *        while(thread)
         *              continue;
         *
         * @return true If thread is running
         * @return false False otherwise
         */
        operator bool() {
                run();
                return isRunning();
        }

        /**
         * @brief Runs the thread       
         *
         */
        void run() {
                if (_thread != nullptr)
                        _thread(_lc);
        }
};

// Sets local continuation to current line
// Note: Imperitive that macro is a single line

// clang-format off
#define LC_SET(_lc) _lc = __LINE__; case __LINE__:
// clang-format on

// Declare start of protothread (version to use if _lc is already declared).
#define PT_BEGIN_LC_ALREADY_DECLARED() \
        bool hasYielded = true;        \
        (void)hasYielded;              \
        switch (_lc) {                 \
                case 0:

// Allows a standalone function to be a prothread
#define PT_BEGIN()                            \
        static JD::Protothread::lc_t _lc = 0; \
        (void)_lc;                            \
        PT_BEGIN_LC_ALREADY_DECLARED()

#define PT_STOP() _lc = JD::Protothread::notRunning

// Ends the protothread
#define PT_END()  \
        default:; \
                } \
                // PT_STOP(); \
                // return;

// Waits until given condition is true
#define PT_WAIT_UNTIL(condition)  \
        do {                      \
                LC_SET(_lc);      \
                if (!(condition)) \
                        return;   \
        } while (0)

// Waits while given condition is true
#define PT_WAIT_WHILE(condition) PT_WAIT_UNTIL(!(condition))

// Runs child thread until completion
#define PT_SPAWN(child) PT_WAIT_WHILE(child)

// Restarts thread's local continuation to PT_BEGIN
#define PT_RESTART()     \
        do {             \
                _lc = 0; \
                return;  \
        } while (0)

// Stop and exit from protothread.
#define PT_EXIT()          \
        do {               \
                PT_STOP(); \
                return;    \
        } while (0)

// Blocks until thread is rescheduled (run called again)
#define PT_YIELD()                  \
        do {                        \
                hasYielded = false; \
                LC_SET(_lc);        \
                if (!hasYielded)    \
                        return;     \
        } while (0)

// Blocks until condition is true
#define PT_YIELD_UNTIL(condition)                \
        do {                                     \
                hasYielded = false;              \
                LC_SET(_lc);                     \
                if (!hasYielded || !(condition)) \
                        return;                  \
        } while (0)
}  // namespace JD

#endif