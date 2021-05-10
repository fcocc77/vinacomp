#ifndef OFX_PROPERTY_SUITE_H
#define OFX_PROPERTY_SUITE_H

#include <ofxCore.h>

OfxStatus prop_set_pointer(OfxPropertySetHandle properties,
                           const char *property, int index, void *value);

OfxStatus prop_set_string(OfxPropertySetHandle properties, const char *property,
                          int index, const char *value);

OfxStatus prop_set_double(OfxPropertySetHandle properties, const char *property,
                          int index, double value);

OfxStatus prop_set_int(OfxPropertySetHandle properties, const char *property,
                       int index, int value);

OfxStatus prop_set_pointer_n(OfxPropertySetHandle properties,
                             const char *property, int count,
                             void *const *value);

OfxStatus prop_set_string_n(OfxPropertySetHandle properties,
                            const char *property, int count,
                            const char *const *value);

OfxStatus prop_set_double_n(OfxPropertySetHandle properties,
                            const char *property, int count,
                            const double *value);

OfxStatus prop_set_int_n(OfxPropertySetHandle properties, const char *property,
                         int count, const int *value);

OfxStatus prop_get_pointer(OfxPropertySetHandle properties,
                           const char *property, int index, void **value);

OfxStatus prop_get_string(OfxPropertySetHandle properties, const char *property,
                          int index, char **value);

OfxStatus prop_get_double(OfxPropertySetHandle properties, const char *property,
                          int index, double *value);

OfxStatus prop_get_int(OfxPropertySetHandle properties, const char *property,
                       int index, int *value);

OfxStatus prop_get_pointer_n(OfxPropertySetHandle properties,
                             const char *property, int count, void **value);

OfxStatus prop_get_string_n(OfxPropertySetHandle properties,
                            const char *property, int count, char **value);

OfxStatus prop_get_double_n(OfxPropertySetHandle properties,
                            const char *property, int count, double *value);

OfxStatus prop_get_int_n(OfxPropertySetHandle properties, const char *property,
                         int count, int *value);

OfxStatus prop_reset(OfxPropertySetHandle properties, const char *property);

OfxStatus prop_get_dimension(OfxPropertySetHandle properties,
                             const char *property, int *count);

#endif // OFX_PROPERTY_SUITE_H
