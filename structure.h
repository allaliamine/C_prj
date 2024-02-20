//
// Created by Amine on 30/11/2023.
//

#ifndef PROJET_GESTION_STRUCTURE_H
#define PROJET_GESTION_STRUCTURE_H
#ifndef PROJETFINALE_STRUCTURE_H
#define PROJETFINALE_STRUCTURE_H
//chemin des fichiers :
char internat_txt[500]="Internat .txt";
char internat_dat[500]="Internat.dat";
char etud_txt[500]="etud.txt";
char etud_dat[500]="etud.dat";
//
typedef struct Cordonnes{
    char TEL[20];
    char adresse[30];
    char email_perso[100];
    char email_academique[100];
    char mot_de_passe[100];
}Cordonnes;

enum Filiere{SMA,SPC,SVT};

typedef struct Etudiant{
    char Nom[20],Prenom[20];
    char sex;
    char CIN[12];
    char CNE[12];
    int age;
    char lieu[20];
    int bac_fil;
    Cordonnes cordonnes;
    char annee[100];
    float note;
    char specialite[100];
}Etudiant;

enum typeAT{carte,assurance,stage,bourse};
enum modification{Tel,email,password};
typedef struct Internat{
    char CNE[20];
    char couloir;
    int numero_etage;
    int numero_chambre;
}Internat;

enum option{profile,reinscription,modification,demande,internat,deconnection};
enum inter{Quitter,inscrire,Info};


#endif //PROJETFINALE_STRUCTURE_H

#endif //PROJET_GESTION_STRUCTURE_H
