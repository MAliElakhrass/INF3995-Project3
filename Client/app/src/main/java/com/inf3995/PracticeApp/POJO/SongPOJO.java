package com.inf3995.PracticeApp.POJO;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class SongPOJO {

    @SerializedName("No")
    @Expose
    private Integer no;

    @SerializedName("album")
    @Expose
    private String album;

    @SerializedName("artist")
    @Expose
    private String artiste;

    @SerializedName("bitrate")
    @Expose
    private int bitrate;

    @SerializedName("channels")
    @Expose
    private int channels;

    @SerializedName("minutes")
    @Expose
    private int minutes;

    @SerializedName("proposedBy")
    @Expose
    private String proposeePar;

    @SerializedName("sampleRate")
    @Expose
    private int sampleRate;

    @SerializedName("seconds")
    @Expose
    private int seconds;

    @SerializedName("title")
    @Expose
    private String titre;

    @SerializedName("id")
    @Expose
    private String id;

    @SerializedName("IP")
    @Expose
    private String ip;

    @SerializedName("MAC")
    @Expose
    private String MAC;

    public Integer getNo() {
        return no;
    }

    public void setNo(int no) {
        this.no = no;
    }

    public String getAlbum() {
        return album;
    }

    public void  setAlbum(String album) {
        this.album = album;
    }

    public String getArtiste() {
        return artiste;
    }

    public void setArtiste(String artiste) {
        this.artiste = artiste;
    }

    public int getBitrate() {
        return bitrate;
    }

    public void setBitrate(int bitrate) {
        this.bitrate = bitrate;
    }

    public int getChannels() {
        return channels;
    }

    public void setChannels(int channels) {
        this.channels = channels;
    }

    public int getMinutes() {
        return minutes;
    }

    public void setMinutes(int minutes) {
        this.minutes = minutes;
    }

    public String getProposeePar() {
        return proposeePar;
    }

    public void setProposeePar(String proposeePar) {
        this.proposeePar = proposeePar;
    }

    public  int getSampleRate() {
        return sampleRate;
    }

    public void setSampleRate(int sampleRate) {
        this.sampleRate = sampleRate;
    }

    public int getSeconds() {
        return seconds;
    }

    public void setSeconds(int seconds) {
        this.seconds = seconds;
    }

    public String getTitre() {
        return titre;
    }

    public void setTitre(String titre) {
        this.titre = titre;
    }

    public String getId() {
        return id;
    }

    public void setId(String id) {
        this.id = id;
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
}
