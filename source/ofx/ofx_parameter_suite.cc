#include <ofx_parameter_suite.h>

OfxStatus param_define(OfxParamSetHandle paramSet, const char *paramType,
                       const char *name, OfxPropertySetHandle *propertySet)
{
    return 0;
}

OfxStatus param_get_handle(OfxParamSetHandle paramSet, const char *name,
                           OfxParamHandle *param,
                           OfxPropertySetHandle *propertySet)
{
    return 0;
}

OfxStatus param_set_get_property_set(OfxParamSetHandle paramSet,
                                     OfxPropertySetHandle *propHandle)
{
    return 0;
}

OfxStatus param_get_property_set(OfxParamHandle param,
                                 OfxPropertySetHandle *propHandle)
{
    return 0;
}

OfxStatus param_get_value(OfxParamHandle paramHandle, ...)
{
    return 0;
}

OfxStatus param_get_value_at_time(OfxParamHandle paramHandle, OfxTime time, ...)
{
    return 0;
}

OfxStatus param_get_derivative(OfxParamHandle paramHandle, OfxTime time, ...)
{
    return 0;
}

OfxStatus param_get_integral(OfxParamHandle paramHandle, OfxTime time1,
                             OfxTime time2, ...)
{
    return 0;
}

OfxStatus param_set_value(OfxParamHandle paramHandle, ...)
{
    return 0;
}

OfxStatus param_set_value_at_time(OfxParamHandle paramHandle,
                                  OfxTime time, // time in frames
                                  ...)
{
    return 0;
}

OfxStatus param_get_num_keys(OfxParamHandle paramHandle,
                             unsigned int *numberOfKeys)
{
    return 0;
}

OfxStatus param_get_key_time(OfxParamHandle paramHandle, unsigned int nthKey,
                             OfxTime *time)
{
    return 0;
}

OfxStatus param_get_key_index(OfxParamHandle paramHandle, OfxTime time,
                              int direction, int *index)
{
    return 0;
}

OfxStatus param_delete_key(OfxParamHandle paramHandle, OfxTime time)
{
    return 0;
}

OfxStatus param_delete_all_keys(OfxParamHandle paramHandle)
{
    return 0;
}

OfxStatus param_copy(OfxParamHandle paramTo, OfxParamHandle paramFrom,
                     OfxTime dstOffset, const OfxRangeD *frameRange)
{
    return 0;
}

OfxStatus param_edit_begin(OfxParamSetHandle paramSet, const char *name)
{
    return 0;
}

OfxStatus param_edit_end(OfxParamSetHandle paramSet)
{
    return 0;
}
