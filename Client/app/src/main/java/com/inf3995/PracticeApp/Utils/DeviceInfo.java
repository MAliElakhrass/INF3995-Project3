package com.inf3995.PracticeApp.Utils;

import android.content.Context;
import android.widget.Toast;

import java.net.InetAddress;
import java.net.NetworkInterface;
import java.util.Collections;

public class DeviceInfo {

    private static String IpAddr_;
    private static String MACAddr_;
    private static Context context_;

    public DeviceInfo(Context context/*View view*/){
        context_ = context;
        setIpAddr();
        setMACAddr();
    }

    private static void setIpAddr(){
        try{
            for(NetworkInterface networkInterface : Collections.list(NetworkInterface.getNetworkInterfaces())){
                for(InetAddress inetAddress : Collections.list(networkInterface.getInetAddresses())){
                    if(!inetAddress.isLoopbackAddress()){
                        String IPAddr = inetAddress.getHostAddress();
                        if(IPAddr.indexOf(":") < 0){
                            IpAddr_ = IPAddr;
                        }
                    }
                }
            }
        } catch (Exception e){
            Toast.makeText(context_, "There was a problem finding your IP", Toast.LENGTH_SHORT);
            IpAddr_ = "0.0.0.0";
        }
    }

    private static void setMACAddr(){
        try{
            for(NetworkInterface networkInterface : Collections.list(NetworkInterface.getNetworkInterfaces())){
                if(!networkInterface.getName().equalsIgnoreCase("wlan0")){
                    continue;
                }
                byte[] macBytes = networkInterface.getHardwareAddress();
                if(macBytes == null){
                    MACAddr_ = "";
                }
                StringBuilder stringBuilder = new StringBuilder();
                for(byte b : macBytes){
                    stringBuilder.append(String.format("%02X:", b));
                }
                if(stringBuilder.length() > 0){
                    stringBuilder.deleteCharAt(stringBuilder.lastIndexOf(":"));
                }
                MACAddr_ = stringBuilder.toString();
            }
        } catch (Exception e){
            Toast.makeText(context_, "There was a problem finding your IP", Toast.LENGTH_SHORT);
            MACAddr_ = "02:00:00:00:00:00";
        }
    }

    public String getIpAddr(){
        return IpAddr_;
    }

    public String getMACAddr(){
        return MACAddr_;
    }
}
