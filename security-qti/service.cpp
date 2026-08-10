#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <android-base/logging.h>
#include "QseeComWrapper.h"
#include "Gatekeeper.h"
#include "KeyMint.h"
#include "SharedSecret.h"
QSEECom_handle* km_handle = nullptr;
QSEECom_handle* cmnlib_handle = nullptr;

int main() {
    android::base::InitLogging(nullptr, android::base::LogdLogger(android::base::SYSTEM));
    ABinderProcess_setThreadPoolMaxThreadCount(2);
    
    if (!QseeComWrapper::init()) {
        LOG(FATAL) << "Failed to init QSEEComWrapper";
        return -1;
    }

    LOG(INFO) << "Loading cmnlib...";
    int ret = QseeComWrapper::start_app(&cmnlib_handle, "/firmware/image", "cmnlib64", 4096);
    if (ret != 0) {
        ret = QseeComWrapper::start_app(&cmnlib_handle, "/firmware/image", "cmnlib", 4096);
    }
    
    LOG(INFO) << "Loading keymaster TA...";
    ret = QseeComWrapper::start_app(&km_handle, "/firmware/image", "keymaster", 8192);
    if (ret != 0) {
        ret = QseeComWrapper::start_app(&km_handle, "/firmware/image", "keymaster64", 8192);
    }

    if (ret != 0 || !km_handle) {
        LOG(FATAL) << "Failed to load hardware TEE apps!";
        return -1;
    }

    // Register Gatekeeper AIDL
    std::shared_ptr<Gatekeeper> gkService = ndk::SharedRefBase::make<Gatekeeper>(km_handle);
    const std::string gkName = std::string() + Gatekeeper::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(gkService->asBinder().get(), gkName.c_str());
    if (status != STATUS_OK) {
        LOG(FATAL) << "Failed to register Gatekeeper HAL";
    }
    
    // Register KeyMint AIDL
    std::shared_ptr<KeyMint> kmService = ndk::SharedRefBase::make<KeyMint>(km_handle);
    const std::string kmName = std::string() + KeyMint::descriptor + "/default";
    status = AServiceManager_addService(kmService->asBinder().get(), kmName.c_str());
    if (status != STATUS_OK) {
        LOG(FATAL) << "Failed to register KeyMint HAL";
    }

    // Register SharedSecret AIDL
    std::shared_ptr<SharedSecret> ssService = ndk::SharedRefBase::make<SharedSecret>();
    const std::string ssName = std::string() + SharedSecret::descriptor + "/default";
    status = AServiceManager_addService(ssService->asBinder().get(), ssName.c_str());
    if (status != STATUS_OK) {
        LOG(FATAL) << "Failed to register SharedSecret HAL";
    }

    LOG(INFO) << "Unified KeyMint + Gatekeeper Daemon is ready!";
    ABinderProcess_joinThreadPool();
    return 0;
}
