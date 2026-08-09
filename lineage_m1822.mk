# Inherit some common Lineage stuff.
$(call inherit-product, $(SRC_TARGET_DIR)/product/core_64_bit.mk)
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# Device
$(call inherit-product, $(LOCAL_PATH)/device.mk)

PRODUCT_BRAND := meizu
PRODUCT_DEVICE := m1822
PRODUCT_MANUFACTURER := meizu
PRODUCT_MODEL := m1822
PRODUCT_NAME := lineage_m1822

PRODUCT_BUILD_PROP_OVERRIDES += \
    BuildDesc="m1822-user 10 QPOS30.52-29-12 401fd release-keys" \
    BuildFingerprint=meizu/m1822/m1822:10/QPOS30.52-29-12/401fd:user/release-keys \
    DeviceName=m1822
