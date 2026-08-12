# Inherit some common Lineage stuff.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Device
$(call inherit-product, $(LOCAL_PATH)/device.mk)

PRODUCT_BRAND := Meizu
PRODUCT_DEVICE := m1822
PRODUCT_MANUFACTURER := Meizu
PRODUCT_MODEL := Meizu Note 8
PRODUCT_NAME := lineage_m1822

PRODUCT_GMS_CLIENTID_BASE := android-meizu

PRODUCT_BUILD_PROP_OVERRIDES += \
    BuildDesc="meizu_M1822_CN-user 8.1.0 OPM1.171019.026 1587639120 release-keys"
    BuildFingerprint="Meizu/meizu_M1822_CN/M1822:8.1.0/OPM1.171019.026/1587639120:user/release-keys"
    DeviceName=m1822
