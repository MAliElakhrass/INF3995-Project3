using System;
using System.Security.Cryptography;
using System.IO;

namespace CafePcApp
{
    public class Utils
    {
        const string KEY_FILE_PATH = "./key.key";

        public struct ResponseServer
        {
            public int status;
            public string message;

            public ResponseServer(int status, string message)
            {
                this.status = status;
                this.message = message;
            }
        }

        /**
         * This function convert a stream to a byte array
         * 
         * https://stackoverflow.com/a/6586039/9319274
         **/
        public static byte[] readFullyStream(Stream input)
        {
            using (MemoryStream ms = new MemoryStream())
            {
                input.CopyTo(ms);
                return ms.ToArray();
            }
        }

        /**
         * This function allows to decrypt the server's answer 
         **/
        public static string AESdecryption(byte[] cypherText)
        {
            try
            {
                byte[] buff = new byte[32];
                FileStream fs = new FileStream(KEY_FILE_PATH, FileMode.Open, FileAccess.Read);
                BinaryReader br = new BinaryReader(fs);
                long numBytes = new FileInfo(KEY_FILE_PATH).Length;
                buff = br.ReadBytes(/*(int)numBytes*/32);
                fs.Close();
                br.Close();

                string plaintext = null;
                // Create AesManaged    
                using (AesManaged aes = new AesManaged())
                {
                    // Create a decryptor   
                    aes.Key = buff;
                    aes.Padding = PaddingMode.Zeros;
                    aes.IV = new byte[16] { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F };
                    
                    ICryptoTransform decryptor = aes.CreateDecryptor(aes.Key, aes.IV);
                    
                    // Create the streams used for decryption.    
                    using (MemoryStream ms = new MemoryStream(cypherText))
                    {
                        // Create crypto stream    
                        using (CryptoStream cs = new CryptoStream(ms, decryptor, CryptoStreamMode.Read))
                        {
                            // Read crypto stream    
                            using (StreamReader reader = new StreamReader(cs))
                            {
                                plaintext = reader.ReadToEnd();
                                reader.Close();
                            }
                            cs.Close();

                        }
                    }
                }
                return plaintext.Remove(plaintext.Length - 1);
            }
            catch (Exception e)
            {
                return e.ToString();
            }
        }

        /**
         * This function allows to encrypt the request
         **/
        public static byte[] AESencryption(string plainText)
        {
            try
            {
                byte[] buff = new byte[32]; // new byte[16] { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
                FileStream fs = new FileStream(KEY_FILE_PATH, FileMode.Open, FileAccess.Read);
                BinaryReader br = new BinaryReader(fs);
                long numBytes = new FileInfo(KEY_FILE_PATH).Length;
                buff = br.ReadBytes(32);
                fs.Close();
                br.Close();
                
                byte[] encrypted;
                // Create a new AesManaged.    
                using (AesManaged aes = new AesManaged())
                {
                    // Create encryptor
                    aes.Key = buff;

                    aes.IV = new byte[16] { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
                    ICryptoTransform encryptor = aes.CreateEncryptor(buff, aes.IV);
                    
                    // Create MemoryStream
                    using (MemoryStream ms = new MemoryStream())
                    {
                        // Create crypto stream using the CryptoStream class. This class is the key to encryption    
                        // and encrypts and decrypts data from any given stream. In this case, we will pass a memory stream    
                        // to encrypt    
                        using (CryptoStream cs = new CryptoStream(ms, encryptor, CryptoStreamMode.Write))
                        {
                            // Create StreamWriter and write data to a stream    
                            using (StreamWriter sw = new StreamWriter(cs))
                                sw.Write(plainText);
                            encrypted = ms.ToArray();
                        }
                    }
                }

                // Return encrypted data    
                return encrypted;
            }
            catch (Exception e)
            {
                Console.WriteLine(e.Message);
                return null;
            }
            
        }
        
    }
        
}
