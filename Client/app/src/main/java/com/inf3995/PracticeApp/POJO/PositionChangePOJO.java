package com.inf3995.PracticeApp.POJO;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class PositionChangePOJO {
    @SerializedName("ancien")
    @Expose
    private int oldPosition;

    @SerializedName("nouveau")
    @Expose
    private int newPosition;

    public PositionChangePOJO(int oldPosition, int newPosition) {
        this.oldPosition = oldPosition;
        this.newPosition = newPosition;
    }

    public int getNewPosition() {
        return newPosition;
    }

    public void setNewPosition(int newPosition) {
        this.newPosition = newPosition;
    }

    public int getOldPosition() {
        return oldPosition;
    }

    public void setOldPosition(int oldPosition) {
        this.oldPosition = oldPosition;
    }
}
