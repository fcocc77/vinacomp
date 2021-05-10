#include <ofx_image_effect_suite.h>

OfxStatus get_property_set(OfxImageEffectHandle imageEffect,
                           OfxPropertySetHandle *propHandle)
{
    return 0;
}

OfxStatus get_param_set(OfxImageEffectHandle imageEffect,
                        OfxParamSetHandle *paramSet)
{
    return 0;
}

OfxStatus clip_define(OfxImageEffectHandle imageEffect, const char *name,
                      OfxPropertySetHandle *propertySet)
{
    return 0;
}

OfxStatus clip_get_handle(OfxImageEffectHandle imageEffect, const char *name,
                          OfxImageClipHandle *clip,
                          OfxPropertySetHandle *propertySet)
{
    return 0;
}

OfxStatus clip_get_property_set(OfxImageClipHandle clip,
                                OfxPropertySetHandle *propHandle)
{
    return 0;
}

OfxStatus clip_get_image(OfxImageClipHandle clip, OfxTime time,
                         const OfxRectD *region,
                         OfxPropertySetHandle *imageHandle)
{
    return 0;
}

OfxStatus clip_release_image(OfxPropertySetHandle imageHandle)
{
    return 0;
}

OfxStatus clip_get_region_of_definition(OfxImageClipHandle clip, OfxTime time,
                                        OfxRectD *bounds)
{
    return 0;
}

int abort(OfxImageEffectHandle imageEffect)
{
    return 0;
}

OfxStatus image_memory_alloc(OfxImageEffectHandle instanceHandle, size_t nBytes,
                             OfxImageMemoryHandle *memoryHandle)
{
    return 0;
}

OfxStatus image_memory_free(OfxImageMemoryHandle memoryHandle)
{
    return 0;
}

OfxStatus image_memory_lock(OfxImageMemoryHandle memoryHandle,
                            void **returnedPtr)
{
    return 0;
}

OfxStatus image_memory_unlock(OfxImageMemoryHandle memoryHandle)
{
    return 0;
}
