package com.inf3995.PracticeApp.POJO;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

public class SupervisorIdentificationPOJO {

    @SerializedName("usager")
    @Expose
    private String usager;

    @SerializedName("mot_de_passe")
    @Expose
    private String password;

    public SupervisorIdentificationPOJO(String name, String password) {
        this.usager = name;
        this.password = password;
    }

    public String getUsager() {
        return usager;
    }

    public void setUsager(String usager) {
        this.usager = usager;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public static class NewPasswordPOJO {

        @SerializedName("ancien")
        @Expose
        private String ancien;

        @SerializedName("nouveau")
        @Expose
        private String nouveau;

        public NewPasswordPOJO(String ancien, String nouveau) {
            this.ancien = ancien;
            this.nouveau = nouveau;
        }
    }

}
