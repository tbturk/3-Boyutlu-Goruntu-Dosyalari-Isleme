#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
//#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <conio.h>

struct noktaBilgisiRGB {
    double i;
    double j;
    double k;
    int r;
    int g;
    int b;

};
struct noktaBilgisiBinaryRGB {
    int r;
    int g;
    int b;
};

struct kureYarat {
    float x;
    float y;
    float z;
    float r;
};

struct noktaBilgisiXYZ {
    float x;
    float y;
    float z;

};

void karsilamaEkrani();
int nktKontrol();
int islemSecim(char *dosyaAdi);
int dosyaKontrol(FILE *dosya,char *dosyaAdi);
void kureTanimla(struct kureYarat *kure);
void icindeMi(float xNokta, float yNokta, float zNokta, float xKure, float yKure, float zKure, float rKure);
void kureIciNoktalar1(struct kureYarat kure, float xNokta, float yNokta, float zNokta);
void kureIciNoktalar2(struct kureYarat kure, float xNokta, float yNokta, float zNokta);
void noktalarArasiUzaklik();
void noktalarArasiOrtalama();
void dosyaAc(const char *konum);
int klasoruListele(const char *konum);
int noktaKontrolAsciiXYZ(FILE *dosya,int noktaSayisi);
int noktaKontrolAsciiRGB(FILE *dosya,int noktaSayisi);
int noktaKontrolBinaryXYZ(FILE *dosya,int rgb);
void dosyaSecim(char *dosyaAdi);//Dosya sectirir. Secilen dosyanýn ismini dosyaAdina atar.
FILE *dosya;
FILE *output;
int NktDosyaSayisi;
int dosyaKapa; // islemSecim fonksiyonunda dosyaKontrol yapilmamis ise 2 degerini dondurur.
struct noktaBilgisiRGB *globalNoktalar;
int noktaSayisiInt;// dosyadan okuma islemi bittikten sonra nokta veri sayisi ile compare edilecek, eger ki farkliysa hata mesaji donecek.
int rgbMi=3; // rgb dosyasi olup olmadigini tutar
int dataVeri;// 1 ise ascii 0 ise binary


int main(int argc, char **argv) {
    int devam;
    int yeniNkt=1;

    output = fopen("output.nkt","a");

    int dosyaUzunlugu;
    while(yeniNkt==1) {
        system("CLS");
        dosyaUzunlugu=klasoruListele(".");
        if(NktDosyaSayisi==0) {
            printf("Klasorde NKT Dosyasi Bulunamadi\n");
            break;
        }
        char dosyaAdi[dosyaUzunlugu];
        dosyaSecim(dosyaAdi);
        fprintf(output,"\t\t%s dosyasi uzerinde calisiliyor...\n",dosyaAdi);
        dosyaAc(dosyaAdi);
        karsilamaEkrani();
        devam=1;
        dosyaKapa=2;//her nkt'de dosyaKontrol'u sorgulamasi icin

        char outputAdi[dosyaUzunlugu+6];
        strcpy(outputAdi,"output");
        strcat(outputAdi,dosyaAdi);
        while(devam==1) {
            devam=islemSecim(dosyaAdi);
            if(devam==1) {
                system("CLS");
                karsilamaEkrani();
            }
        }
        if(devam==0) {
            fclose(dosya);
            free(globalNoktalar);
            printf("\n\n\t\t\t\t%s kapandi\n\n",dosyaAdi);//dosyanin kapandigina dair bilgi dondurur
        }
        printf("Baska Bir nkt Dosyasi Uzerinde Islem Yapmak Icin 1'e basin.");
        scanf("%d",&yeniNkt);

    }
    fclose(output);
    printf("\t\t\t\t\tProgramdan cikiliyor...");

    return 0;
}
int noktaKontrolAsciiXYZ(FILE *dosya,int noktaSayisi) {
    // The size of globalNoktalar[] will be change when we found the number of globalNoktalar lines.

    fseek(dosya,0,SEEK_SET);
    globalNoktalar = (struct noktaBilgisiRGB*)malloc(noktaSayisi * sizeof(struct noktaBilgisiRGB));
    char temp[50];

    int boyut=0;
    for(int i=0; i<5; i++) {
        fgets(temp,50,dosya);
        boyut += strlen(temp);
    }

    char cumle2[100];
    fseek(dosya,boyut+5,SEEK_SET);
    int boslukCount;
    int satir2=6;
    for(int i=0; i<noktaSayisi; i++) {
        fgets(cumle2,100,dosya);
        strtok(cumle2,"\n");
        int cumle_boyut = strlen(cumle2);
        boslukCount=0;
        for(int j=0; j<cumle_boyut; j++) {
            if(cumle2[j] == ' ') {
                boslukCount++; // if(boslukCount != 5) --> hata mesaji dondurecek
            }
        }

        if(boslukCount<2) {
            printf("\n%d. satirda Nokta verisi eksiktir",satir2);
            return 0;
        }

        else if(boslukCount>2) {
            printf("\n%d. satirda Nokta verilerinde fazlalik vardir.",satir2);
            return 0;
        }

        satir2++;
    }

    fseek(dosya,boyut,SEEK_SET);

    while(getc(dosya)!=EOF) {
        // The size of globalNoktalar[] will be change when we found the number of globalNoktalar lines.
        for(int i=0; i<noktaSayisi; i++) {
            fscanf(dosya,"%lf %lf %lf\n",&(globalNoktalar[i].i),&(globalNoktalar[i].j),&(globalNoktalar[i].k));
        }
    }
    return 1;

}
int noktaKontrolAsciiRGB(FILE *dosya,int noktaSayisi) {
    // The size of globalNoktalar[] will be change when we found the number of globalNoktalar lines.
    fseek(dosya,0,SEEK_SET);
    globalNoktalar = (struct noktaBilgisiRGB*)malloc(noktaSayisi * sizeof(struct noktaBilgisiRGB));
    char temp[50];
    int boyut=0;
    for(int i=0; i<5; i++) {
        fgets(temp,50,dosya);
        boyut += strlen(temp);
    }

    char cumle2[100];
    fseek(dosya,boyut+5,SEEK_SET);
    int boslukCount;
    int satir2=6;
    for(int i=0; i<noktaSayisi; i++) {
        fgets(cumle2,100,dosya);
        strtok(cumle2,"\n");
        int cumle_boyut = strlen(cumle2);
        boslukCount=0;
        for(int j=0; j<cumle_boyut; j++) {
            if(cumle2[j] == ' ') {
                boslukCount++; // if(boslukCount != 5) --> hata mesaji dondurecek
            }
        }

        if(boslukCount<5) {
            printf("\n%d. satirda Nokta verisi r g b bilgileri olmadan verilmistir.",satir2);
            return 0;
        }

        else if(boslukCount>5) {
            printf("\n%d. satirda Nokta verilerinde fazlalik vardir.",satir2);
            return 0;
        }

        satir2++;
    }


    fseek(dosya,boyut,SEEK_SET);

    while(getc(dosya)!=EOF) {
        // The size of globalNoktalar[] will be change when we found the number of globalNoktalar lines.
        for(int i=0; i<noktaSayisi; i++) {
            fscanf(dosya,"%lf %lf %lf %d %d %d\n",&(globalNoktalar[i].i),&(globalNoktalar[i].j),&(globalNoktalar[i].k),&(globalNoktalar[i].r),&(globalNoktalar[i].g),&(globalNoktalar[i].b));
        }
    }


    int satir=6;

    for(int i=0; i<noktaSayisi; i++) {
        if((globalNoktalar[i].r)<0 || (globalNoktalar[i].r)>255) {
            printf("\n%d. satirda rgb bilgisi hatali",satir);
            return 0;
        }

        else if((globalNoktalar[i].g)<0 || (globalNoktalar[i].g)>255) {
            printf("\n%d. satirda rgb bilgisi hatali",satir);
            return 0;
        }

        else if((globalNoktalar[i].b)<0 || (globalNoktalar[i].b)>255) {
            printf("\n%d. satirda rgb bilgisi hatali",satir);
            return 0;
        }

        satir++;
    }

    return 1;
}

int dosyaKontrol(FILE *dosya,char *dosyaAdi) {
    int hataSayisi=0;
    int uzunlukAlan;
    char noktaSayisi[10];
    int kontrolAscii = 1;
    int kontrolBinary = 0;
    int cumleBoyut = 0;
    char cumleData[100];

    for(int i=0; i<4; i++) {
        fgets(cumleData,100,dosya);
        strtok(cumleData,"\n");
        cumleBoyut += strlen(cumleData);
    }

    fseek(dosya,cumleBoyut+8,SEEK_SET); // --> Cursor burada binary ascii kontolu yapmak icin DATA satirina atanmistir.
    fgets(cumleData,100,dosya);
    strtok(cumleData,"\n");
    //puts(cumleData); //dosyanin turu hakkinda bilgi verir

    if(strstr(cumleData,"ascii")) {
        kontrolAscii = 1;
        dataVeri = 1;
    } else if(strstr(cumleData,"binary")) {
        kontrolAscii = 0;
        kontrolBinary = 1;
        dataVeri = 0;
    } else {
        hataSayisi++;
    }



    fseek(dosya,0,SEEK_SET); // --> Cursor tekrar dosyanin en basina atanmistir.


    char cumle[100];
    strtok(cumle,"\n");
    char n[7][100] = {"# Noktalar dosya format","VERSION 1","ALANLAR x y z r g b","ALANLAR x y z","NOKTALAR 10000","DATA ascii","DATA binary"};

    int i=0;
    int alanTmp=1;
    int dataTmp=1;
    char bosluk ='\n';
    for(i=0; i<5; i++) { //baslik kontrol ediyor ve rgb ise 1 donduruyor, ayrıca ascii mi binary mi onu donduruyor.
        fgets(cumle,50,dosya);
        strtok(cumle,"\n");

        if((strcmp(cumle,"\n"))==0 || strcmp(cumle," ")==0) {
            printf("\n%d. satirda bilgi hatasi vardir.",i+1);
            fprintf(output,"%d. satirda bilgi hatasi vardir.\n",i+1);
            hataSayisi++;
            break;
        }

        if(i==0 && (strcmp(cumle,n[i])!=0)) {
            printf("\n%d. satirda baslik bilgisi hatalidir.",i+1);
            fprintf(output,"%d. satirda baslik bilgisi hatalidir.\n",i+1);
            hataSayisi++;
            break;
        }

        else if (i==1 && (strcmp(cumle,n[i])!=0)) {
            printf("\n%d. satirda baslik bilgisi hatalidir.",i+1);
            fprintf(output,"%d. satirda baslik bilgisi hatalidir.\n",i+1);
            hataSayisi++;
            break;
        }

        else if(i==2 && strcmp(cumle,n[2])!=0) {
            alanTmp=0;
        }

        if(i==2 && strcmp(cumle,n[3])!=0) {
            if(alanTmp==0) {
                printf("\n%d. satirda baslik bilgisi hatalidir.",i+1);
                fprintf(output,"%d. satirda baslik bilgisi hatalidir.\n",i+1);
                hataSayisi++;
                break;
            }
        }
        if(alanTmp==0)
            rgbMi=0;
        else if(alanTmp==1)
            rgbMi=1;

        if(i==3) {
            for(int k=0; k<8; k++) {
                if(n[4][k]!=cumle[k]) {
                    printf("\n%d. satirda baslik bilgisi hatalidir.",i+1);
                    fprintf(output,"%d. satirda baslik bilgisi hatalidir.\n",i+1);
                    printf("%c %c",n[4][k],cumle[k]);
                    hataSayisi++;
                    break;
                }

            }
            uzunlukAlan=strlen(cumle);
            int n=0;
            for(int k=9; k<uzunlukAlan; k++) {
                noktaSayisi[n]=cumle[k];
                n++;
            }
        }

        if(i==4&& strcmp(cumle,n[5])!=0) {
            dataTmp=0;
        }

        if(i==4 && strcmp(cumle,n[6])!=0) {
            if(dataTmp==0) {
                printf("\n%d. satirda baslik bilgisi hatalidir.",i+1);
                fprintf(output,"%d. satirda baslik bilgisi hatalidir.\n",i+1);
                hataSayisi++;
                break;
            }
        }


    }
    //printf("\nbinary %d\nascii %d",kontrolBinary,kontrolAscii); //dosyanin binary mi ascii mi oldugunu ekrana basar.

    fseek(dosya,0,SEEK_SET);
    //printf("%d",hataSayisi);
    if(hataSayisi==0) {
        if(kontrolAscii==1) {
            int ch;
            int satir_sayisi=1;

            for(ch = fgetc(dosya); ch!=EOF; ch = getc(dosya)) {
                if(ch == '\n') {
                    satir_sayisi++;
                }
            }
            int dosyaNoktaSayisi = satir_sayisi -5;
            noktaSayisiInt = atoi(noktaSayisi);
            if(noktaSayisiInt!=dosyaNoktaSayisi) {
                printf("\nVerilen Nokta Sayilari Hatali");
                fputs("Veriler dosyada verilen sayilarla uyusmamaktadir.\n\n",output);
                hataSayisi++;
            }
            fseek(dosya,0,SEEK_SET);
            if(hataSayisi==0) {
                printf("\nDosyanin Baslik Bilgilerinde Herhangi Bir Hata Yoktur");
                if(rgbMi==1) {
                    if(noktaKontrolAsciiRGB(dosya,noktaSayisiInt)==1) { //eger nokta kontrol hataliysa dosya kapanir.
                        fputs("Tum dosyalar uyumludur\n",output);
                        return 1;
                    } else {
                        return 0;
                        fputs("Dosyalar uyumlu degildir.\n\n",output);
                    }
                } else if(rgbMi==0) {
                    if(noktaKontrolAsciiXYZ(dosya,noktaSayisiInt)==1) { //eger nokta kontrol hataliysa dosya kapanir.
                        fputs("Tum dosyalar uyumludur\n",output);
                        return 1;
                    } else {
                        fputs("Dosyalar uyumlu degildir.\n\n",output);
                        return 0;
                    }
                }

            } else {
                fputs("Dosyalar uyumlu degildir.\n\n",output);
                return 0;
            }
        }

        else if(kontrolBinary==1) {
            fclose(dosya);
            dosya = fopen(dosyaAdi,"rb");
            if(dosya==NULL) {
                printf("Dosya Binary Modunda Tekrar Acilamadi!");
                return 0;
            } else
                printf("Dosya Binary Modunda Tekrar Acildi!");

            if(rgbMi==0) {
                if(noktaKontrolBinaryXYZ(dosya,rgbMi)==1) {
                    fputs("Tum dosyalar uyumludur.\n",output);
                    return 1;
                } else {
                    fputs("Dosyalar uyumlu degildir.\n\n",output);
                    return 0;
                }
            } else if(rgbMi==1) {
                if(noktaKontrolBinaryXYZ(dosya,rgbMi)==1) {
                    fputs("Tum dosyalar uyumludur.\n",output);
                    return 1;
                } else {
                    fputs("Dosyalar uyumlu degildir.\n\n",output);
                    return 0;
                }
            }

            else {
                fputs("Dosyalar uyumlu degildir.\n\n",output);
                return 0;
            }

        }

        fputs("Tum dosyalar uyumludur.\n",output);
        return 1;
    }

    else {
        fputs("Dosyalar uyumlu degildir.\n\n",output);
        return 0;
    }

}

int noktaKontrolBinaryXYZ(FILE *dosya,int rgb) {
    fseek(dosya,0,SEEK_SET);
    int sayac=0;
    char temp[50];
    int boyut=0;
    for(int i=0; i<5; i++) {
        fgets(temp,50,dosya);
        boyut += strlen(temp);
    }
    fseek(dosya,boyut,SEEK_SET);
    if(rgb==0) {
        while(1) {
            struct noktaBilgisiXYZ noktalarIlk;
            size_t sayi = fread(&noktalarIlk,sizeof(struct noktaBilgisiXYZ),1,dosya);
            if(sayi<1)
                break;
            sayac++;
        }
    } else if(rgb==1) {
        while(1) {
            struct noktaBilgisiBinaryRGB noktalarIlk;
            size_t sayi = fread(&noktalarIlk,sizeof(struct noktaBilgisiBinaryRGB),1,dosya);
            if(sayi<1)
                break;
            sayac++;
        }
        sayac/=2;
    }

    printf("\n%d\n",sayac);
    noktaSayisiInt=sayac;

    fseek(dosya,boyut,SEEK_SET);
    char c;
    int dosyaBoyut=0;
    globalNoktalar = (struct noktaBilgisiRGB*)malloc(noktaSayisiInt * sizeof(struct noktaBilgisiRGB));
    while((c=fgetc(dosya))!=EOF) {
        dosyaBoyut++;
    }
    //printf("%d\n",dosyaBoyut);
    if(rgb==0) {
        if(dosyaBoyut%12!=0) {
            printf("\nBaslik Bilgisi Ile Nokta Sayisi Uyusmuyor\n");
            return 0;
        }//nokta kontrolu hataliysa cikar.

        fseek(dosya,0,SEEK_SET);
        fseek(dosya,boyut,SEEK_SET);
        int sayac1=0;
        while(1) {
            struct noktaBilgisiXYZ noktalar;
            size_t sayi = fread(&noktalar,sizeof(struct noktaBilgisiXYZ),1,dosya);
            if(sayi<1)
                break;
            globalNoktalar[sayac1].i=(double)noktalar.x;
            globalNoktalar[sayac1].j=(double)noktalar.y;
            globalNoktalar[sayac1].k=(double)noktalar.z;
            printf("\n%lf %lf %lf\n",globalNoktalar[sayac1].i,globalNoktalar[sayac1].j,globalNoktalar[sayac1].k);
            sayac1++;
        }
        //printf("%d",noktaSayisiInt);
        return 1;
    } else if(rgb==1) {
        printf("%d",dosyaBoyut);
        if(dosyaBoyut%24!=0) {
            printf("\nBaslik Bilgisi Ile Nokta Sayisi Uyusmuyor\n");
            return 0;
        }//nokta kontrolu hataliysa cikar.

        fseek(dosya,boyut,SEEK_SET);
        int sayac1=0;
        for(int dolas=0; dolas<noktaSayisiInt; dolas++) {
            struct noktaBilgisiXYZ noktalar;
            struct noktaBilgisiBinaryRGB noktalar1;
            fread(&noktalar,sizeof(struct noktaBilgisiXYZ),1,dosya);
            fread(&noktalar1,sizeof(struct noktaBilgisiBinaryRGB),1,dosya);

            printf("\n%f %f %f %d %d %d",noktalar.x,noktalar.y,noktalar.z,noktalar1.r,noktalar1.g,noktalar1.b);
            globalNoktalar[sayac1].i=noktalar.x;
            globalNoktalar[sayac1].j=noktalar.y;
            globalNoktalar[sayac1].k=noktalar.z;
            globalNoktalar[sayac1].r=noktalar1.r;
            globalNoktalar[sayac1].g=noktalar1.g;
            globalNoktalar[sayac1].b=noktalar1.b;
            //printf("\n%lf %lf %lf %d %d %d\n",globalNoktalar[sayac1].i,globalNoktalar[sayac1].j,globalNoktalar[sayac1].k,noktalar.r,globalNoktalar[sayac1].g,globalNoktalar[sayac1].b);
            sayac1++;
        }
        printf("%d",noktaSayisiInt);
        return 1;
    }
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
    while(satirNo>NktDosyaSayisi||satirNo<=0) {
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

int islemSecim(char *dosyaAdi) {

    int secim;//secim sifir oldugu surece while dongusu calismaya devam eder
    int devam;
    while(1) {
        scanf("%d", &secim);
        switch(secim) {
        case 1:
            fputs("SECIM 1\n",output);
            dosyaKapa = dosyaKontrol(dosya,dosyaAdi);//dosyada hata varsa dondurecek deger
            break;

        case 2:
            if(dosyaKapa==2) {
                printf("\n\t\t\t\tLutfen Diger Islemlere Gecmeden Once Dosya Kontrolu Yapiniz!"
                       "\n\nSecim yapmak istediginiz islemin numarisini girin (ilk once 1): ");
                secim=0;
                break;
            } else {
                fputs("SECIM 2\n",output);
                noktalarArasiUzaklik();
                break;
            }
        case 3:
            if(dosyaKapa==2) {
                printf("\n\t\t\t\tLutfen Diger Islemlere Gecmeden Once Dosya Kontrolu Yapiniz!"
                       "\n\nSecim yapmak istediginiz islemin numarisini girin (ilk once 1): ");
                secim=0;
                break;
            } else {
                fputs("SECIM 3\n",output);
                kupBul();
                break;
            }
        case 4: {
            if(dosyaKapa==2) {
                printf("\n\t\t\t\tLutfen Diger Islemlere Gecmeden Once Dosya Kontrolu Yapiniz!"
                       "\n\nSecim yapmak istediginiz islemin numarisini girin (ilk once 1): ");
                secim=0;
                break;
            } else {
                fputs("SECIM 4\n",output);
                system("CLS");
                struct kureYarat kure;
                kureTanimla(&kure);
                system("CLS");
                printf( "cx=%f\n"
                        "cy=%f\n"
                        "cz=%f\n"
                        "cr=%f\n",kure.x,kure.y,kure.z,kure.r);

                fprintf(output,"cx=%f\n"
                        "cy=%f\n"
                        "cz=%f\n"
                        "cr=%f\n",kure.x,kure.y,kure.z,kure.r);
                if(rgbMi=1) {
                    fputs("ALANLAR x y z r g b\n",output);
                } else {
                    fputs("ALANLAR x y z\n",output);
                }

                fprintf(output,"NOKTALAR %d\n",noktaSayisiInt);

                if(dataVeri=1) {
                    fputs("DATA ascii\n",output);
                } else {
                    fputs("DATA binary\n",output);
                }
                for(int i=0; i<noktaSayisiInt; i++) {
                    kureIciNoktalar2(kure,globalNoktalar[i].i,globalNoktalar[i].j,globalNoktalar[i].k);
                }
                break;
            }
        }

        case 5:
            if(dosyaKapa==2) {
                printf("\n\t\t\t\tLutfen Diger Islemlere Gecmeden Once Dosya Kontrolu Yapiniz!"
                       "\n\nSecim yapmak istediginiz islemin numarisini girin (ilk once 1): ");
                secim=0;
                break;
            } else {
                fputs("SECIM 5\n",output);
                noktalarArasiOrtalama();
                break;
            }
        default:
            printf("yanlis bir deger girdiniz (lutfen [1,5] arasi deger giriniz) \n");
            secim=0;
            break;

        }
        if(secim !=0)//degeri yanlis girmezse switch case den cikiyor
            break;
    }
    if(dosyaKapa==1) {
        printf("\nYeni Islem Yapmak Icin 1 e basiniz");
        scanf("%d",&devam);
        if(devam!=1) {
            return 0;
        } else
            return 1;
    } else
        return 0;
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

void kureIciNoktalar2(struct kureYarat kure, float xNokta, float yNokta, float zNokta) {
    float yaricap = kure.r;
    float noktaninMerkezeUzakligi = sqrt( pow(xNokta-kure.x,2) + pow(yNokta-kure.y,2) + pow(zNokta-kure.z,2) );
    if(noktaninMerkezeUzakligi<=yaricap) {
        printf("%f %f %f\n",xNokta,yNokta,zNokta);
        fprintf(output,"%f %f %f\n",xNokta,yNokta,zNokta);
    }
}

void noktalarArasiUzaklik() {
    int i,j;
    double karex;
    double karey;
    double karez;
    int toplam_nokta=0;

    double baslangicx = pow((globalNoktalar[0].i-globalNoktalar[1].i),2);
    double baslangicy = pow((globalNoktalar[0].j-globalNoktalar[1].j),2);
    double baslangicz = pow((globalNoktalar[0].k-globalNoktalar[1].k),2);
    double enk = sqrt(baslangicx+baslangicy+baslangicz);
    double enb = sqrt(baslangicx+baslangicy+baslangicz);
    double toplam = 0;

    int yer1=0;
    int yer2=0;
    int yer3=0;
    int yer4=0;

    //temp about maximum value
    double temp_sayi;
    //temp about minimum value
    //double temp_min;


    for(i=1; i<noktaSayisiInt-1; i++) {
        for(j=i+1; j<noktaSayisiInt; j++) {
            karex = pow((globalNoktalar[i].i-globalNoktalar[j].i),2);
            karey = pow((globalNoktalar[i].j-globalNoktalar[j].j),2);
            karez = pow((globalNoktalar[i].k-globalNoktalar[j].k),2);
            temp_sayi = sqrt(karex+karey+karez);
            toplam += temp_sayi;
            toplam_nokta++;
            if(enk > temp_sayi) {
                enk = temp_sayi;
                yer1 = i;
                yer2 = j;

            }
            if(enb < temp_sayi) {
                enb = temp_sayi;
                yer3 = i;
                yer4 = j;

            }
        }

    }

    printf("Iki nokta arasi en buyuk uzaklik %lf\n",enb);
    printf("Bu iki noktanin bilgileri;\nIlk nokta bilgileri: x: %lf y: %lf z: %lf\n",globalNoktalar[yer3].i,globalNoktalar[yer3].j,globalNoktalar[yer3].k);
    printf("Ikinci nokta bilgileri: x: %lf y: %lf z: %lf\n\n",globalNoktalar[yer4].i,globalNoktalar[yer4].j,globalNoktalar[yer4].k);


    printf("Iki nokta arasi en kucuk uzaklik %lf\n",enk);
    printf("Bu iki noktanin bilgileri;\nIlk nokta bilgileri: x: %lf y: %lf z: %lf\n",globalNoktalar[yer1].i,globalNoktalar[yer1].j,globalNoktalar[yer1].k);
    printf("Ikinci nokta bilgileri: x: %lf y: %lf z: %lf\n\n",globalNoktalar[yer2].i,globalNoktalar[yer2].j,globalNoktalar[yer2].k);

    fprintf(output,"%lf %lf %lf\n%lf %lf %lf\n",globalNoktalar[yer1].i,globalNoktalar[yer1].j,globalNoktalar[yer1].k,globalNoktalar[yer2].i,globalNoktalar[yer2].j,globalNoktalar[yer2].k);
    fprintf(output,"%lf %lf %lf\n%lf %lf %lf\n",globalNoktalar[yer3].i,globalNoktalar[yer3].j,globalNoktalar[yer3].k,globalNoktalar[yer4].i,globalNoktalar[yer4].j,globalNoktalar[yer4].k);
}

void noktalarArasiOrtalama() {
    int i,j;
    double karex;
    double karey;
    double karez;
    double toplam_nokta=0;

    double baslangicx = pow((globalNoktalar[0].i-globalNoktalar[1].i),2);
    double baslangicy = pow((globalNoktalar[0].j-globalNoktalar[1].j),2);
    double baslangicz = pow((globalNoktalar[0].k-globalNoktalar[1].k),2);
    double enb = sqrt(baslangicx+baslangicy+baslangicz);
    double temp_max = enb;
    double toplam = enb;

    for(i=1; i<noktaSayisiInt-1; i++) {
        for(j=i+1; j<noktaSayisiInt; j++) {
            karex = pow((globalNoktalar[i].i-globalNoktalar[j].i),2);
            karey = pow((globalNoktalar[i].j-globalNoktalar[j].j),2);
            karez = pow((globalNoktalar[i].k-globalNoktalar[j].k),2);
            temp_max = sqrt(karex+karey+karez);
            toplam += temp_max; // This variable holds the all lengths between the dots.
            toplam_nokta++;     // This variable holds to number of lines.

        }

    }

    double ortalama = toplam/toplam_nokta;
    printf("Butun ikili noktalar arasi uzakliklar ortalamasi %lf",ortalama);
    fprintf(output,"%lf\n",ortalama);
}

void kupBul() {
    double enbx = globalNoktalar[0].i;
    double enby = globalNoktalar[0].j;
    double enbz = globalNoktalar[0].k;
    double enkx = globalNoktalar[0].i;
    double enky = globalNoktalar[0].j;
    double enkz = globalNoktalar[0].k;

    for(int i=1; i<noktaSayisiInt; i++) {
        if(enbx<globalNoktalar[i].i) {
            enbx = globalNoktalar[i].i;
        }
        if(enby<globalNoktalar[i].j) {
            enby = globalNoktalar[i].j;
        }
        if(enbz<globalNoktalar[i].k) {
            enbz = globalNoktalar[i].k;
        }
        if(enkx>globalNoktalar[i].i) {
            enkx = globalNoktalar[i].i;
        }
        if(enky>globalNoktalar[i].j) {
            enky = globalNoktalar[i].j;
        }
        if(enkz>globalNoktalar[i].k) {
            enkz = globalNoktalar[i].k;
        }

    }

    double farkx;
    double farky;
    double farkz;
    double enbuyukFark;

    farkx = fabs(enbx - enkx);
    farky = fabs(enby - enky);
    farkz = fabs(enbz - enkz);

    if(farkx > farky && farkx > farkz) {
        enbuyukFark = farkx;
    } else if(farky > farkx && farky > farkz) {
        enbuyukFark = farky;
    } else if(farkz > farkx && farkz > farky) {
        enbuyukFark = farkz;
    }

    printf("%lf %lf %lf\n",enkx,enky,enkz);
    printf("%lf %lf %lf\n",enkx+enbuyukFark,enky,enkz);
    printf("%lf %lf %lf\n",enkx,enky+enbuyukFark,enkz);
    printf("%lf %lf %lf\n",enkx,enky,enkz+enbuyukFark);
    printf("%lf %lf %lf\n",enbx,enby,enbz);
    printf("%lf %lf %lf\n",enbx-enbuyukFark,enby,enbz);
    printf("%lf %lf %lf\n",enbx,enby-enbuyukFark,enbz);
    printf("%lf %lf %lf\n",enbx,enby,enbz-enbuyukFark);

    fprintf(output,"%lf %lf %lf\n",enkx,enky,enkz);
    fprintf(output,"%lf %lf %lf\n",enkx+enbuyukFark,enky,enkz);
    fprintf(output,"%lf %lf %lf\n",enkx,enky+enbuyukFark,enkz);
    fprintf(output,"%lf %lf %lf\n",enkx,enky,enkz+enbuyukFark);
    fprintf(output,"%lf %lf %lf\n",enbx,enby,enbz);
    fprintf(output,"%lf %lf %lf\n",enbx-enbuyukFark,enby,enbz);
    fprintf(output,"%lf %lf %lf\n",enbx,enby-enbuyukFark,enbz);
    fprintf(output,"%lf %lf %lf\n",enbx,enby,enbz-enbuyukFark);
}

