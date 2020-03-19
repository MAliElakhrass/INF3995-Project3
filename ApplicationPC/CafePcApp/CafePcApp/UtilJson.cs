using Newtonsoft.Json;
using System;
using System.Collections.Generic;

namespace CafePcApp
{
    public class UtilJson
    {
        public struct Login
        {
            public string usager;
            public string mot_de_passe;

            public Login(string usager, string mdp)
            {
                this.usager = usager;
                this.mot_de_passe = mdp;
            }
        }

        public struct Password
        {
            public string ancien;
            public string nouveau;

            public Password(string ancien, string nouveau)
            {
                this.ancien = ancien;
                this.nouveau = nouveau;
            }
        }

        public struct Logs
        {
            public List<Log> log;
            public Logs(List<Log> log)
            {
                this.log = log;
            }

            public struct Log
            {
                public int no;
                public string info;
                public Log(int no, string info)
                {
                    this.no = no;
                    this.info = info;
                }
            }

            public struct Dernier
            {
                public int dernier;
                public Dernier(int dernier)
                {
                    this.dernier = dernier;
                }
            }
        }

        public struct Statistiques
        {
            public int chansons;
            public int utilisateurs;
            public int elemines;
            public string temps;

            public Statistiques(int chansons, int utilisateurs, int elemines, string temps)
            {
                this.chansons = chansons;
                this.utilisateurs = utilisateurs;
                this.elemines = elemines;
                this.temps = temps;
            }
        }

        public struct SupervisorAccount
        {
            public string usager;
            public string mot_de_passe;

            public SupervisorAccount(string usager, string mot_de_passe)
            {
                this.usager = usager;
                this.mot_de_passe = mot_de_passe;
            }

            public struct Delete
            {
                public string usager;

                public Delete(string usager)
                {
                    this.usager = usager;
                }
            }
        }

        public static string createJsonLogin(Login loginInfo)
        {
            return JsonConvert.SerializeObject(loginInfo);
        }

        public static string createJsonPassword(Password pwd)
        {
            return JsonConvert.SerializeObject(pwd);
        }

        public static string createJsonLastLog(Logs logs)
        {
            try
            {
                return JsonConvert.SerializeObject(new Logs.Dernier(logs.log[logs.log.Count - 1].no + 1));
            }
            catch (Exception e)
            {
                return JsonConvert.SerializeObject(new Logs.Dernier(0));
            }            
        }

        public static string createJsonSupervisorUser(SupervisorAccount sup)
        {
            return JsonConvert.SerializeObject(sup);
        }

        public static string createJsonSupervisorDelete(SupervisorAccount.Delete sup)
        {
            return JsonConvert.SerializeObject(sup);
        }

        public static Logs parseJsonLogs(string json)
        {
            
            try
            {
                int pos = json.LastIndexOf('}');
                json = json.Substring(0, pos + 1);
                dynamic parsedJson = parsedJson = JsonConvert.DeserializeObject(json);
                List<Logs.Log> logsList = new List<Logs.Log>();
                for (int i = 0; i < parsedJson.information.Count; i++)
                {
                    int no = parsedJson.information[i].no;
                    string info = parsedJson.information[i].info;
                    logsList.Add(new Logs.Log(no, info));
                }
                return new Logs(logsList);
            }
            catch (Exception e)
            {
                // json += "]}";
                // parsedJson = JsonConvert.DeserializeObject(json);
            }
            return new Logs(new List<Logs.Log>());                        
        }

        public static Statistiques parseJsonsStatistics(string json)
        {
            int pos = json.LastIndexOf('}');
            json = json.Substring(0, pos + 1);
            dynamic parsedJson = JsonConvert.DeserializeObject(json);
            int chansons = parsedJson.chansons , utilisateurs = parsedJson.utilisateurs, elemines = parsedJson.elemines;
            string temps = parsedJson.temps;
            return new Statistiques(chansons, utilisateurs, elemines, temps);
        }
    }
}
