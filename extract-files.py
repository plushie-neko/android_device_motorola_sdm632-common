#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

import extract_utils.tools

extract_utils.tools.DEFAULT_PATCHELF_VERSION = '0_9'

from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)

from extract_utils.fixups_lib import (
    lib_fixup_remove,
    lib_fixup_vendorcompat,
    lib_fixups_user_type,
    libs_proto_3_9_1,
)

from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

namespace_imports = [
    'device/meizu/m1822',
    'device/meizu/m1822',
    "hardware/qcom-caf/msm8996",
    "hardware/qcom-caf/wlan",
    'vendor/meizu/m1822',
    "vendor/qcom/opensource/dataservices",
]

lib_fixups: lib_fixups_user_type = {
    libs_proto_3_9_1: lib_fixup_vendorcompat,
    (
        'libc2dcolorconvert',
    ): lib_fixup_remove,
}

blob_fixups: blob_fixups_user_type = {
    'vendor/bin/charge_only_mode': blob_fixup()
        .add_needed('libmemset_shim.so'),
    
    ('vendor/lib/sensors.ssc.so', 'vendor/lib64/sensors.ssc.so'): blob_fixup()
        .replace_needed('libutils.so', 'libutils-v32.so'),
    'vendor/lib/soundfx/libmmieffectswrapper.so': blob_fixup()
    'vendor/lib/soundfx/libspeakerbundle.so': blob_fixup()
        .replace_needed('libutils.so', 'libutils-v32.so')
    'vendor/lib/libmot_gpu_mapper.so': blob_fixup()
        .add_needed('libgui_shim_vendor.so'),
    'vendor/lib/libmmcamera2_pproc_modules.so': blob_fixup()
        .binary_regex_replace(b'\x70\x72\x6F\x64\x75\x63\x74\x2E\x6D\x61\x6E\x75', b'\x70\x72\x6F\x64\x75\x63\x74\x2E\x6E\x6F\x70\x65'),
}  # fmt: skip

module = ExtractUtilsModule(
    'm1822',
    'meizu',
    namespace_imports=namespace_imports,
    blob_fixups=blob_fixups,
    lib_fixups=lib_fixups,
)

if __name__ == '__main__':
    utils = ExtractUtils.device_with_common(module, 'm1822', module.vendor)
    utils.run()

# Merged from common
#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

import extract_utils.tools

extract_utils.tools.DEFAULT_PATCHELF_VERSION = '0_9'

from extract_utils.file import File
from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.fixups_lib import (
    lib_fixup_remove,
    lib_fixups,
    lib_fixups_user_type,
    lib_fixup_remove_arch_suffix,
    lib_fixup_vendorcompat,
    lib_fixups_user_type,
    libs_clang_rt_ubsan,
    libs_proto_3_9_1,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

namespace_imports = [
        'device/meizu/m1822',
        'hardware/meizu',
        'hardware/qcom-caf/msm8996',
        'hardware/qcom-caf/wlan',
        'vendor/qcom/opensource/dataservices',
]

def lib_fixup_vendor_suffix(lib: str, partition: str, *args, **kwargs):
    return f'{lib}_{partition}' if partition == 'vendor' else None

lib_fixups: lib_fixups_user_type = {
    **lib_fixups,
    (
        'com.qualcomm.qti.imscmservice@1.0',
        'com.qualcomm.qti.imscmservice@2.0',
        'com.qualcomm.qti.imscmservice@2.1',
        'com.qualcomm.qti.imscmservice@2.2',
        'com.qualcomm.qti.uceservice@2.0',
        'com.qualcomm.qti.uceservice@2.1',
        'vendor.qti.data.factory@2.1',
        'vendor.qti.data.slm@1.0',
        'vendor.qti.hardware.fm@1.0',
        'vendor.qti.hardware.iop@1.0',
        'vendor.qti.hardware.iop@2.0',
        'vendor.qti.hardware.scve.objecttracker@1.0',
        'vendor.qti.hardware.scve.panorama@1.0',
        'vendor.qti.ims.callinfo@1.0',
        'vendor.qti.ims.rcsconfig@1.0',
        'vendor.qti.ims.rcsconfig@1.1',
        'vendor.qti.imsrtpservice@2.0',
        'vendor.qti.imsrtpservice@2.1',
        'vendor.qti.imsrtpservice@2.0'
    ): lib_fixup_vendor_suffix,
    (
        'libmm-omxcore',
        'libc2dcolorconvert',

    ): lib_fixup_remove,
}

blob_fixups: blob_fixups_user_type = {
    (
        'system_ext/etc/permissions/com.qualcomm.qti.imscmservice-V2.0-java.xml',
        'system_ext/etc/permissions/com.qualcomm.qti.imscmservice-V2.1-java.xml',
        'system_ext/etc/permissions/qcrilhook.xml',
        'system_ext/etc/permissions/telephonyservice.xml',
        'system_ext/etc/permissions/vendor.qti.hardware.data.connection-V1.0-java.xml',
        'system_ext/etc/permissions/vendor.qti.hardware.data.connection-V1.1-java.xml',
    ): blob_fixup()
        .regex_replace('product', 'system_ext')
        .regex_replace('xml version="2.0"', 'xml version="1.0"'),
    (
        'system_ext/lib/lib-imscamera.so',
        'system_ext/lib/lib-imsvideocodec.so',
        'system_ext/lib64/lib-imscamera.so',
        'system_ext/lib64/lib-imsvideocodec.so',
    ): blob_fixup()
        .add_needed('libgui_shim.so'),
    'vendor/bin/pm-service': blob_fixup()
        .add_needed('libutils-v33.so'),
    
    'vendor/lib64/libmotext_inf.so': blob_fixup()
        .remove_needed('libril.so'),
    'vendor/lib64/libril-qc-hal-qmi.so': blob_fixup()
        .add_needed('libcutils_shim.so'),
    'vendor/lib64/libwvhidl.so': blob_fixup()
        .add_needed('libcrypto_shim.so'),
}  # fmt: skip

module = ExtractUtilsModule(
    'm1822',
    'meizu',
    blob_fixups=blob_fixups,
    lib_fixups=lib_fixups,
    namespace_imports=namespace_imports,
)

if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
