#pragma once

#include <stdio.h>

/// Reads config data from a file.
///
/// File format:
/// \code
///     Group {
///             Key = Value
///             Key = "Value with spaces"
///             Group {
///                     Key = Value
///             }
///     }
/// \endcode
/// 
/// \author Julius Trinkunas
class ConfigReader
{
        enum ClassDefines {
                MaxBufferSize = 1024
        };

private:
        FILE*                   mFile;
        char                    mBuffer[MaxBufferSize];
        int                     mBufferPos;
        int                     mLastReadCharacter;
        bool                    mKeyRead;
        
private:
        int                     ReadToken();

public:
                                ConfigReader(const char* fileName);
                                ~ConfigReader();

        bool                    IsFileOpen() { return mFile != 0; };

        /// See next key's name without reading it.
        /// \returns key name in a zero terminated string format.
        virtual const char*     PeekKey();


        /// Start reading a key group.
        /// \param name of the key.
        /// \returns true if the group's name matches, or false if it doesn't.
        virtual bool            BeginGroup(const char* name);

        /// End reading a key group. 
        /// \returns true if all keys in a group where read, or false if it doesn't.
        virtual bool            EndGroup();


        /// Read key as 32 bit int value.
        /// \param name of the key.
        /// \returns value as int.
        virtual int             ReadInt32(const char* name);

        /// Read key as 32 bit unsigned int value.
        /// \param name of the key.
        /// \returns value as int.
        virtual unsigned int    ReadUInt32(const char* name);

        /// Read key as float value.
        /// \param name of the key.
        /// \returns value as float.
        virtual float           ReadFloat(const char* name);

        /// Read key as bool value.
        /// \param name of the key.
        /// \returns value in a zero terminated string format.
        virtual bool            ReadBool(const char* name);

        /// Read key as string value.
        /// \param name of the key.
        /// \returns value in a zero terminated string format.
        virtual const char*     ReadString(const char* name);
};
