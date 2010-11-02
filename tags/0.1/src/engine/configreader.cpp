#include "headers.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <string>

#include "shared/ccor.h"
#include "configreader.h"

using namespace ccor;


ConfigReader::ConfigReader(const char* fileName)
:
        mBufferPos(0),
        mLastReadCharacter(-1),
        mKeyRead(false)
{
        mFile = fopen(fileName, "rb");
}

        
ConfigReader::~ConfigReader()
{
        if (mFile) {
                fclose(mFile);
        }
}


int ConfigReader::ReadToken()
{
        int b;
        char c;
        
        mKeyRead = false;

        /// Process the first character
        if (mLastReadCharacter != -1) {
                b = mLastReadCharacter;
                c = (char)b;
        } else {
                b = fgetc(mFile);
                c = (char)b;
        }

        /// Skip white spaces
        while ((c == ' ') || (c == '\t') || (c == '\n') || (c == '\r')) {
                b = fgetc(mFile);
                c = (char)b;
                if (b == -1) {
                        return -1;
                }
        }

        if (b == -1) {
                return -1;
        }

        mBuffer[0] = c;
        mBufferPos = 1;

        /// Check if it's a comment
        if (c == ';') {
                while ((b = fgetc(mFile)) >= 0) {
                        if ((b == 0xA) || (b == 0xD)) {
                                return ReadToken();
                        }
                }
        /// Check if it's a string token
        } else if (c == '"') {
                mBufferPos = 0; // skip leading '"'
                while ((b = fgetc(mFile)) >= 0) {
                        if ((char)b == '"') {
                                mLastReadCharacter = -1;
                                break;
                        }
                        mBuffer[mBufferPos++] = (char)b;
                }
        /// Check if it's a non alpha-numeric token
        } else if (!isalnum(c)) {
                mBuffer[1] = 0;
                mLastReadCharacter = -1;
        /// Read alpha-numeric token
        } else {
                while ((b = fgetc(mFile)) >= 0) {
                        if (isalnum(b) || (ispunct(b) && ((char)b != '='))){
                                mBuffer[mBufferPos++] = (char)b;
                        } else {
                                mLastReadCharacter = b;
                                break;
                        }
                }
        }

        mBuffer[mBufferPos] = 0;

        return b;
}


const char* ConfigReader::PeekKey()
{
        if (!mKeyRead) {
                int c = ReadToken();
                if (c == -1) {
                        return 0;
                }
                mKeyRead = true;
        }

        if (strcmp("}", mBuffer) == 0) {
                mLastReadCharacter = (int)'}';
                return 0;
        }

        return mBuffer;
}


bool ConfigReader::BeginGroup(const char* name)
{
        if (strcmp(PeekKey(), name) != 0) {
                getCore()->logMessage("Error: wrong group key: %s, expected: %s", PeekKey(), name);
                return false;
        }

        bool ok = (strcmp(name, mBuffer) == 0);
        if (ok) {
                ReadToken();
                ok = (strcmp("{", mBuffer) == 0);
        }
        return ok;
}


bool ConfigReader::EndGroup()
{
        ReadToken();
        if (strcmp("}", mBuffer) != 0) {
                getCore()->logMessage("Error: expected '}', but found: %s", mBuffer);
                return false;
        }
        return true;
}


int ConfigReader::ReadInt32(const char* name)
{
        const char* string = ReadString(name);
        int value = atoi(string);
        return value;
}


unsigned int ConfigReader::ReadUInt32(const char* name)
{
        const char* string = ReadString(name);
        int value = atoi(string);
        return (unsigned int)value;
}


bool ConfigReader::ReadBool(const char* name)
{
        const char* string = ReadString(name);
        bool value = true;
        if (stricmp(string, "false") == 0) {
            value = false;
        } else if (stricmp(string, "no") == 0) {
            value = false;
        } else if (strcmp(string, "0") == 0) {
            value = false;
        } else if (strlen(string) == 0) {
            value = false;
        }
        return value;
}


float ConfigReader::ReadFloat(const char* name)
{
        const char* string = ReadString(name);
        float value = (float)atof(string);
        return value;
}


const char* ConfigReader::ReadString(const char* name)
{
        if (strcmp(PeekKey(), name) != 0) {
                getCore()->logMessage("Error: wrong key: %s, expected: %s", PeekKey(), name);
        }

        ReadToken();
        if (strcmp("=", mBuffer) != 0) {
                getCore()->logMessage("Error: expected '=' found: %s", mBuffer);
        }

        ReadToken();

        return mBuffer;
}
