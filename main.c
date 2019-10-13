#include <stdio.h>
#include <unistd.h>
//#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <conio.h>

struct noktaBilgisi {
    int i,j,k;
    int r,g,b;
};

struct kureYarat {
    float x;
    float y;
    float z;
    float r;
};

struct noktaVerileri {
    double x;
    double y;
    double z;

};

void karsilamaEkrani();
int islemSecim();
int nktKontrol();
int dosyaKontrol();
void kureTanimla(struct kureYarat *kure);
void icindeMi(float xNokta, float yNokta, float zNokta, float xKure, float yKure, float zKure, float rKure);
void kureIciNoktalar1(struct kureYarat kure, float xNokta, float yNokta, float zNokta);
void kureIciNoktalar2(struct kureYarat kure, float xNokta, float yNokta, float zNokta);
void noktalarArasiUzaklik(struct noktaVerileri *data2[],int uzaklik);
void dosyaAc(const char *konum);
int klasoruListele(const char *konum);
void dosyaSecim(char *dosyaAdi);//Dosya sectirir. Secilen dosyanýn ismini dosyaAdina atar.
FILE *dosya;
int NktDosyaSayisi;


int main(int argc, char **argv) {

    int devam;
    int yeniNkt=1;
    int dosyaUzunlugu;
    while(yeniNkt==1) {
        system("CLS");
        dosyaUzunlugu=klasoruListele(".");
        if(NktDosyaSayisi==0){
            printf("Klasorde NKT Dosyasi Bulunamadi\n");
            break;
        }
        char dosyaAdi[dosyaUzunlugu];
        dosyaSecim(dosyaAdi);
        dosyaAc(dosyaAdi);
        karsilamaEkrani();
        devam=1;
        while(devam==1) {
            devam=islemSecim();
            system("CLS");
            karsilamaEkrani();
        }
        if(devam==0) {
            if(fclose(dosya)==0)
                system("CLS");
            printf("\t\t\t\t%s kapandi\n\n",dosyaAdi);
        }
        printf("Baska Bir nkt Dosyasi Uzerinde Islem Yapmak Icin 1'e basin.");
        scanf("%d",&yeniNkt);

    }

    printf("\t\t\t\t\tProgramdan cikiliyor...");

    return 0;
}


int klasoruListele(const char *konum) {
    struct dirent *entry;  // Pointer for directory entry
    NktDosyaSayisi=0;
    int enUzunBoyut=1;
    // opendir() returns a pointer of DIR type.
    DIR *dir = opendir(konum);

    if (dir == NULL) { // opendir returns NULL if couldn't open directory
        printf("Klasor Acilamadi" );
        return 1;
    }

    // Refer http://pubs.opengroup.org/onlinepubs/7990989775/xsh/readdir.html
    // for readdir()

    while ((entry = readdir(dir)) != NULL) {
        if(nktKontrol(entry->d_name)==1) {
            NktDosyaSayisi++;
            if(enUzunBoyut<strlen(entry->d_name))//en uzun dosya isminin boyutunu tutar;
                enUzunBoyut = strlen(entry->d_name);
            printf("\t%d)%s\n",NktDosyaSayisi,entry->d_name);

        }
    }
    closedir(dir);
    return enUzunBoyut;
}

void dosyaSecim(char *dosyaAdi) {
    strcpy(dosyaAdi,"default");
    int satirNo;
    printf("Islem yapmak istediginiz NKT dosyasinin satir numarasini giriniz:");
    scanf("%d",&satirNo);
    while(satirNo>NktDosyaSayisi||satirNo<=0){
        printf("Yanlis bir deger girdiniz. [1-%d] arasi bir deger giriniz:",NktDosyaSayisi);
        scanf("%d",&satirNo);
    }
    struct dirent *entry;  // Pointer for directory entry
    int i=0;
    // opendir() returns a pointer of DIR type.
    DIR *dir = opendir(".");

    if (dir == NULL) { // opendir returns NULL if couldn't open directory
        printf("Klasor Acilamadi" );
        return;
    }
    while ((entry = readdir(dir)) != NULL) {

        if(nktKontrol(entry->d_name)==1) {
            i++;
            if(i==satirNo) {
                strcpy(dosyaAdi,entry->d_name);
            }
        }
    }
    closedir(dir);
}

void dosyaAc(const char *konum) { //iki okuma modunda da acmayi deneyecek, null deger donderirse diger modda okuyacak.
    system("CLS");
    dosya = fopen(konum,"r");
    if(dosya==NULL) {
        printf("Dosya Acilamadi!");
    } else {
        printf("\t\t\t %s Dosyasi Uzerinde Islem Yapiliyor...\n\n",konum);
    }

}

void karsilamaEkrani() {

    printf("ISLEMLER \n----------------------------------------- \n"
           "	1.) Dosya Kontrolu \n"
           "	2.) En Yakin/Uzak Noktalar \n"
           "	3.) Tum Noktalari Icine Alan Kup \n"
           "	4.) Kurenin Icindeki Noktalar \n"
           "	5.) Nokta Uzakliklari Ortalamasi \n"
           "-----------------------------------------\n"
           "(Secim yapmak istediginiz islemin numarisini girin Or:4):  ");
}

int nktKontrol(const char *konum) {
    if(strncmp(konum + strlen(konum) - 4, ".nkt", 4)==0) { //dosya uzantisi nkt mi diye kontrol ediyor
        //dosyaAc(konum);
        //printf("%s\n", konum);
        return 1;
    } else
        return 0;
}

int islemSecim() {

    int secim;
    int devam;
    while(1) {
        scanf("%d", &secim);
        switch(secim) {
        case 1:
            printf("1 secilmistir \n");
            break;

        case 2:
            printf("2 secilmistir \n");
            break;

        case 3:
            printf("3 secilmistir \n");
            break;

        case 4: {
            system("CLS");
            struct kureYarat kure;
            kureTanimla(&kure);
            system("CLS");
            printf( "cx=%f\n"
                    "cy=%f\n"
                    "cz=%f\n"
                    "cr=%f\n",kure.x,kure.y,kure.z,kure.r);
            /*for(int i=0; i<satirSayisi; i++) {
                kureIciNoktalar2(kure,data[i].x,data[i].y,data[i].z);
            }*/
            break;
        }

        case 5:
            printf("5 secilmistir \n");
            break;

        default:
            printf("yanlis bir deger girdiniz (lutfen [1,5] arasi deger giriniz) \n");
            secim=0;
            break;

        }
        if(secim !=0)//degeri yanlis girmezse switch case den cikiyor
            break;
    }
    printf("\nYeni Islem Yapmak Icin 1 e basiniz");
    scanf("%d",&devam);
    if(devam!=1) {
        return 0;
    }
    return 1;
}


int dosyaKontrol(FILE *dosya) {
    int i=0;
    char a[50];
    while(!feof(dosya)) {
        i++;
        if(fgets(a,50,dosya)!="null");
        printf("%s",a);
    }
    return 1;
}

void kureTanimla(struct kureYarat *kure) {

    printf("Kurenin merkezinin x koordinatini girin:");
    scanf("%f",&kure->x);
    printf("Kurenin merkezinin y koordinatini girin:");
    scanf("%f",&kure->y);
    printf("Kurenin merkezinin z koordinatini girin:");
    scanf("%f",&kure->z);
    printf("Kurenin yaricapini girin               :");
    scanf("%f",&kure->r);
}

void kureIciNoktalar1(struct kureYarat kure, float xNokta, float yNokta, float zNokta) {



    if(kure.x==0 && kure.y==0 && kure.z==0) {
        icindeMi(xNokta, yNokta, zNokta, kure.x, kure.y, kure.z, kure.r);
    }

    else if(kure.x==0 && kure.y!=0 && kure.z==0) {
        float yNoktaTmp = fabs(yNokta);
        yNoktaTmp -= kure.y;
        icindeMi(xNokta, yNoktaTmp, zNokta, 0, 0, 0, kure.r);

    }

    else if(kure.x!=0 && kure.y==0 && kure.z==0) {
        float xNoktaTmp = fabs(xNokta);
        xNoktaTmp -= kure.x;
        icindeMi(xNoktaTmp, yNokta, zNokta, 0, 0, 0, kure.r);
    }

    else if(kure.x==0 && kure.y==0 && kure.z!=0) {
        float zNoktaTmp = fabs(zNokta);
        zNoktaTmp -= kure.z;
        icindeMi(xNokta, yNokta, zNoktaTmp, 0, 0, 0, kure.r);
    }

    else if(kure.x!=0 && kure.y!=0 && kure.z!=0) {
        float xNoktaTmp = fabs(xNokta);
        xNoktaTmp -= kure.x;
        float yNoktaTmp = fabs(yNokta);
        yNoktaTmp -= kure.y;
        float zNoktaTmp = fabs(zNokta);
        zNoktaTmp -= kure.z;
        icindeMi(xNoktaTmp, yNoktaTmp, zNoktaTmp, 0, 0, 0, kure.r);
    }

    else if(kure.x!=0 && kure.y!=0 && kure.z==0) {
        float xNoktaTmp = fabs(xNokta);
        xNoktaTmp -= kure.x;
        float yNoktaTmp = fabs(yNokta);
        yNoktaTmp -= kure.y;
        icindeMi(xNoktaTmp, yNoktaTmp, zNokta, 0, 0, 0, kure.r);
    }

    else if(kure.x!=0 && kure.y==0 && kure.z!=0) {



        float xNoktaTmp = fabs(xNokta);
        xNoktaTmp -= kure.x;
        float zNoktaTmp = fabs(zNokta);
        zNoktaTmp -= kure.z;
        icindeMi(xNoktaTmp, yNokta, zNoktaTmp, 0, 0, 0, kure.r);
    } else if(kure.x==0 && kure.y!=0 && kure.z!=0) {
        float yNoktaTmp = fabs(yNokta);
        yNoktaTmp -= kure.y;
        float zNoktaTmp = fabs(zNokta);
        zNoktaTmp -= kure.z;
        icindeMi(xNokta, yNoktaTmp, zNoktaTmp, 0, 0, 0, kure.r);
    }

}

void icindeMi(float xNokta, float yNokta, float zNokta, float xKure, float yKure, float zKure, float rKure) {
    if(fabs(xNokta) <= xKure+rKure) { //x ekseninin sinirlarini belirledik
        if(fabs(yNokta) <= sqrt( pow(rKure,2) - pow(xNokta,2) ) ) { //x'e gore y'nin max alabilecegi degeri belirledik
            if(fabs(zNokta) <= sqrt( pow(rKure,2) - pow(xNokta,2) - pow(yNokta,2)) ) { //x ve y'ye gore z'nin max alabilecegi degeri belirledik
                printf("x=%f y=%f z=%f",xNokta,yNokta,zNokta);
            }
        }
    }
}

void kureIciNoktalar2(struct kureYarat kure, float xNokta, float yNokta, float zNokta) {
    float yaricap = kure.r;
    float noktaninMerkezeUzakligi = sqrt( pow(xNokta-kure.x,2) + pow(yNokta-kure.y,2) + pow(zNokta-kure.z,2) );
    if(noktaninMerkezeUzakligi<=yaricap) {
        printf("%f %f %f\n",xNokta,yNokta,zNokta);
    }
}


void noktalarArasiUzaklik(struct noktaVerileri *data2[],int uzaklik) {
    int i,j;
    double karex;
    double karey;
    double karez;
    int toplam_nokta=0;

    double baslangicx = pow((data2[0]->x-data2[1]->x),2.0);
    double baslangicy = pow((data2[0]->y-data2[1]->y),2.0);
    double baslangicz = pow((data2[0]->z-data2[1]->z),2.0);
    double enk = sqrt(baslangicx+baslangicy+baslangicz);
    double enb = sqrt(baslangicx+baslangicy+baslangicz);
    double toplam = enb;

    int yer1=0;
    int yer2=0;
    int yer3=0;
    int yer4=0;

    //temp about maximum value
    double temp_max;
    //temp about minimum value
    double temp_min;


    for(i=1; i<uzaklik; i++) {
        for(j=i+1; j<uzaklik; j++) {
            karex = pow((data2[i]->x-data2[j]->x),2.0);
            karey = pow((data2[i]->y-data2[j]->y),2.0);
            karez = pow((data2[i]->z-data2[j]->z),2.0);
            temp_min = sqrt(karex+karey+karez);
            temp_max = sqrt(karex+karey+karez);
            toplam += temp_max;
            toplam_nokta++;
            if(enk > temp_min) {
                enk = temp_min;
                yer1 = i;
                yer2 = j;

            }
            if(enb < temp_max) {
                enb = temp_max;
                yer3 = i;
                yer4 = j;

            }

        }

    }

    printf("Iki nokta arasi en buyuk uzaklik %lf\n",enb);
    printf("Bu iki noktanin bilgileri;\nIlk nokta bilgileri: x: %lf y: %lf z: %lf\n",data2[yer1]->x,data2[yer1]->y,data2[yer1]->z);
    printf("Ikinci nokta bilgileri: x: %lf y: %lf z: %lf\n\n",data2[yer2]->x,data2[yer2]->y,data2[yer2]->z);


    printf("Iki nokta arasi en kucuk uzaklik %lf\n",enk);
    printf("Bu iki noktanin bilgileri;\nIlk nokta bilgileri: x: %lf y: %lf z: %lf\n",data2[yer3]->x,data2[yer3]->y,data2[yer3]->z);
    printf("Ikinci nokta bilgileri: x: %lf y: %lf z: %lf\n\n",data2[yer4]->x,data2[yer4]->y,data2[yer4]->z);

}

void noktalarArasiOrtalama(struct noktaVerileri *data3[],int uzaklik) {
    int i,j;
    double karex;
    double karey;
    double karez;
    int toplam_nokta=0;

    double baslangicx = pow((data3[0]->x-data3[1]->x),2.0);
    double baslangicy = pow((data3[0]->y-data3[1]->y),2.0);
    double baslangicz = pow((data3[0]->z-data3[1]->z),2.0);
    double enb = sqrt(baslangicx+baslangicy+baslangicz);
    double temp_max = enb;
    double toplam = enb;

    for(i=1; i<uzaklik; i++) {
        for(j=i+1; j<uzaklik; j++) {
            karex = pow((data3[i]->x-data3[j]->x),2.0);
            karey = pow((data3[i]->y-data3[j]->y),2.0);
            karez = pow((data3[i]->z-data3[j]->z),2.0);
            temp_max = sqrt(karex+karey+karez);
            toplam += temp_max; // This variable holds the all lengths between the dots.
            toplam_nokta++;     // This variable holds to number of lines.

        }

    }

    double ortalama = toplam/toplam_nokta; // This variable holds the avarage of all lengths between the dots.

    printf("Butun ikili noktalar arasi uzakliklar ortalamasi: %lf",ortalama);

}
