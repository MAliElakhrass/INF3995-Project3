package com.inf3995.PracticeApp.POJO;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class SoundPOJO {
    @SerializedName("volume")
    @Expose
    private Integer volume;

    @SerializedName("sourdine")
    @Expose
    private Boolean sourdine;

    public SoundPOJO(int volume, boolean sourdine) {
        this.volume = volume;
        this.sourdine = sourdine;
    }

    public long getVolume() {
        return volume;
    }

    public boolean isSourdine() {
        return sourdine;
    }
}
