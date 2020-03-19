package com.inf3995.PracticeApp.POJO;

import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

import java.util.ArrayList;
import java.util.List;

public class BlackListUserPOJO {
    @SerializedName("bloques")
    @Expose
    private User[] userBlocked;

    @SerializedName("users")
    @Expose
    private User[] users;

    public BlackListUserPOJO(User[] userBlocked) {
        this.userBlocked = userBlocked;
    }

    public User[] getUserBlocked() {
        return userBlocked;
    }

    public User[] getUserIndentified() {
        return users;
    }

    public List<User> getAsList() {
        List<User> list = new ArrayList<User>();
        if (userBlocked != null) {
            for (User user: userBlocked) {
                list.add(user);
            }
        }

        return list;
    }

    public List<User> getUserIdentifiedAsList() {
        List<User> list = new ArrayList<User>();
        if (users != null) {
            for (User user: users) {
                list.add(user);
            }
        }

        return list;
    }

    public static class User {
        @SerializedName("ip")
        @Expose
        private String ip;

        @SerializedName("MAC")
        @Expose
        private String MAC;

        @SerializedName("nom")
        @Expose
        private String nom;

        public User(String nom, String ip, String MAC) {
            this.ip = ip;
            this.MAC = MAC;
            this.nom = nom;
        }

        public String getNom() {
            return nom;
        }

        public String getIp() {
            return ip;
        }

        public String getMAC() {
            return MAC;
        }
    }

    public static class MAC {
        @SerializedName("MAC")
        @Expose
        private String mac;

        public MAC (String mac){
            this.mac = mac;
        }
    }
}
