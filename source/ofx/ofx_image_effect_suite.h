#ifndef OFX_IMAGE_EFFECT_SUITE_H
#define OFX_IMAGE_EFFECT_SUITE_H

#include <ofxImageEffect.h>

OfxStatus get_property_set(OfxImageEffectHandle imageEffect,
                           OfxPropertySetHandle *propHandle);

OfxStatus get_param_set(OfxImageEffectHandle imageEffect,
                        OfxParamSetHandle *paramSet);

OfxStatus clip_define(OfxImageEffectHandle imageEffect, const char *name,
                      OfxPropertySetHandle *propertySet);

OfxStatus clip_get_handle(OfxImageEffectHandle imageEffect, const char *name,
                          OfxImageClipHandle *clip,
                          OfxPropertySetHandle *propertySet);

OfxStatus clip_get_property_set(OfxImageClipHandle clip,
                                OfxPropertySetHandle *propHandle);

OfxStatus clip_get_image(OfxImageClipHandle clip, OfxTime time,
                         const OfxRectD *region,
                         OfxPropertySetHandle *imageHandle);

OfxStatus clip_release_image(OfxPropertySetHandle imageHandle);

OfxStatus clip_get_region_of_definition(OfxImageClipHandle clip, OfxTime time,
                                        OfxRectD *bounds);

int abort(OfxImageEffectHandle imageEffect);

OfxStatus image_memory_alloc(OfxImageEffectHandle instanceHandle, size_t nBytes,
                             OfxImageMemoryHandle *memoryHandle);

OfxStatus image_memory_free(OfxImageMemoryHandle memoryHandle);

OfxStatus image_memory_lock(OfxImageMemoryHandle memoryHandle,
                            void **returnedPtr);

OfxStatus image_memory_unlock(OfxImageMemoryHandle memoryHandle);

#endif // OFX_IMAGE_EFFECT_SUITE_H
