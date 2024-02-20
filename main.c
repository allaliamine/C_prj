#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <conio.h>
#include "structure.h"

void faire_demande(Etudiant e){
    //fonction pour faire les demandes
    putchar('\n');
    for(int i=0;i<92;i++)putchar('-');
    printf("\nDEMANDE  \n");
    for(int i=0;i<100;i++)putchar('-');
    putchar('\n');

    int type;
    time_t date_demande;
    do {
        printf("\t\t\tVeuillez saisir le type de demande :\n");
        printf("0:carte etudiant \n1:Assurance\n2:stage\n3:bourse\n");
        scanf("%d", &type);
        if(type<0 || type >3){
            printf("\nchoix invalide!!\n");
        }
    }while(type<0 || type >3);

    for(int i=0;i<92;i++)putchar('-');
    printf("\nDEMANDE ACCEPTEE\n");
    for(int i=0;i<100;i++)putchar('-');
    putchar('\n');

    printf("NOM : %s\t PRENOM : %s\n",e.Nom,e.Prenom);
    printf("Type: ");
    switch (type) {
        case carte:
            printf("Carte d'etudiant\n");
            break;
        case assurance:
            printf("Assurance\n");
            break;
        case stage:
            printf("Stage\n");
            break;
        case bourse:
            printf("Bourse\n");
            break;
        default:
            printf("\nchoix invalide !!");
    }
    time(&date_demande);
    struct tm date = *localtime(&date_demande);
    printf("Date de demande : %d/%02d/%02d a %02d:%02d \n",date.tm_year+1900,date.tm_mon+1,date.tm_mday,date.tm_hour,date.tm_min);
    date.tm_mday+=1;
    printf("Date d'envoi : %d/%02d/%02d a %02d:%02d\n",date.tm_year+1900,date.tm_mon+1,date.tm_mday,date.tm_hour,date.tm_min);
    for(int i=0;i<92;i++)putchar('-');
    putchar('\n');
    printf("tapez entrer pour continuer...");
    getchar();
    getchar();
}
int verifier_CIN(char *CIN){
    //verifier si la chaine respecte le syntaxe de CIN
    if(strlen(CIN)!=7){
        return 0;
    }
    if (isalpha(CIN[0]) && isupper(CIN[0])){
        if(isalpha(CIN[1]) || isdigit(CIN[1])){
            int i=2;
            while(CIN[i]!='\0'){
                if(!isdigit(CIN[i]))return 0;
                i++;
            }
            return 1;
        }
    }
}
int verifier_CNE(char *CNE){
    //verifier si la chaine respecte le syntaxe de CNE
    if(strlen(CNE)!=10)return 0;
    if (isalpha(CNE[0])&& isupper(CNE[0])){
        int i=1;
        while(CNE[i]!='\0'){
            if(!isdigit(CNE[i]))return 0;
            i++;
        }
        return 1;
    }
}
int verifier_TEL(char *TEL){
    //verifier si le numero de telephone ne contient que des chiffres
    int i=0;
    while(TEL[i]!='\0'){
        if(!isdigit(TEL[i])){
            return 0;
        }
        i++;
    }
    return 1;
}
void modifier_fichier(Etudiant e){
    //fonction pour modifer le ficher des etudiant apres avoir effectuer des modif
    FILE *f= fopen(etud_dat,"rb");
    if(f==NULL){
        printf("error");
        exit(0);
    }
    else{
        FILE *temp= fopen("temp.dat","ab");
        if(temp==NULL){
            printf("error1");
            fclose(f);
            exit(0);
        }
        else{
            Etudiant modif;
            while (fread(&modif, sizeof(Etudiant),1,f)){
                if(strcmp(e.CNE,modif.CNE)==0) {
                    fwrite(&e, sizeof(Etudiant), 1, temp);
                }
                else fwrite(&modif, sizeof(Etudiant),1,temp);
            }
            fclose(f);
            fclose(temp);
            remove(etud_dat);
            rename("temp.dat",etud_dat);
        }
    }
}

int comparer_Internat(Etudiant e){
    //fonction pour verifier si l'etudiant possede d'internat
    FILE *f=fopen(internat_txt,"rt");
    if(f==NULL){
        exit(0);
    }
    else{
        char cne[20];
        while(fscanf(f,"%s\n",cne)!=EOF)
            if(strcmp(e.CNE,cne)==0){
                fclose(f);
                return 1;
            }
        fclose(f);
        return 0;
    }
}

int comparer_chambre(Internat internat){
    //fonction pour donner les number des chambers different
    FILE *f= fopen(internat_dat,"rb");
    if(f==NULL){
        exit(0);
    }
    else{
        Internat test;
        while(fread(&test, sizeof(Internat),1,f))
            if(internat.numero_chambre==test.numero_chambre){
                fclose(f);
                return 1;
            }
        fclose(f);
        return 0;
    }

}

void inscription_internat(Etudiant etudiant){
    // fonction pour faire l'inscription a l'internat
    FILE *f= fopen(internat_dat,"ab");

    if(f==NULL){
        exit(0);
    }

    else{
        putchar('\n');
        for(int i=0;i<92;i++)putchar('-');
        printf("\n\t\t\tINSCRIPTION INTERNAT \n");
        for(int i=0;i<100;i++)putchar('-');
        putchar('\n');

        if(comparer_Internat(etudiant)==1){
            Internat internat;
            strcpy(internat.CNE,etudiant.CNE);
            internat.couloir=etudiant.sex;
            do {
                internat.numero_chambre = rand() % 200 + 1;
            }while (comparer_chambre(internat)==1);

            internat.numero_etage=rand()%2+1;

            fwrite(&internat, sizeof(Internat),1,f);
            printf("\t\t\t\t Vous etes inscrit avec suceces \t\t\t\t\n");
            fclose(f);
            return ;
        }

        else{
            printf("Vous n'etes pas dans la liste des admis a l'internat \n");
            fclose(f);
            return ;
        }
    }
}

void rendre_miniscule(char *str){
    for(int i=0;str[i];i++)str[i]= tolower(str[i]);
}

void afficher_internat(Etudiant e){
    //fonction d'affichage d'internat
    FILE *f= fopen(internat_dat,"rb");
    if (f==NULL){
        exit(0);
    }

    if (comparer_Internat(e)==1){

        Internat a;
        while (fread(&a, sizeof(Internat),1,f)){

            if (strcmp(a.CNE,e.CNE)==0){

                putchar('\n');
                for(int i=0;i<92;i++)putchar('-');
                printf("\nCORDONNES D'INTERNANT\n");
                for(int i=0;i<100;i++)putchar('-');
                putchar('\n');

                printf("NOM : %s \b PRENOM : %s \n",e.Nom,e.Prenom);

                if (a.couloir=='f') {
                    printf("Couloir : fille \n");
                }

                else {
                    printf("Couloir : Garcons\n");
                }

                printf("ETAGE : %d\n",a.numero_etage);
                printf("NUMERO DE CHAMBRE : %d \n",a.numero_chambre);

                putchar('\n');
                for(int i=0;i<100;i++)putchar('-');
                putchar('\n');
                fclose(f);
                return;
            }
        }
        printf("AVERTISSEMENT : vous n'etes pas inscrit encore \nVeuillez inscrire s'il vous plait \n");
        fclose(f);
    }
    else if(comparer_Internat(e)==0){
        printf("Vous n'etes pas dans la liste des internat\n");
    }
    fclose(f);
}

void modifier_donnes(Etudiant *e){
    putchar('\n');
    for(int i=0;i<92;i++)putchar('-');
    printf("\n\t\t\tMODIFICATION DU COMPTE\n");

    for(int i=0;i<100;i++)putchar('-');
    putchar('\n');

    FILE *f= fopen("etud.dat","rb");

    if (f == NULL){
        exit(0);
    }
    else{
        Etudiant modif;
        while(fread(&modif, sizeof(Etudiant),1,f)){
            if (strcmp(e->CNE,modif.CNE)==0)
                break;
        }
        int i=4;
        char pass1[20],pass2[20];

        while(i!=3){
            printf("\nVoulez vous modifier :\n");

            do{
                printf("0:Numero de telephone\n1:Email personnelle\n2:Mot de passe \n3:Quitter\nREPONSE: ");
                scanf("%d", &i);
                if(i>3 || i<0){
                    printf("\nchoix invalide !!");
                }
            }while(i>3 || i<0);

            switch (i){
                case Tel:{
                    do {
                        printf("veuillez saisir le nouveau numero du telephone: ");
                        scanf("%s", (e->cordonnes).TEL);
                        if(strlen((e->cordonnes).TEL)!=10){
                            printf("numero de telephone invalide veuillez le re-saisire\n");
                        }
                    }while(strlen((e->cordonnes).TEL)!=10);
                    printf("donner modifier avec succes!\ntapez entrer pour continuer...");
                    getchar();
                    getchar();
                }
                    break;
                case email:{
                    printf("veuillez saisir le nouveau email \n");
                    getchar();
                    gets((e->cordonnes).email_perso);
                    while ((strstr((e->cordonnes).email_perso,"@")==NULL)&&(strstr((e->cordonnes).email_perso,"mail.com")==NULL)){
                        printf("\nERROR : email invalide resaisir ton email: ");
                        gets((e->cordonnes).email_perso);
                    }
                    printf("donner modifier avec succes!\ntapez entrer pour continuer...");
                    getchar();
                    getchar();
                }
                    break;
                case password:{
                    char pass[20];
                    getchar();
                    printf("\ndonner l'ancien mot de passe : ");
                    scanf("%s",pass);
                    if(strcmp((e->cordonnes).mot_de_passe,pass)==0){
                        do {
                            printf("veuillez saisir le nouveau mot de passe \n");
                            scanf("%s", (e->cordonnes).mot_de_passe);
                            printf("Reconfirmer le mot de passe \n");
                            scanf("%s", pass2);
                            if (strcmp((e->cordonnes).mot_de_passe, pass2) != 0) {
                                printf("ERROR : les mots de passes sont incompatibles \n");
                            }
                        } while (strcmp((e->cordonnes).mot_de_passe, pass2) != 0);

                        printf("donner modifier avec succes!\ntapez entrer pour continuer...");
                        getchar();
                        getchar();
                    }
                    else printf("\nmot de pass incorrect !");
                }
                    break;
                default:i=3;
            }
        }
        fclose(f);
        modifier_fichier(*e);
    }
}

void changer_annee(Etudiant *e,char *nom) {
    //fonction pour changer annee d'etude {cp1,cp2,ci1,ci2,ci3}
    strcpy(e->annee, nom);
    modifier_fichier(*e);
}

void changer_specialite(Etudiant *e,char *nom) {
    //fonction pour changer la specialite{tdia(1,2,3),gi(1,2,3)....}
    strcpy(e->specialite, nom);
    modifier_fichier(*e);
}

void modifier_note(Etudiant *e){
//fonction pour donner une note a un etudiant
    FILE *f= fopen(etud_dat,"rb");
    if (f==NULL)exit(0);
    else{
        Etudiant modif;
        while(fread(&modif, sizeof(Etudiant),1,f)){
            if (strcmp(e->CNE,modif.CNE)==0){
                (e->note)=rand()%14+7;
                break;
            }
        }
        modifier_fichier(*e);
        fclose(f);
    }

}

char *cycle1(int i){
    //fonction pour retourner la filiere d'etudiant selon son annee
    switch (i){
        case 0:
            printf("vous etes inscrer en premiere annee transformation digitale et intelligence artificielle");
            return "TDIA1";
            break;
        case 1:
            printf("vous etes inscrer en premier annee ingenierie de donnees");
            return "ID1";
            break;
        case 2:
            printf("vous etes inscrer en premiere annee genie informatique");
            return "GI1";
            break;
        case 3:
            printf("vous etes inscrer en premiere annee genie civil");
            return "GC1";
            break;
        case 4:
            printf("vous etes inscrer en premiere annee genie mecanique");
            return "GM1";
            break;
        case 5:
            printf("vous etes inscrer en premiere annee genie energitique et energie renouvlable");
            return "GEER1";
            break;
        case 6:
            printf("vous etes inscrer en premiere annee genie de l'eau et de l'environement");
            return "GEE1";
            break;
        default:return 0;
    }

}

char *cycle2(int i){
    //fonction pour retourner la filiere d'etudiant selon son annee
    switch (i){
        case 0:
            printf("vous etes inscrer en deuxieme annee transformation digitale et intelligence artificielle");
            return "TDIA2";
            break;
        case 1:
            printf("vous etes inscrer en deuxieme annee ingenierie de donnees");
            return "ID2";
            break;
        case 2:
            printf("vous etes inscrer en deuxieme annee genie informatique");
            return "GI2";
            break;
        case 3:
            printf("vous etes inscrer en deuxieme annee genie civil");
            return "GC2";
            break;
        case 4:
            printf("vous etes inscrer en deuxieme annee genie mecanique");
            return "GM2";
            break;
        case 5:
            printf("vous etes inscrer en deuxieme annee genie energitique et energie renouvlable");
            return "GEER2";
            break;
        case 6:
            printf("vous etes inscrer en deuxieme annee genie de l'eau et de l'environement");
            return "GEE2";
            break;
        default:return 0;
    }
}

char *cycle3(int i){
    //fonction pour retourner la filiere d'etudiant selon son annee
    switch (i){
        case 0:
            printf("vous etes inscrit en troisieme annee transformation digitale et intelligence artificielle");
            return "TDIA3";
            break;
        case 1:
            printf("vous etes inscrit en troisieme annee ingenierie de donnees");
            return "ID3";
            break;
        case 2:
            printf("vous etes inscrit en troisieme annee genie informatique");
            return "GI3";
            break;
        case 3:
            printf("vous etes inscrit en troisieme annee genie civil");
            return "GC3";
            break;
        case 4:
            printf("vous etes inscrit en troisieme annee genie mecanique");
            return "GM3";
            break;
        case 5:
            printf("vous etes inscrit en troisieme annee genie energitique et energie renouvlable");
            return "GEER3";
            break;
        case 6:
            printf("vous etes inscrit en troisieme annee genie de l'eau et de l'environement");
            return "GEE3";
            break;
        default:return 0;
    }
}
char *saisir_mot_de_passe(){
    char *password;
    int i = 0;
    char ch;

    printf("Entrez votre mot de passe : ");

    // Masquer l'entrée de l'utilisateur
    while (1) {
        ch =getch();
        if(ch==13)break;
        else if (ch == '\b' && i > 0) {
            printf("\b \b");
            i--;
        } else {
            password[i++] = ch;
            printf("*");
        }
    }

    password[i] = '\0'; // Ajouter un caractère nul à la fin pour former une chaîne
    return password;
}
void Inscrire(){
    //fonction pour faire l'inscription
    //l'inscription se fait et apres le programme verifie si
    //les donnes exist dans le fichier des etudiant admis

    FILE *f= fopen(etud_txt,"r");
    FILE *p= fopen(etud_dat,"ab");

    if(f==NULL || p==NULL ){
        printf("error inscription");
    }
    else {
        /*inscription de etudiant*/
        Etudiant etudiant,e1;
        printf("veuillez saisir ton nom: ");
        scanf("%s",etudiant.Nom);
        printf("veuillez saisir ton prenom: ");
        scanf("%s",etudiant.Prenom);
        do {
            getchar();
            printf("Sexe : m:Homme  |  f:Femme :");
            scanf("%c", &etudiant.sex);
        }while ((etudiant.sex != 'm') && (etudiant.sex != 'f'));

        printf("\nveuillez saisir ta carte d'identite nationnale: ");
        scanf("%s",etudiant.CIN);
        while (!verifier_CIN(etudiant.CIN)){
            printf("\nle Code CIN devra respecter le syntaxe suivant : <AB12345> ou <A123456>\n");
            printf("Saisir to CIN ");
            scanf("%s",etudiant.CIN);
        }
        printf("Veuillez saisir ton CNE: ");
        scanf("%s",etudiant.CNE);
        while (!verifier_CNE(etudiant.CNE)){
            printf("\nle Code CNE devra respecter le syntaxe suivant : <A123456789>\n");
            printf("Saisir to CNE ");
            scanf("%s",etudiant.CNE);
        }
        printf("\nVeuillez saisir age et lieu de naissance:\n");
        printf("\tAge: ");
        scanf("%d", &etudiant.age);
        if(etudiant.age < 17 || etudiant.age >=26){
            do{
                printf("\nage invalide veuillez re-saisir votre age : ");
                scanf("%d", &etudiant.age);
            }while(etudiant.age < 17 || etudiant.age >25);
        }

        printf("\n\tLieu: ");
        scanf("%s",etudiant.lieu);
        printf("bac:  \n");
        printf("\n\t filiere :\n\t0:Sciences Mathematiques (SMA) \n\t1:Sciences Physiques et chimiques (SPC)\n\t2:Sciences de la vie et de la terre (SVT) ?\n->REPONSE: ");
        scanf("%d", &etudiant.bac_fil);

        while ((etudiant.bac_fil != SMA) && (etudiant.bac_fil != SPC) && (etudiant.bac_fil != SVT)) {
            printf("ERROR : re-saisir s'il vous plait: ");
            scanf("%d", &etudiant.bac_fil);
        }

        printf("\nveuillez saisir tes cordonnees :");
        printf("\n\tNumero de telephone: ");
        scanf("%s",etudiant.cordonnes.TEL);
        while(strlen(etudiant.cordonnes.TEL)!=10||!verifier_TEL(etudiant.cordonnes.TEL)){
            printf("\n\tNumero de telephone invalide veuilllez le re-saisir: ");
            scanf("%s",etudiant.cordonnes.TEL);
        }

        printf("\n\tAdresse : ");
        getchar();
        gets(etudiant.cordonnes.adresse);

        printf("\n\tVeuillez saisir votre email personnelle :");
        scanf("%s",etudiant.cordonnes.email_perso);
        while ((strstr(etudiant.cordonnes.email_perso,"@")==NULL)||(strstr(etudiant.cordonnes.email_perso,"mail.com")==NULL)){
            printf("\nemail invalide (il faut respecter le syntaxe <chaine>@<chaine>mail.com)\nresaisir ton email : ");
            scanf("%s",etudiant.cordonnes.email_perso);
        }

        char verif[30];
        do {
            printf("\nVeuillez saisir ton mon de passe : ");
            scanf("%s", etudiant.cordonnes.mot_de_passe);
            printf("\nRenconfimer le mot de passe : ");
            scanf("%s", verif);
            if (strcmp(verif,etudiant.cordonnes.mot_de_passe)!=0){
                printf("\nLes mots de passe sont  incompatibles\n ");
            }
            else break;
        }while(strcmp(verif,etudiant.cordonnes.mot_de_passe)==0);

        /*creation de l'email academique*/
        char conc[100]="etudiant";
        strcpy(etudiant.cordonnes.email_academique,"vide");
        strcpy(conc,etudiant.Nom);
        strcat(conc, ".");
        strcat(conc,etudiant.Prenom);
        rendre_miniscule(conc);
        strcpy(etudiant.cordonnes.email_academique,conc);
        strcat(etudiant.cordonnes.email_academique,"@etu.uae.ac.ma");
        strcpy(etudiant.annee,"CP1");
        strcpy(etudiant.specialite,"------");


        /*verification de l'existance dans fichier des admis*/
        while (fscanf(f,"%s %s %s %d", e1.CNE, e1.Nom, e1.Prenom, &e1.age) != EOF ) {

            if (!strcmp(e1.CNE, etudiant.CNE) && !strcmp(e1.Nom, etudiant.Nom) && !strcmp(e1.Prenom, etudiant.Prenom) &&
                (e1.age == etudiant.age)) {
                fwrite(&etudiant, sizeof(etudiant), 1, p);
                for (int i = 0; i < 92; i++)putchar('-');
                putchar('\n');
                printf("Vous etes inscrit a l'ecole nationnale des sciences Appliques Al Hoceima \n");
                for (int i = 0; i < 100; i++)putchar('-');
                putchar('\n');
                fclose(p);
                fclose(f);
                //donner une note a l;etudiant apres l'inscription
                modifier_note(&etudiant);

                return;
            }
        }
        printf("vous donnees n'existe pas");
        fclose(p);
        fclose(f);
    }
}

void Afficher_etudiant(Etudiant etudiant){
    //fonction pour afficher un etudiant

    putchar('\n');
    for(int i=0;i<92;i++)putchar('-');
    printf("\nIDENTIFICATEUR DE L\'ETUDIANT \n");
    for(int i=0;i<100;i++)putchar('-');
    putchar('\n');

    printf("\tNOM: %s \t Prenom: %s \n",etudiant.Nom,etudiant.Prenom);
    printf("\tSexe : %c \n\tCIN: %s \n\tCNE: %s\n",etudiant.sex,etudiant.CIN,etudiant.CNE);

    putchar('\n');
    for(int i=0;i<92;i++)putchar('-');
    printf("\nNAISSANCE DE L'ETUDIANT \n");
    for(int i=0;i<100;i++)putchar('-');
    putchar('\n');

    printf("\tage : %d\n",etudiant.age);
    printf("\tLieu de Naissance : %s\n",etudiant.lieu);

    putchar('\n');
    for(int i=0;i<92;i++)putchar('-');
    printf("\nCOORDONNEES DE LETUDIANT \n");
    for(int i=0;i<100;i++)putchar('-');
    putchar('\n');

    printf("\tNumero de telephone: %s\n\tAdresse: %s\n\tEmail personnelle: %s\n\tEmail Academique: %s\n",etudiant.cordonnes.TEL,etudiant.cordonnes.adresse,etudiant.cordonnes.email_perso,etudiant.cordonnes.email_academique);

    putchar('\n');
    for(int i=0;i<92;i++)putchar('-');
    printf("\nFILIERE DE L\'ETUDIANT \n");
    for(int i=0;i<100;i++)putchar('-');
    putchar('\n');

    printf("\tAnnee : %s ",etudiant.annee);
    printf("\tspecialite : %s",etudiant.specialite);

    putchar('\n');
    for(int i=0;i<92;i++)putchar('-');
    printf("\nBAC DE L\'ETUDIANT \n");
    for(int i=0;i<100;i++)putchar('-');
    putchar('\n');

    printf("\tFiliere : ");
    switch (etudiant.bac_fil) {
        case SMA:
            printf("Sciences Mathematiques\n");
            break;
        case SPC:
            printf("Sciences Physiques et Chimiques\n");
            break;
        case SVT:
            printf("Sciences de la vie et de la terre\n");
            break;
    }
    printf("\n\t\t\t\t\t note de cette annee : %.2f",etudiant.note);

}

void choix_fil(Etudiant *etudiant){
    //fonction pour le choix de filere des cp2 selon la note

    int a,cmp=0;

    putchar('\n');
    for(int i=0;i<92;i++)putchar('-');
    printf("\n CHOIX FILIERE POUR LES CP2 \n");
    for(int i=0;i<100;i++)putchar('-');
    putchar('\n');

    do{
        printf("CHOISIR VOTRE FILIERE : ");
        printf("\n\t0: TDIA\n\t1: ID\n\t2: GI\n\t3: GC\n\t4: GM\n\t5: GEER\n\t6: GEE\n");
        scanf("%d",&a);

        switch(a){
            case 0: {
                if (etudiant->note >= 14) {
                    changer_specialite(etudiant, cycle1(a));
                    cmp = 1;
                    break;
                }
                else {
                    printf("vous etes pas eligible pour cette filiere choisir une autre");
                    break;
                }
            }
            case 1: {
                if (etudiant->note >= 14) {
                    changer_specialite(etudiant, cycle1(a));
                    cmp = 1;
                    break;
                }
                else {
                    printf("vous etes pas elegible pour cette filiere choisir une autre");
                    break;
                }
            }
            case 2: {
                if (etudiant->note >= 13.5) {
                    changer_specialite(etudiant, cycle1(a));
                    cmp = 1;
                    break;
                }
                else {
                    printf("vous etes pas elegible pour cette filiere choisir une autre");
                    break;
                }
            }
            case 3: {
                if (etudiant->note >= 13) {
                    changer_specialite(etudiant, cycle1(a));
                    cmp = 1;
                    break;
                }
                else {
                    printf("vous etes pas elegible pour cette filiere choisir une autre");
                    break;
                }
            }
            case 4: {
                if (etudiant->note >= 11) {
                    changer_specialite(etudiant, cycle1(a));
                    cmp = 1;
                    break;
                }
                else {
                    printf("vous etes pas elegible pour cette filiere choisir une autre");
                    break;
                }
            }
            case 5: {
                if (etudiant->note >= 10) {
                    changer_specialite(etudiant, cycle1(a));
                    cmp = 1;
                    break;
                } else {
                    printf("vous etes pas elegible pour cette filiere choisir une autre");
                    break;
                }
            }
            case 6: {
                if (etudiant->note >= 10) {
                    changer_specialite(etudiant, cycle1(a));
                    cmp = 1;
                    break;
                }
                else {
                    printf("vous etes pas elegible pour cette filiere choisir une autre");
                    break;
                }
            }
            default: printf("choix invalide !!");
        }
    }while(cmp != 1);
}

void Reinscription(Etudiant *etudiant) {
    //fonction pour faire la reinscription
    putchar('\n');
    for (int i = 0; i < 92; i++)putchar('-');
    printf("\n REINSCRIPTION \n");
    for (int i = 0; i < 100; i++)putchar('-');
    putchar('\n');

    int i=0;
    //le programme va detecter l'anne automatiquement

    if (strcmp(etudiant->annee, "CP1" ) == 0) i = 1;
    else if (strcmp(etudiant->annee, "CP2") == 0) i = 2;
    else if (strcmp(etudiant->annee, "CI1") == 0) i = 3;
    else if (strcmp(etudiant->annee, "CI2") == 0) i = 4;
    else if (strcmp(etudiant->annee, "CI3") == 0) i = 5;

    int choix=0;

    switch (i) {
        case 1:
            printf("\nvous vient d'inscrire en CP2 \n");
            do {
                printf("\ttapper 1 pour confirmer votre reinscription : ");
                scanf("%d", &choix);
                getchar();
                if (choix==1) {
                    if (etudiant->note < 10) {
                        printf("\ndesole vous etes ajournee ");
                        modifier_note(etudiant);
                        break;
                    } else {
                        printf("\nvotre reinscription a ete bien effectuer \ntapper entrer pour continuer...");
                        changer_annee(etudiant, "CP2");
                        getchar();
                        break;
                    }
                }
            }while(choix != 1 || !isalpha(choix));
            break;
        case 2:
            printf("\nvous vient d'inscrire en CI1\n");
            do {
                printf("\ttapper 1 pour comfirmer votre reinscription : ");
                scanf("%d", &choix);
                getchar();
                if (choix == 1) {
                    if (etudiant->note < 10) {
                        printf("\ndesole vous etes ajournee");
                        modifier_note(etudiant);
                        break;
                    } else {
                        printf("\nvotre reinscription a ete bien effectuer\n");
                        printf("\n");
                        choix_fil(etudiant);
                        changer_annee(etudiant, "CI1");
                        modifier_note(etudiant);
                        printf("\ntapez entrer pour continuer...");
                        getchar();
                        getchar();
                        break;
                    }
                }
            }while(choix != 1 || !isalpha(choix));
            break;

        case 3:
            printf("\nvous vient d'inscrire en CI2\n");
            int j;
            if(strcmp(etudiant->specialite,"TDIA1")==0)j=0;
            else if(strcmp(etudiant->specialite,"ID1")==0)j=1;
            else if(strcmp(etudiant->specialite,"GI1")==0)j=2;
            else if(strcmp(etudiant->specialite,"GC1")==0)j=3;
            else if(strcmp(etudiant->specialite,"GM1")==0)j=4;
            else if(strcmp(etudiant->specialite,"GEER1")==0)j=5;
            else if(strcmp(etudiant->specialite,"GEE1")==0)j=6;
            do {
                printf("\ttapper 1 pour comfirmer votre reinscription : ");
                scanf("%d", &choix);
                getchar();
                if (choix == 1) {
                    if (etudiant->note < 12) {
                        printf("\ndesole vous etes ajournee");
                        modifier_note(etudiant);
                        break;
                    } else {
                        printf("\nvotre reinscription a ete bien effectuer\n");
                        changer_annee(etudiant, "CI2");
                        changer_specialite(etudiant, cycle2(j));
                        modifier_note(etudiant);
                        printf("\ntapez entrer pour continuer...");
                        getchar();
                        break;
                    }
                }
            }while(choix != 1 || !isalpha(choix));
            break;

        case 4:
            printf("\nvous vient d'inscrire en CI3\n");
            int x;
            if(strcmp(etudiant->specialite,"TDIA2")==0)x=0;
            else if(strcmp(etudiant->specialite,"ID2")==0)x=1;
            else if(strcmp(etudiant->specialite,"GI2")==0)x=2;
            else if(strcmp(etudiant->specialite,"GC2")==0)x=3;
            else if(strcmp(etudiant->specialite,"GM2")==0)x=4;
            else if(strcmp(etudiant->specialite,"GEER2")==0)x=5;
            else if(strcmp(etudiant->specialite,"GEE2")==0)x=6;
            do {
                printf("\ttapper 1 pour comfirmer votre reinscription : ");
                scanf("%d", &choix);
                getchar();
                if (choix == 1) {
                    if (etudiant->note < 12) {
                        printf("\ndesole vous etes ajournee\n");
                        modifier_note(etudiant);
                        break;
                    } else {
                        printf("\nvotre reinscription a ete bien effectuer\n");
                        changer_annee(etudiant, "CI3");
                        changer_specialite(etudiant, cycle3(x));
                        modifier_note(etudiant);
                        printf("\ntapez entrer pour continuer...");
                        getchar();
                        break;
                    }
                }
            }while(choix != 1 || !isalpha(choix));
            break;
        case 5:
            printf("\nvous avez terminer vous etudes a L'ENSAH vous pouvez pas reinscrire\n");
            printf("tapper entrer pour continuer...");
            getchar();
            getchar();
            break;
    }
}

int Connecter(Etudiant *etudiant){
    //fonction pour connecter a votre compte
    FILE *f=fopen("etud.dat","rb");
    if (f==NULL){
        exit(0);
    }
    else{
        putchar('\n');
        for(int i=0;i<92;i++)putchar('-');
        printf("\nCONNECTION\n");
        for(int i=0;i<100;i++)putchar('-');
        putchar('\n');

        int j=0;
        char saisir[20]/*mot_de_passe[20]*/;
        printf("Veuillez saisir ton CNE :\n");
        scanf("%s",saisir);
        //printf("Veuillez saisir votre mot de passe :\n",mot_de_passe);
        //scanf("%s",mot_de_passe);
        Etudiant e;
        char *mot_de_passe=saisir_mot_de_passe();
        while (j<3){
            while (fread(&e, sizeof(Etudiant),1,f)){
                if((strcmp(saisir,e.CNE)==0)&&(strcmp(mot_de_passe,e.cordonnes.mot_de_passe)==0)){

                    putchar('\n');
                    for(int i=0;i<92;i++)putchar('-');
                    printf("\nBonjour %s %s . BIENVENUE A L\'ECOLE NATIONALE DES SCIENCES APPLIQUES D\'AL HOCEIMA \n",e.Nom,e.Prenom);
                    for(int i=0;i<100;i++)putchar('-');
                    putchar('\n');

                    *etudiant=e;
                    fclose(f);
                    return 1;
                }
            }
            rewind(f);
            j++;
            printf("\nERROR : CNE ou Mot de passe incorrect,\nATTENTION: il vous reste %d attentatives \n",3-j);
            printf("Veuillez saisir ton CNE \n ");
            scanf("%s",saisir);
            mot_de_passe=saisir_mot_de_passe();
            putchar('\n');
        }
        fclose(f);
        return 0;
    }
}

void choisir() {
    Etudiant etudiant;
    int choix;
    char choice;
    choice='7';
    choix=choice-'0';
    do {
        do {
            printf("\n1: INSCRIPTION\n2: CONNEXION\n3: QUITTER\n-> REPONSE: ");
            scanf(" %c", &choice);
            if (isdigit(choice) && ((choice >= '1') && (choice <= '3'))) {
                choix = choice - '0';
            } else {
                getchar(); // Supprime le caractère non numérique de la saisie
                printf("\nIl faut saisir un nombre compris entre 1 et 3.\n");
                choix = -1; // Réinitialisez la variable de choix pour forcer une nouvelle saisie
            }
        } while (choix < 1 || choix > 3);

        if (choix == 1) {
            Inscrire();
        }
        else if (choix == 2) {
            int i = Connecter(&etudiant);
            if (i == 1) {
                int repo;
                do {
                    putchar('\n');
                    for (int j = 0; j < 92; j++)putchar('-');
                    printf("\t\t\t\nOPTIONS:\n");
                    printf("0:Profile \n1:Reinscription\n2:Modifier le profile\n3:Effectuer une demande\n4:Internat\n5:Deconnecter\n");
                    for (int j = 0; j < 92; j++)putchar('-');
                    putchar('\n');
                    printf("CHOIX : ");
                    scanf("%d", &repo);
                    switch (repo) {
                        case profile:
                            Afficher_etudiant(etudiant);
                            printf("\ntapez entrer pour continuer...");
                            getchar();
                            getchar();
                            break;
                        case reinscription:
                            Reinscription(&etudiant);
                            break;
                        case modification:
                            modifier_donnes(&etudiant);
                            break;
                        case demande:
                            faire_demande(etudiant);
                            break;
                        case internat:{
                            int m;
                            putchar('\n');
                            for(int j=0;j<92;j++)putchar('-');
                            printf("\nINTERNAT \n");
                            for(int j=0;j<100;j++)putchar('-');
                            putchar('\n');
                            do{
                                printf("Appuyer sur \n1: Inscrire \n2: Information\n0: Quitter \n->REPONSE: ");
                                scanf("%d",&m);
                                switch (m) {
                                    case 0:
                                        break;
                                    case 2:
                                        afficher_internat(etudiant);
                                        printf("tapez enterer pour continuer...");
                                        getchar();
                                        getchar();
                                        break;
                                    case 1:
                                        inscription_internat(etudiant);
                                        printf("tapez enterer pour continuer...");
                                        getchar();
                                        getchar();
                                        break;
                                    default:
                                        printf("\nchoix invalide !");
                                        break;
                                }
                            }while ((m==1)||(m==2));
                            break;
                        }
                        case deconnection: {
                            modifier_fichier(etudiant);
                            time_t date_deconnection;
                            time(&date_deconnection);
                            struct tm date =*localtime(&date_deconnection);

                            for(int j=0;j<100;j++)putchar('-');
                            putchar('\n');
                            printf("Vous avez deconnecter, a bientot\n");
                            printf("Date de derniere connection : %d/%02d/%02d a %02d : %02d\n",date.tm_year+1900,date.tm_mon+1,date.tm_mday,date.tm_hour,date.tm_min);
                            for(int j=0;j<100;j++)putchar('-');
                            putchar('\n');
                            getchar();
                            break;
                        }
                    }
                } while (repo != 5);
            }
        }
        else if (choix < 1 || choix >3 )printf("\t\t\t\tchoix invalide !\n");
    }while (choix != 3);
}

int main() {
    putchar('\n');
    for (int i = 0; i < 80; i++)putchar('-');
    printf("\nSITE DE L'ECOLE NATIONALE DES SCIENCES APPLIQUES D'AL HOCEIMA \n");
    for (int i = 0; i < 100; i++)putchar('-');
    putchar('\n');
    choisir();
    return 0;
}
