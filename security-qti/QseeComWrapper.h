#pragma once
#include <stdint.h>

struct QSEECom_handle {
    unsigned char *ion_sbuffer;
    uint32_t sbuf_len;
};

class QseeComWrapper {
public:
    static bool init();
    static int start_app(QSEECom_handle **clnt_handle, const char *path, const char *fname, uint32_t sb_size);
    static int send_cmd(QSEECom_handle *handle, void *send_req_ptr, uint32_t send_req_len, void *resp_ptr, uint32_t resp_len);
    static int shutdown_app(QSEECom_handle **handle);
};
