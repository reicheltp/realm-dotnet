////////////////////////////////////////////////////////////////////////////
//
// Copyright 2016 Realm Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
////////////////////////////////////////////////////////////////////////////

#ifndef REALM_PROPERTY_HPP
#define REALM_PROPERTY_HPP

#include <string>

namespace realm {
    enum PropertyType {
        /** Integer type: NSInteger, int, long, Int (Swift) */
        PropertyTypeInt    = 0,
        /** Boolean type: BOOL, bool, Bool (Swift) */
        PropertyTypeBool   = 1,
        /** Float type: CGFloat (32bit), float, Float (Swift) */
        PropertyTypeFloat  = 9,
        /** Double type: CGFloat (64bit), double, Double (Swift) */
        PropertyTypeDouble = 10,
        /** String type: NSString, String (Swift) */
        PropertyTypeString = 2,
        /** Data type: NSData */
        PropertyTypeData   = 4,
        /** Any type: id, **not supported in Swift** */
        PropertyTypeAny    = 6,
        /** Date type: NSDate */
        PropertyTypeDate   = 7,
        /** Object type. See [Realm Models](http://realm.io/docs/cocoa/latest/#models) */
        PropertyTypeObject = 12,
        /** Array type. See [Realm Models](http://realm.io/docs/cocoa/latest/#models) */
        PropertyTypeArray  = 13,
    };

    struct Property {
        std::string name;
        PropertyType type;
        std::string object_type;
        bool is_primary = false;
        bool is_indexed = false;
        bool is_nullable = false;

        size_t table_column = -1;
        bool requires_index() const { return is_primary || is_indexed; }
        bool is_indexable() const { return type == PropertyTypeInt || type == PropertyTypeBool || type == PropertyTypeString || type == PropertyTypeDate; }

#if __GNUC__ < 5
        // GCC 4.9 does not support C++14 braced-init with NSDMIs
        Property(std::string name="", PropertyType type=PropertyTypeInt, std::string object_type="", bool is_primary=false, bool is_indexed=false, bool is_nullable=false)
        : name(std::move(name))
        , type(type)
        , object_type(std::move(object_type))
        , is_primary(is_primary)
        , is_indexed(is_indexed)
        , is_nullable(is_nullable)
        {
        }
#endif
    };

    static inline const char *string_for_property_type(PropertyType type) {
        switch (type) {
            case PropertyTypeString:
                return "string";
            case PropertyTypeInt:
                return "int";
            case PropertyTypeBool:
                return "bool";
            case PropertyTypeDate:
                return "date";
            case PropertyTypeData:
                return "data";
            case PropertyTypeDouble:
                return "double";
            case PropertyTypeFloat:
                return "float";
            case PropertyTypeAny:
                return "any";
            case PropertyTypeObject:
                return "object";
            case PropertyTypeArray:
                return "array";
#if __GNUC__
            default:
                __builtin_unreachable();
#endif
        }
    }
}

#endif /* REALM_PROPERTY_HPP */
