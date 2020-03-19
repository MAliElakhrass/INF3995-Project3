package com.inf3995.PracticeApp.POJO;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class IdentificationPOJO {
        @SerializedName("identificateur")
        @Expose
        private String identificateur;

        @SerializedName("message")
        @Expose
        private String message;

        public String getIdentificateur() {
            return identificateur;
        }

        public void setIdentificateur(String identificateur) {
            this.identificateur = identificateur;
        }

        public String getMessage() {
            return message;
        }

        public void setMessage(String message) {
            this.message = message;
        }

}
