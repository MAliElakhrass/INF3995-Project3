package com.inf3995.PracticeApp.POJO;


import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class IdentificationParamsPOJO {

    @SerializedName("ip")
    @Expose
    private String ip;
    @SerializedName("MAC")
    @Expose
    private String MAC;
    @SerializedName("nom")
    @Expose
    private String nom;

    public IdentificationParamsPOJO(String ip, String MAC, String nom) {
        this.ip = ip;
        this.MAC = MAC;
        this.nom = nom;
    }

    public String getIp() {
        return ip;
    }

    public void setIp(String ip) {
        this.ip = ip;
    }

    public String getMAC() {
        return MAC;
    }

    public void setMAC(String MAC) {
        this.MAC = MAC;
    }

    public String getNom() {
        return nom;
    }

    public void setNom(String nom) {
        this.nom = nom;
    }
}
