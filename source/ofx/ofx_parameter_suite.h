#ifndef OFX_PARAMETER_SUITE_H
#define OFX_PARAMETER_SUITE_H

#include <ofxParam.h>

OfxStatus param_define(OfxParamSetHandle paramSet, const char *paramType,
                       const char *name, OfxPropertySetHandle *propertySet);

OfxStatus param_get_handle(OfxParamSetHandle paramSet, const char *name,
                           OfxParamHandle *param,
                           OfxPropertySetHandle *propertySet);

OfxStatus param_set_get_property_set(OfxParamSetHandle paramSet,
                                     OfxPropertySetHandle *propHandle);

OfxStatus param_get_property_set(OfxParamHandle param,
                                 OfxPropertySetHandle *propHandle);

OfxStatus param_get_value(OfxParamHandle paramHandle, ...);

OfxStatus param_get_value_at_time(OfxParamHandle paramHandle, OfxTime time,
                                  ...);

OfxStatus param_get_derivative(OfxParamHandle paramHandle, OfxTime time, ...);

OfxStatus param_get_integral(OfxParamHandle paramHandle, OfxTime time1,
                             OfxTime time2, ...);

OfxStatus param_set_value(OfxParamHandle paramHandle, ...);

OfxStatus param_set_value_at_time(OfxParamHandle paramHandle,
                                  OfxTime time, // time in frames
                                  ...);

OfxStatus param_get_num_keys(OfxParamHandle paramHandle,
                             unsigned int *numberOfKeys);

OfxStatus param_get_key_time(OfxParamHandle paramHandle, unsigned int nthKey,
                             OfxTime *time);

OfxStatus param_get_key_index(OfxParamHandle paramHandle, OfxTime time,
                              int direction, int *index);

OfxStatus param_delete_key(OfxParamHandle paramHandle, OfxTime time);

OfxStatus param_delete_all_keys(OfxParamHandle paramHandle);

OfxStatus param_copy(OfxParamHandle paramTo, OfxParamHandle paramFrom,
                     OfxTime dstOffset, const OfxRangeD *frameRange);

OfxStatus param_edit_begin(OfxParamSetHandle paramSet, const char *name);

OfxStatus param_edit_end(OfxParamSetHandle paramSet);

#endif // OFX_PARAMETER_SUITE_H
