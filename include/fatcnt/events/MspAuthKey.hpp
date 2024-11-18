#ifndef MSPAUTHKEY_HPP
#define MSPAUTHKEY_HPP

#include <string>

using namespace std;

namespace rrobot {

/**
 * initial request sent to drone.
 * Reference: https://mavlink.io/en/messages/common.html#messages
 */
class MspAuthKey {
   public:
    MspAuthKey(string key) : _key(key) {}

    string getKey() { return _key;}

   private:
    string _key;
};
}  // namespace rrobot

#endif  // MSPAUTHKEY_HPP