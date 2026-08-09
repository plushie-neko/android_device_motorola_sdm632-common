#include "KeyMint.h"
#include <android-base/logging.h>

ndk::ScopedAStatus KeyMint::getHardwareInfo(KeyMintHardwareInfo* info) {
    info->versionNumber = 100;
    info->securityLevel = SecurityLevel::TRUSTED_ENVIRONMENT;
    info->keyMintName = "Custom QTI KeyMint";
    info->keyMintAuthorName = "Antigravity";
    info->timestampTokenRequired = false;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus KeyMint::addRngEntropy(const std::vector<uint8_t>& data) {
    return ndk::ScopedAStatus::ok(); // Stub
}

ndk::ScopedAStatus KeyMint::generateKey(const std::vector<KeyParameter>& keyParams,
                               const std::optional<AttestationKey>& attestationKey,
                               KeyCreationResult* creationResult) {
    keymaster::GenerateKeyRequest req(keymaster::kDefaultMessageVersion);
    // Note: To fully map KeyParameter to AuthorizationSet, we'd iterate over keyParams.
    // For now, we rely on the framework to pass down valid tags and we forward them.
    // req.key_description.Reinitialize(keyParams); // Assuming some conversion helper exists or we map manually

    size_t req_size = req.SerializedSize() + 4;
    size_t resp_size = 8192; // Max response size

    if (req_size > mHandle->sbuf_len || resp_size > mHandle->sbuf_len) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);
    }

    uint8_t* req_buf = mHandle->ion_sbuffer;
    uint32_t* cmd = reinterpret_cast<uint32_t*>(req_buf);
    *cmd = 0x108; // GENERATE_KEY
    req.Serialize(req_buf + 4, req_buf + req_size);

    uint8_t* resp_buf = req_buf + req_size;
    int ret = QseeComWrapper::send_cmd(mHandle, req_buf, req_size, resp_buf, resp_size);

    if (ret != 0) {
        return ndk::ScopedAStatus::fromServiceSpecificError(ErrorCode::UNKNOWN_ERROR);
    }

    keymaster::GenerateKeyResponse resp(keymaster::kDefaultMessageVersion);
    const uint8_t* resp_ptr = resp_buf;
    resp.Deserialize(&resp_ptr, resp_buf + resp_size);

    if (resp.error != keymaster::KM_ERROR_OK) {
        return ndk::ScopedAStatus::fromServiceSpecificError(static_cast<int32_t>(resp.error));
    }

    // Assign the resulting blob to the creation result
    creationResult->keyBlob.assign(resp.key_blob.key_material, resp.key_blob.key_material + resp.key_blob.key_material_size);
    // creationResult->keyCharacteristics = ... // Map resp.enforced and resp.unenforced back to KeyCharacteristics

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus KeyMint::importKey(const std::vector<KeyParameter>& keyParams,
                             KeyFormat keyFormat, const std::vector<uint8_t>& keyData,
                             const std::optional<AttestationKey>& attestationKey,
                             KeyCreationResult* creationResult) {
    return ndk::ScopedAStatus::fromServiceSpecificError(ErrorCode::UNIMPLEMENTED);
}

ndk::ScopedAStatus KeyMint::importWrappedKey(const std::vector<uint8_t>& wrappedKeyData,
                                    const std::vector<uint8_t>& wrappingKeyBlob,
                                    const std::vector<uint8_t>& maskingKey,
                                    const std::vector<KeyParameter>& unwrappingParams,
                                    int64_t passwordSid, int64_t biometricSid,
                                    KeyCreationResult* creationResult) {
    return ndk::ScopedAStatus::fromServiceSpecificError(ErrorCode::UNIMPLEMENTED);
}

ndk::ScopedAStatus KeyMint::upgradeKey(const std::vector<uint8_t>& keyBlobToUpgrade,
                              const std::vector<KeyParameter>& upgradeParams,
                              std::vector<uint8_t>* upgradedKeyBlob) {
    return ndk::ScopedAStatus::fromServiceSpecificError(ErrorCode::UNIMPLEMENTED);
}

ndk::ScopedAStatus KeyMint::deleteKey(const std::vector<uint8_t>& keyBlob) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus KeyMint::deleteAllKeys() {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus KeyMint::destroyAttestationIds() {
    return ndk::ScopedAStatus::fromServiceSpecificError(ErrorCode::UNIMPLEMENTED);
}

ndk::ScopedAStatus KeyMint::begin(KeyPurpose purpose, const std::vector<uint8_t>& keyBlob,
                         const std::vector<KeyParameter>& params,
                         const std::optional<HardwareAuthToken>& authToken,
                         BeginResult* result) {
    keymaster::BeginOperationRequest req(keymaster::kDefaultMessageVersion);
    req.purpose = static_cast<keymaster_purpose_t>(purpose);
    req.SetKeyMaterial(keyBlob.data(), keyBlob.size());
    // req.additional_params.Reinitialize(params);

    size_t req_size = req.SerializedSize() + 4;
    size_t resp_size = 8192;

    if (req_size > mHandle->sbuf_len) return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);

    uint8_t* req_buf = mHandle->ion_sbuffer;
    uint32_t* cmd = reinterpret_cast<uint32_t*>(req_buf);
    *cmd = 0x10f; // BEGIN
    req.Serialize(req_buf + 4, req_buf + req_size);

    uint8_t* resp_buf = req_buf + req_size;
    int ret = QseeComWrapper::send_cmd(mHandle, req_buf, req_size, resp_buf, resp_size);

    if (ret != 0) {
        return ndk::ScopedAStatus::fromServiceSpecificError(ErrorCode::UNKNOWN_ERROR);
    }

    keymaster::BeginOperationResponse resp(keymaster::kDefaultMessageVersion);
    const uint8_t* resp_ptr = resp_buf;
    resp.Deserialize(&resp_ptr, resp_buf + resp_size);

    if (resp.error != keymaster::KM_ERROR_OK) {
        return ndk::ScopedAStatus::fromServiceSpecificError(static_cast<int32_t>(resp.error));
    }

    result->challenge = resp.op_handle;
    // We would need to implement an IKeyMintOperation service here and return it
    // result->operation = ndk::SharedRefBase::make<KeyMintOperation>(mHandle, resp.op_handle);

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus KeyMint::deviceLocked(bool passwordOnly,
                                const std::optional<TimeStampToken>& timestampToken) {
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus KeyMint::earlyBootEnded() {
    return ndk::ScopedAStatus::ok();
}
