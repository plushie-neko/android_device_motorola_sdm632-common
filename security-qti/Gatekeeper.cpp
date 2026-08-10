#include "Gatekeeper.h"
#include <android-base/logging.h>

ndk::ScopedAStatus Gatekeeper::verify(
        int32_t uid, int64_t challenge,
        const std::vector<uint8_t>& enrolledPasswordHandle,
        const std::vector<uint8_t>& providedPassword,
        GatekeeperVerifyResponse* _aidl_return) {

    uint32_t req_size = sizeof(qti_gk_verify_req) + enrolledPasswordHandle.size() + providedPassword.size();
    uint32_t resp_size = sizeof(qti_gk_resp) + 1024;
    
    if (req_size + resp_size > mHandle->sbuf_len) return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);

    uint8_t* req_buf = mHandle->ion_sbuffer;
    qti_gk_verify_req* req = reinterpret_cast<qti_gk_verify_req*>(req_buf);
    
    req->cmd_id = 0x1002;
    req->uid = uid;
    req->challenge = challenge;
    
    req->pwd_handle_offset = sizeof(qti_gk_verify_req);
    req->pwd_handle_length = enrolledPasswordHandle.size();
    memcpy(req_buf + req->pwd_handle_offset, enrolledPasswordHandle.data(), req->pwd_handle_length);
    
    req->provided_pwd_offset = req->pwd_handle_offset + req->pwd_handle_length;
    req->provided_pwd_length = providedPassword.size();
    memcpy(req_buf + req->provided_pwd_offset, providedPassword.data(), req->provided_pwd_length);

    uint8_t* resp_buf = req_buf + req_size;
    int ret = QseeComWrapper::send_cmd(mHandle, req_buf, req_size, resp_buf, resp_size);

    qti_gk_resp* resp = reinterpret_cast<qti_gk_resp*>(resp_buf);
    
    if (ret != 0 || resp->status != 0) {
        return ndk::ScopedAStatus::fromServiceSpecificError(IGatekeeper::ERROR_RETRY_TIMEOUT);
    }

    _aidl_return->hardwareAuthToken.mac.assign(
        resp_buf + resp->payload_offset,
        resp_buf + resp->payload_offset + resp->payload_length
    );

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Gatekeeper::enroll(
        int32_t uid,
        const std::vector<uint8_t>& currentPasswordHandle,
        const std::vector<uint8_t>& currentPassword,
        const std::vector<uint8_t>& desiredPassword,
        GatekeeperEnrollResponse* _aidl_return) {

    uint32_t req_size = sizeof(qti_gk_enroll_req) + currentPasswordHandle.size() + currentPassword.size() + desiredPassword.size();
    uint32_t resp_size = sizeof(qti_gk_resp) + 1024;
    
    if (req_size + resp_size > mHandle->sbuf_len) return ndk::ScopedAStatus::fromExceptionCode(EX_ILLEGAL_ARGUMENT);

    uint8_t* req_buf = mHandle->ion_sbuffer;
    qti_gk_enroll_req* req = reinterpret_cast<qti_gk_enroll_req*>(req_buf);
    
    req->cmd_id = 0x1001;
    req->uid = uid;
    
    uint32_t offset = sizeof(qti_gk_enroll_req);
    req->current_pwd_handle_offset = offset;
    req->current_pwd_handle_length = currentPasswordHandle.size();
    memcpy(req_buf + offset, currentPasswordHandle.data(), req->current_pwd_handle_length);
    offset += req->current_pwd_handle_length;

    req->current_pwd_offset = offset;
    req->current_pwd_length = currentPassword.size();
    memcpy(req_buf + offset, currentPassword.data(), req->current_pwd_length);
    offset += req->current_pwd_length;

    req->desired_pwd_offset = offset;
    req->desired_pwd_length = desiredPassword.size();
    memcpy(req_buf + offset, desiredPassword.data(), req->desired_pwd_length);

    uint8_t* resp_buf = req_buf + req_size;
    int ret = QseeComWrapper::send_cmd(mHandle, req_buf, req_size, resp_buf, resp_size);

    qti_gk_resp* resp = reinterpret_cast<qti_gk_resp*>(resp_buf);
    
    if (ret != 0 || resp->status != 0) {
        return ndk::ScopedAStatus::fromServiceSpecificError(IGatekeeper::ERROR_RETRY_TIMEOUT);
    }

    _aidl_return->data.assign(
        resp_buf + resp->payload_offset,
        resp_buf + resp->payload_offset + resp->payload_length
    );

    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Gatekeeper::deleteAllUsers() {
    uint32_t req_size = sizeof(qti_gk_clear_all_req);
    uint32_t resp_size = sizeof(qti_gk_resp);
    
    uint8_t* req_buf = mHandle->ion_sbuffer;
    qti_gk_clear_all_req* req = reinterpret_cast<qti_gk_clear_all_req*>(req_buf);
    
    req->cmd_id = 0x1004;

    uint8_t* resp_buf = req_buf + req_size;
    QseeComWrapper::send_cmd(mHandle, req_buf, req_size, resp_buf, resp_size);
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus Gatekeeper::deleteUser(int32_t uid) {
    uint32_t req_size = sizeof(qti_gk_clear_user_req);
    uint32_t resp_size = sizeof(qti_gk_resp);
    
    uint8_t* req_buf = mHandle->ion_sbuffer;
    qti_gk_clear_user_req* req = reinterpret_cast<qti_gk_clear_user_req*>(req_buf);
    
    req->cmd_id = 0x1003;
    req->uid = uid;

    uint8_t* resp_buf = req_buf + req_size;
    QseeComWrapper::send_cmd(mHandle, req_buf, req_size, resp_buf, resp_size);
    return ndk::ScopedAStatus::ok();
}
