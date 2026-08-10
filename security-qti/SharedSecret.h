#pragma once

#include <aidl/android/hardware/security/sharedsecret/BnSharedSecret.h>

using namespace aidl::android::hardware::security::sharedsecret;

class SharedSecret : public BnSharedSecret {
public:
    ndk::ScopedAStatus getSharedSecretParameters(SharedSecretParameters* _aidl_return) override;
    ndk::ScopedAStatus computeSharedSecret(const std::vector<SharedSecretParameters>& params,
                                           std::vector<uint8_t>* _aidl_return) override;
};
