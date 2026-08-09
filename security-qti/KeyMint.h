#pragma once
#include <aidl/android/hardware/security/keymint/BnKeyMintDevice.h>
#include <aidl/android/hardware/security/keymint/BnKeyMintOperation.h>
#include <keymaster/android_keymaster_messages.h>
#include "QseeComWrapper.h"

using namespace aidl::android::hardware::security::keymint;

class KeyMint : public BnKeyMintDevice {
    QSEECom_handle* mHandle;
public:
    KeyMint(QSEECom_handle* handle) : mHandle(handle) {}

    ndk::ScopedAStatus getHardwareInfo(KeyMintHardwareInfo* info) override;
    ndk::ScopedAStatus addRngEntropy(const std::vector<uint8_t>& data) override;
    ndk::ScopedAStatus generateKey(const std::vector<KeyParameter>& keyParams,
                                   const std::optional<AttestationKey>& attestationKey,
                                   KeyCreationResult* creationResult) override;
    ndk::ScopedAStatus importKey(const std::vector<KeyParameter>& keyParams,
                                 KeyFormat keyFormat, const std::vector<uint8_t>& keyData,
                                 const std::optional<AttestationKey>& attestationKey,
                                 KeyCreationResult* creationResult) override;
    ndk::ScopedAStatus importWrappedKey(const std::vector<uint8_t>& wrappedKeyData,
                                        const std::vector<uint8_t>& wrappingKeyBlob,
                                        const std::vector<uint8_t>& maskingKey,
                                        const std::vector<KeyParameter>& unwrappingParams,
                                        int64_t passwordSid, int64_t biometricSid,
                                        KeyCreationResult* creationResult) override;
    ndk::ScopedAStatus upgradeKey(const std::vector<uint8_t>& keyBlobToUpgrade,
                                  const std::vector<KeyParameter>& upgradeParams,
                                  std::vector<uint8_t>* upgradedKeyBlob) override;
    ndk::ScopedAStatus deleteKey(const std::vector<uint8_t>& keyBlob) override;
    ndk::ScopedAStatus deleteAllKeys() override;
    ndk::ScopedAStatus destroyAttestationIds() override;
    ndk::ScopedAStatus begin(KeyPurpose purpose, const std::vector<uint8_t>& keyBlob,
                             const std::vector<KeyParameter>& params,
                             const std::optional<HardwareAuthToken>& authToken,
                             BeginResult* result) override;
    ndk::ScopedAStatus deviceLocked(bool passwordOnly,
                                    const std::optional<TimeStampToken>& timestampToken) override;
    ndk::ScopedAStatus earlyBootEnded() override;
};
