#include "QseeComWrapper.h"
#include <dlfcn.h>
#include <android-base/logging.h>

static void* libHandle = nullptr;
static int (*qsee_start_app)(QSEECom_handle**, const char*, const char*, uint32_t) = nullptr;
static int (*qsee_send_cmd)(QSEECom_handle*, void*, uint32_t, void*, uint32_t) = nullptr;
static int (*qsee_shutdown_app)(QSEECom_handle**) = nullptr;

bool QseeComWrapper::init() {
    libHandle = dlopen("libQSEEComAPI.so", RTLD_NOW);
    if (!libHandle) {
        LOG(ERROR) << "Failed to load libQSEEComAPI.so: " << dlerror();
        return false;
    }
    qsee_start_app = reinterpret_cast<decltype(qsee_start_app)>(dlsym(libHandle, "QSEECom_start_app"));
    qsee_send_cmd = reinterpret_cast<decltype(qsee_send_cmd)>(dlsym(libHandle, "QSEECom_send_cmd"));
    qsee_shutdown_app = reinterpret_cast<decltype(qsee_shutdown_app)>(dlsym(libHandle, "QSEECom_shutdown_app"));
    return qsee_start_app && qsee_send_cmd && qsee_shutdown_app;
}

int QseeComWrapper::start_app(QSEECom_handle **handle, const char *path, const char *fname, uint32_t sb_size) {
    return qsee_start_app(handle, path, fname, sb_size);
}

int QseeComWrapper::send_cmd(QSEECom_handle *handle, void *req, uint32_t req_len, void *resp, uint32_t resp_len) {
    return qsee_send_cmd(handle, req, req_len, resp, resp_len);
}

int QseeComWrapper::shutdown_app(QSEECom_handle **handle) {
    return qsee_shutdown_app(handle);
}
