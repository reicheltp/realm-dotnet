/* Copyright 2015 Realm Inc - All Rights Reserved
 * Proprietary and Confidential
 */

using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace Realms
{
    internal class MarshalHelpers
    {
        public static IntPtr BoolToIntPtr(Boolean value)
        {
            return value ? (IntPtr)1 : (IntPtr)0;
        }

        public static Boolean IntPtrToBool(IntPtr value)
        {
            return (IntPtr)1 == value;
        }

        public static IntPtr StrAllocateBuffer(out long currentBufferSizeChars, long bufferSizeNeededChars)
        {
            currentBufferSizeChars = bufferSizeNeededChars;
            return Marshal.AllocHGlobal((IntPtr)(bufferSizeNeededChars * sizeof(char)));
            //allocHGlobal instead of  AllocCoTaskMem because allcHGlobal allows lt 2 gig on 64 bit (not that .net supports that right now, but at least this allocation will work with lt 32 bit strings)   
        }

        public static string StrBufToStr(IntPtr buffer, int bufferSizeNeededChars)
        {
            string retStr = bufferSizeNeededChars > 0 ? Marshal.PtrToStringUni(buffer, bufferSizeNeededChars) : "";
            //return "" if the string is empty, otherwise copy data from the buffer
            Marshal.FreeHGlobal(buffer);
            return retStr;
        }

        public static Boolean StrBufferOverflow(IntPtr buffer, long currentBufferSizeChars, long bufferSizeNeededChars)
        {
            if (currentBufferSizeChars < bufferSizeNeededChars)
            {
                Marshal.FreeHGlobal(buffer);

                return true;
            }
            return false;
        }

        public static IntPtr RealmColType(Type columnType)
        {
            // values correspond to core/data_type.hpp enum DataType
            // ordered in decreasing likelihood of type
            if (columnType == typeof(string))
                return (IntPtr)2;  // type_String
            if (columnType == typeof(char) || columnType == typeof(byte) || columnType == typeof(short) || columnType == typeof(int) || columnType == typeof(long) ||
                columnType == typeof(char?) || columnType == typeof(byte?) || columnType == typeof(short?) || columnType == typeof(int?) || columnType == typeof(long?))
                return (IntPtr)0;  // type_Int
            if (columnType == typeof(float) || columnType == typeof(float?))
                return (IntPtr)9;  // type_Float
            if (columnType == typeof(double) || columnType == typeof(double?))
                return (IntPtr)10; // type_Double
            if (columnType == typeof(DateTimeOffset) || columnType == typeof(DateTimeOffset?))
                return (IntPtr)7;  // type_DateTime
            if (columnType == typeof(bool) || columnType == typeof(bool?))
                return (IntPtr)1;  // type_Bool
            if (columnType.BaseType == typeof(RealmObject))
                return (IntPtr)12;  // type_Link
            if (columnType.IsGenericType)
            {
                var type = columnType.GetGenericTypeDefinition();
                if (type == typeof(RealmList<>) || type == typeof(IList<>))
                    return (IntPtr)13;  // type_LinkList 
            }
            /*
            TODO
                    Binary = 4,  // type_Binary
                    Table = 5, // type_Table for sub-tables, not relatinoships????
                    Mixed = 6, // type_Mixed

            */
            throw new NotImplementedException("Type " + columnType.FullName + " not supported");
        }
    }
}
