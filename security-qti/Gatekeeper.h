#pragma once
#include <aidl/android/hardware/gatekeeper/BnGatekeeper.h>
#include "QseeComWrapper.h"

using namespace aidl::android::hardware::gatekeeper;

struct __attribute__((packed)) qti_gk_verify_req {
    uint32_t cmd_id;             // 0x1002
    uint32_t uid;
    uint64_t challenge;
    uint32_t pwd_handle_offset;
    uint32_t pwd_handle_length;
    uint32_t provided_pwd_offset;
    uint32_t provided_pwd_length;
};

struct __attribute__((packed)) qti_gk_resp {
    int32_t  status;
    uint32_t payload_offset;
    uint32_t payload_length;
};

struct __attribute__((packed)) qti_gk_enroll_req {
    uint32_t cmd_id;             // 0x1001
    uint32_t uid;
    uint32_t current_pwd_handle_offset;
    uint32_t current_pwd_handle_length;
    uint32_t current_pwd_offset;
    uint32_t current_pwd_length;
    uint32_t desired_pwd_offset;
    uint32_t desired_pwd_length;
};

struct __attribute__((packed)) qti_gk_clear_user_req {
    uint32_t cmd_id;             // 0x1003
    uint32_t uid;
};

struct __attribute__((packed)) qti_gk_clear_all_req {
    uint32_t cmd_id;             // 0x1004
};

class Gatekeeper : public BnGatekeeper {
    QSEECom_handle* mHandle;
public:
    Gatekeeper(QSEECom_handle* handle) : mHandle(handle) {}

    ndk::ScopedAStatus enroll(
        int32_t uid,
        const std::vector<uint8_t>& currentPasswordHandle,
        const std::vector<uint8_t>& currentPassword,
        const std::vector<uint8_t>& desiredPassword,
        GatekeeperEnrollResponse* _aidl_return) override;

    ndk::ScopedAStatus verify(
        int32_t uid, int64_t challenge,
        const std::vector<uint8_t>& enrolledPasswordHandle,
        const std::vector<uint8_t>& providedPassword,
        GatekeeperVerifyResponse* _aidl_return) override;
        
    ndk::ScopedAStatus deleteAllUsers() override;
    ndk::ScopedAStatus deleteUser(int32_t uid) override;
};
