/*
AES_CBC:    Class used by admin services (all admin DynamicPages) to decrypt HTTP request content and
            encrypt HTTP response content.
*/
#ifndef AES
#define AES

#include <string>
#include <openssl/aes.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>

#include "../constants.h"

#define KEY_FILE_PATH "../ssl/admin_encryption/key.key"
#define KEY_SIZE 256

class AES_CBC
{

public:
    AES_CBC() {}

    unsigned char* encrypt(std::string plaintext)
    {

        std::ifstream input( KEY_FILE_PATH, std::ios::binary );
        if (!input)
            return nullptr;
        
        AES_KEY key;
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
        input.close();

        if (AES_set_encrypt_key(buffer.data(), KEY_SIZE, &key) != 0)
                return nullptr;
        
        out_ = new unsigned char[len];
        unsigned char iVec[AES_BLOCK_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                              0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
        AES_cbc_encrypt((unsigned char*) plaintext.data(), out_, len, &key, iVec, AES_ENCRYPT);

        return out_;
    }

    std::string decrypt(std::vector<unsigned char> cyphertext)
    {
        std::ifstream input( KEY_FILE_PATH, std::ios::binary );
        if (!input)
            return ENCRYPTION_ERROR;
        
        AES_KEY key;
        std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
        input.close();
      
        if (AES_set_decrypt_key(buffer.data(), KEY_SIZE, &key) != 0)
            return ENCRYPTION_ERROR;
        
        unsigned char out[cyphertext.size()];
        unsigned char iVec[AES_BLOCK_SIZE] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                                              0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
        AES_cbc_encrypt(cyphertext.data(), out, cyphertext.size(), &key, iVec, AES_DECRYPT);
        const std::string decypheredText(reinterpret_cast<const char *>(out), cyphertext.size());

        return decypheredText;
    }

    //must be a multiple of AES_BLOCK_SIZE
    void setTextLength(std::string text)
    {
        if ((text.size()) % AES_BLOCK_SIZE == 0) 
            len = text.size();
        else
            len = (((text.size()) / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE;
    }

    int getLength() { return len; }

private:
    int len;
    unsigned char* out_;
};

#endif