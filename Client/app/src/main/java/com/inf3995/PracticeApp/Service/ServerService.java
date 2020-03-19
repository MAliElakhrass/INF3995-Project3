package com.inf3995.PracticeApp.Service;

import android.content.Context;

import com.inf3995.PracticeApp.R;
import com.jakewharton.retrofit2.adapter.rxjava2.RxJava2CallAdapterFactory;

import java.io.IOException;
import java.io.InputStream;
import java.security.KeyManagementException;
import java.security.KeyStore;
import java.security.KeyStoreException;
import java.security.NoSuchAlgorithmException;
import java.security.UnrecoverableKeyException;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.cert.X509Certificate;
import java.util.concurrent.TimeUnit;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.KeyManager;
import javax.net.ssl.KeyManagerFactory;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.TrustManager;
import javax.net.ssl.TrustManagerFactory;

import okhttp3.OkHttpClient;

import retrofit2.Retrofit;
import retrofit2.converter.gson.GsonConverterFactory;
import retrofit2.converter.scalars.ScalarsConverterFactory;

public class ServerService {
    private ServerAPI serverAPI;
    private Context mContext;
    private final static String SERVER_URL_SSL = "https://192.168.2.39:443/"; //*/ "https://132.207.89.60:443/";
    private final static String SERVER_URL_NON_SECURED = "http://192.168.2.39:80/"; //*/ "http://132.207.89.60:80/";

    public ServerService(boolean ssl, Context context) {
        mContext = context;

        OkHttpClient client = null;
        try {
            client = initClient(ssl);
        } catch (KeyStoreException e) {
            e.printStackTrace();
        } catch (CertificateException e) {
            e.printStackTrace();
        } catch (NoSuchAlgorithmException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (UnrecoverableKeyException e) {
            e.printStackTrace();
        } catch (KeyManagementException e) {
            e.printStackTrace();
        }
        Retrofit retrofit = initRetrofit(ssl, client);

        serverAPI = retrofit.create(ServerAPI.class);
    }

    public ServerAPI getServerAPI(){
        return serverAPI;
    }

    /**
     * Inspired from :
     *  https://chariotsolutions.com/blog/post/https-with-client-certificates-on/
     **/
    private OkHttpClient initClient(boolean ssl) throws KeyStoreException, CertificateException, NoSuchAlgorithmException, IOException, UnrecoverableKeyException, KeyManagementException {
        if (ssl) {
            // Trust self signed certificate
            InputStream certificateFileCRT = mContext.getResources().openRawResource(R.raw.server);
            CertificateFactory certificateFactory = CertificateFactory.getInstance("X.509");
            X509Certificate cert = (X509Certificate) certificateFactory.generateCertificate(certificateFileCRT);
            String alias = cert.getSubjectX500Principal().getName();
            KeyStore trustStore = KeyStore.getInstance(KeyStore.getDefaultType());
            trustStore.load(null);
            trustStore.setCertificateEntry(alias, cert);

            // KeyStore containing client certificate
            KeyStore keyStore = KeyStore.getInstance("PKCS12");
            InputStream fis = mContext.getResources().openRawResource(R.raw.client);
            keyStore.load(fis, "wazaa".toCharArray());

            // Build an SSL context
            KeyManagerFactory kmf = KeyManagerFactory.getInstance("X509");
            kmf.init(keyStore, "wazaa".toCharArray());
            KeyManager[] keyManagers = kmf.getKeyManagers();
            TrustManagerFactory tmf = TrustManagerFactory.getInstance("X509");
            tmf.init(trustStore);
            TrustManager[] trustManagers = tmf.getTrustManagers();

            SSLContext sslContext = SSLContext.getInstance("SSL");
            sslContext.init(keyManagers, trustManagers, null);

            return new OkHttpClient.Builder()
                    .readTimeout(60, TimeUnit.SECONDS)
                    .sslSocketFactory(sslContext.getSocketFactory())
                    .hostnameVerifier(new HostnameVerifier() {
                        @Override
                        public boolean verify(String hostname, SSLSession session) {
                            return true;
                        }
                    })
                    .build();
        } else {
            return new OkHttpClient.Builder()
                    .readTimeout(60, TimeUnit.SECONDS)
                    .build();
        }
    }

    private Retrofit initRetrofit(boolean ssl, OkHttpClient client) {
        return new Retrofit.Builder()
                .baseUrl(ssl? SERVER_URL_SSL : SERVER_URL_NON_SECURED)
                .addConverterFactory(GsonConverterFactory.create())
                .addConverterFactory(ScalarsConverterFactory.create())
                .addCallAdapterFactory(RxJava2CallAdapterFactory.create())
                .client(client)
                .build();
    }
}
