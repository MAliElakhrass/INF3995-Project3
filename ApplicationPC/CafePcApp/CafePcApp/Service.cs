using System;
using System.IO;
using System.Net;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;
using static CafePcApp.Utils;

namespace CafePcApp
{
    public class Service
    {
        public const string SERVER_ADMIN_URL = "https://132.207.89.60:443/admin/";
        public const string LOGIN_PATH = "login";
        public const string LOGS_PATH = "logs";
        public const string LOGOUT_PATH = "logout";
        public const string STATISTICS_PATH = "statistiques";
        public const string CHANGE_PWD = "changement_mdp";
        public const string CREATE_SUP_ACCOUNT = "creationcompte";
        public const string DELETE_SUP_ACCOUNT = "suppressioncompte";

        public X509Certificate certificate;

        /**
         * Constructor 
         */
        public Service() {
            initCertificate();
        }

        /**
         * POST /admin/login
         **/
        public ResponseServer loginAdmin(string json)
        {
            HttpWebRequest req = (HttpWebRequest) WebRequest.Create(SERVER_ADMIN_URL + LOGIN_PATH);
            req.ClientCertificates.Add(certificate);
            req.Method = "POST";

            try
            {
                Stream dataStream = req.GetRequestStream();
                dataStream.Write(Utils.AESencryption(json), 0, Utils.AESencryption(json).Length);
                dataStream.Close();

                HttpWebResponse resp = (HttpWebResponse) req.GetResponse();

                byte[] responseEncrypted = Utils.readFullyStream(resp.GetResponseStream());

                return new ResponseServer((int) resp.StatusCode, Utils.AESdecryption(responseEncrypted));
            }
            catch (WebException e)
            {
                if (e.Message.Contains("400"))
                {
                    return new ResponseServer(400, "Please check username or password");
                }
                
                return new ResponseServer(403, e.Message);
            }
        }

        /**
         * POST /admin/logout
         **/
        public ResponseServer logout()
        {
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(SERVER_ADMIN_URL + LOGOUT_PATH);
            req.ClientCertificates.Add(certificate);
            req.Method = "POST";

            try
            {
                HttpWebResponse resp = (HttpWebResponse)req.GetResponse();

                byte[] responseEncrypted = Utils.readFullyStream(resp.GetResponseStream());

                return new ResponseServer((int)resp.StatusCode, Utils.AESdecryption(responseEncrypted));
            }
            catch (WebException e)
            {
                return new ResponseServer(401, e.Message);
            }
        }

        /**
         * POST /admin/changement_mdp
         **/
        public ResponseServer changePassword(string json)
        {
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(SERVER_ADMIN_URL + CHANGE_PWD);
            req.ClientCertificates.Add(certificate);
            req.Method = "POST";

            try
            {
                Stream dataStream = req.GetRequestStream();
                dataStream.Write(Utils.AESencryption(json), 0, Utils.AESencryption(json).Length);
                dataStream.Close();

                HttpWebResponse resp = (HttpWebResponse)req.GetResponse();

                byte[] responseEncrypted = Utils.readFullyStream(resp.GetResponseStream());

                return new ResponseServer((int)resp.StatusCode, Utils.AESdecryption(responseEncrypted));
            }
            catch (WebException e)
            {
                if (e.Message.Contains("400"))
                {
                    return new ResponseServer(400, "Please check username or password");
                }

                return new ResponseServer(401, e.Message);
            }
        }

        /**
         * POST /admin/logs
         **/
        public ResponseServer logsAdmin(String json)
        {
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(SERVER_ADMIN_URL + LOGS_PATH);
            req.ClientCertificates.Add(certificate);
            req.Method = "POST";

            try
            {
                Stream dataStream = req.GetRequestStream();
                dataStream.Write(Utils.AESencryption(json), 0, Utils.AESencryption(json).Length);
                dataStream.Close();

                HttpWebResponse resp = (HttpWebResponse) req.GetResponse();

                byte[] responseEncrypted = Utils.readFullyStream(resp.GetResponseStream());

                return new ResponseServer((int) resp.StatusCode, Utils.AESdecryption(responseEncrypted));
            }
            catch (WebException e)
            {
                if (e.Message.Contains("400"))
                {
                    return new ResponseServer(400, e.Message);
                }
                
                return new ResponseServer(401, e.Message);
            }
        }

        /**
         * GET /admin/statistiques
         **/
        public ResponseServer getStatistics()
        {
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(SERVER_ADMIN_URL + STATISTICS_PATH);
            req.ClientCertificates.Add(certificate);
            req.Method = "GET";

            try
            {
                HttpWebResponse resp = (HttpWebResponse)req.GetResponse();

                byte[] responseEncrypted = Utils.readFullyStream(resp.GetResponseStream());

                return new ResponseServer((int)resp.StatusCode, Utils.AESdecryption(responseEncrypted));
            }
            catch (WebException e)
            {
                return new ResponseServer(401, e.Message);
            }
        }

        /**
         * POST /admin/creationcompte
         **/
        public ResponseServer createSupAccount(String json)
        {
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(SERVER_ADMIN_URL + CREATE_SUP_ACCOUNT);
            req.ClientCertificates.Add(certificate);
            req.Method = "POST";

            try
            {
                Stream dataStream = req.GetRequestStream();
                dataStream.Write(Utils.AESencryption(json), 0, Utils.AESencryption(json).Length);
                dataStream.Close();

                HttpWebResponse resp = (HttpWebResponse)req.GetResponse();

                byte[] responseEncrypted = Utils.readFullyStream(resp.GetResponseStream());

                return new ResponseServer((int)resp.StatusCode, Utils.AESdecryption(responseEncrypted));
            }
            catch (WebException e)
            {
                if (e.Message.Contains("400"))
                {
                    return new ResponseServer(400, e.Message);
                }

                return new ResponseServer(403, e.Message);
            }
        }

        /**
         * POST /admin/suppressioncompte
         **/
        public ResponseServer deleteSupAccount(String json)
        {
            HttpWebRequest req = (HttpWebRequest)WebRequest.Create(SERVER_ADMIN_URL + DELETE_SUP_ACCOUNT);
            req.ClientCertificates.Add(certificate);
            req.Method = "POST";

            try
            {
                Stream dataStream = req.GetRequestStream();
                dataStream.Write(Utils.AESencryption(json), 0, Utils.AESencryption(json).Length);
                dataStream.Close();

                HttpWebResponse resp = (HttpWebResponse)req.GetResponse();

                byte[] responseEncrypted = Utils.readFullyStream(resp.GetResponseStream());

                return new ResponseServer((int)resp.StatusCode, Utils.AESdecryption(responseEncrypted));
            }
            catch (WebException e)
            {
                if (e.Message.Contains("400"))
                {
                    return new ResponseServer(400, e.Message);
                }

                return new ResponseServer(403, e.Message);
            }
        }

        /**
         * This function reads the client's certificate from the file "client.p12"
         * and validates it
         **/
        private void initCertificate()
        {
            certificate = new X509Certificate2("client.p12", "wazaa");

            // Validate client certificate with server's certificate
            ServicePointManager.CheckCertificateRevocationList = false;
            ServicePointManager.ServerCertificateValidationCallback = ValidateCertificate;
            ServicePointManager.Expect100Continue = true;
        }

        /**
         * This function returns a boolean. It validates the certificate.
         * 
         * Inspired from : https://stackoverflow.com/a/11251705/9319274
         **/
        private bool ValidateCertificate(object sender, X509Certificate certificate,
            X509Chain chain, SslPolicyErrors sslPolicyErrors)
        {
            if (sslPolicyErrors == SslPolicyErrors.None)
                return true;

            X509Certificate serverCertificate = new X509Certificate2("server.pfx", "wazaa");

            if (certificate.Subject.Equals(serverCertificate.Subject))
            {
                return true;
            }

            // Do not allow this client to communicate with unauthenticated servers.
            return false;
        }
    }
}
