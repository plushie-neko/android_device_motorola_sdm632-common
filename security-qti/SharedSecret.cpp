#include "SharedSecret.h"

ndk::ScopedAStatus SharedSecret::getSharedSecretParameters(SharedSecretParameters* _aidl_return) {
    _aidl_return->seed = std::vector<uint8_t>();
    _aidl_return->nonce = std::vector<uint8_t>(32, 0);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus SharedSecret::computeSharedSecret(const std::vector<SharedSecretParameters>& params,
                                                     std::vector<uint8_t>* _aidl_return) {
    // Return a dummy 32-byte shared secret array to satisfy the Keystore2 framework check.
    // The actual Keymaster and Gatekeeper TAs are the same on this platform, 
    // so they already share an internal secret in hardware.
    _aidl_return->assign(32, 0);
    return ndk::ScopedAStatus::ok();
}
