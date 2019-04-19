#ifndef JD_UPDATEABLE_H
#define JD_UPDATEABLE_H
namespace JD {
/**
 * @brief Class to provide update method to various classes
 *        Allows various objects to be updated within a loop
 *
 */
class Updateable {
public:
        virtual void update() = 0;
};
}  // namespace JD

#endif