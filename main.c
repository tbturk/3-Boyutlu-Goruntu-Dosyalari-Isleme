#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <conio.h>

struct noktaBilgisiRGB {    //tum datalar icin ortak nokta bilgisi structı
    double i;
    double j;
    double k;
    int r;
    int g;
    int b;

};

struct noktaBilgisiBinaryRGB {  //binary nokta r g b si cekebilmek icin kullanilan fonksiyon.
    int r;
    int g;
    int b;
};

struct kureYarat {  //kullanıcidan alinan kure bilgilerinin tutuldugu kure structinin, struct tanimlamasi
    float x;
    float y;
    float z;
    float r;
};

struct noktaBilgisiXYZ {    //binary nokta xyz si cekebilmek icin kullanilan fonksiyon.
    float x;
    float y;
    float z;

};

int klasoruListele(const char *konum);      // projenin bulundugu klasordeki nkt dosyalarini listeler.
int nktKontrol(const char *konum);  //kendine gonderilen dosya isminin .nkt uzantili olup olmadigini kontrol eder.
int dosyaSecim(char *dosyaAdi);    //Dosya sectirir. Secilen dosyanin ismini dosyaAdina atar.
void dosyaAc(const char *konum);    //parametre olarak aldigi dosya adini read modda acar.
int dosyaKontrol(FILE *dosya,char *dosyaAdi);   //1.isterin fonksiyonu.
int noktaKontrolBinaryXYZ(FILE *dosya,int rgb);// binary dosyalardaki noktalari globalNoktalar struct'ına atar.
int noktaKontrolAsciiXYZ(FILE *dosya);      // ascii xyz dosyasindeki noktalari globalNoktalar struct'ına atar.
int noktaKontrolAsciiRGB(FILE *dosya);      // ascii xyz dosyasindeki noktalari globalNoktalar struct'ına atar.
void karsilamaEkrani();     //islemlerin listelenip ekrana basildigi fonksiyon.
int islemSecim(char *dosyaAdi);     // 1-5 arasi islemleri secme fonksiyonu.
void kureTanimla();//--------------------------------------------------------------------------------------------------
void icindeMi(float xNokta, float yNokta, float zNokta, float xKure, float yKure, float zKure, float rKure);//3. isterin fonksiyonlari.
int kureIciNoktalar(struct kureYarat kure, float xNokta, float yNokta, float zNokta);//---------------------------------------------------------
void noktalarArasiUzaklik(); //2. isterin fonksiyonu.
void noktalarArasiOrtalama();//5.isterin fonksiyonu.
void kupBul(); // 3.isterin fonksiyonu.

FILE *dosya;    //anlik acilan dosyasinin file pointeri.
FILE *output;   // cikti dosyasinin file pointeri.
struct noktaBilgisiRGB *globalNoktalar;     // anlik okunan nkt dosyasinin nokta bilgilerini icerir.
struct kureYarat kure;      //kure verilerini tutar.
int NktDosyaSayisi;     // .nkt uzantili dosyalarin sayisi.
int dosyaKapa;      // islemSecim fonksiyonunda dosyaKontrol yapilmamis ise 2 degerini dondurur.
int noktaSayisiInt;     // dosyadan okuma islemi bittikten sonra nokta veri sayisi ile compare edilecek, eger ki farkliysa hata mesaji donecek.
int rgbMi=3;    // rgb dosyasi olup olmadigini tutar  1:ise xyzrgb 0:ise xyz.
int dataVeri;   // 1 ise ascii 0 ise binary.
int kureSecim=0;    //islemSecim() de kullanicidan küre verisinin bir kere alinmasi icin.(eger deger 1 se kure tekrar alinmaz);



int main(int argc, char **argv) {
    int devam;

    output = fopen("output.nkt","a");
    int karsilama=0;
    int dosyaUzunlugu;
    while(1) {
        dosyaUzunlugu=klasoruListele(".");
        if(NktDosyaSayisi==0) {
            printf("Klasorde NKT Dosyasi Bulunamadi\n");
            break;
        }
        char dosyaAdi[dosyaUzunlugu];
        if(dosyaSecim(dosyaAdi)==0) {
            break;
        }
        fprintf(output,"\t\t%s dosyasi uzerinde calisiliyor...\n",dosyaAdi);
        dosyaAc(dosyaAdi);
        karsilamaEkrani();
        devam=1;
        dosyaKapa=2;//her nkt'de dosyaKontrol'u sorgulamasi icin
        while(devam==1) {
            devam=islemSecim(dosyaAdi);
            if(devam==1&&karsilama==0) {
                karsilamaEkrani();
                karsilama++;
            }
        }
        if(devam==0) {
            fclose(dosya);
            free(globalNoktalar);
            system("CLS");
            printf("\n\n\t\t\t\t%s kapandi\n\n",dosyaAdi);//dosyanin kapandigina dair bilgi dondurur
        }

    }
    fclose(output);
    printf("\t\t\t\t\tProgramdan cikiliyor...");

    return 0;
}


int klasoruListele(const char *konum) {
    struct dirent *entry;  // Klasor(dizin) girisinin pointeri
    NktDosyaSayisi=0;
    int enUzunBoyut=1;
    // opendir() DIR tipinde pointer dondurur.
    DIR *dir = opendir(konum);

    if (dir == NULL) { // opendir Klasor(dizin) acilamazsa NULL dondurur
        printf("Klasor Acilamadi" );
        return 1;
    }
    printf("\t0-Programi Sonlandir\n");
    while ((entry = readdir(dir)) != NULL) {
        if(nktKontrol(entry->d_name)==1) {
            NktDosyaSayisi++;
            if(enUzunBoyut<strlen(entry->d_name))//en uzun dosya isminin boyutunu tutar;
                enUzunBoyut = strlen(entry->d_name);
            if((strcmp(entry->d_name,"output.nkt")!=0)) {
                printf("\t%d-%s\n",NktDosyaSayisi,entry->d_name);
            }


        }
    }
    closedir(dir);
    return enUzunBoyut;
}

int nktKontrol(const char *konum) {
    if(strncmp(konum + strlen(konum) - 4, ".nkt", 4)==0) { //dosya uzantisi nkt mi diye kontrol ediyorx
        return 1;
    } else
        return 0;
}

int dosyaSecim(char *dosyaAdi) {
    strcpy(dosyaAdi,"default");
    int satirNo;
    printf("Islem yapmak istediginiz NKT dosyasinin satir numarasini giriniz:");
    scanf("%d",&satirNo);
    while(satirNo>NktDosyaSayisi||satirNo<0) {
        printf("Yanlis bir deger girdiniz. [1-%d] arasi bir deger giriniz:",NktDosyaSayisi);
        scanf("%d",&satirNo);
    }
    if(satirNo==0){
        return 0;
    }
    struct dirent *entry;
    int i=0;
    DIR *dir = opendir(".");

    if (dir == NULL) {
        printf("Klasor Acilamadi" );
        return 0;
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
    return 1;
}

void dosyaAc(const char *konum) {
    system("CLS");
    dosya = fopen(konum,"r");
    if(dosya==NULL) {
        printf("Dosya Acilamadi!");
    } else {
        printf("\t\t\t %s Dosyasi Uzerinde Islem Yapiliyor...\n\n",konum);
    }

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

        else if(i==2 && strcmp(cumle,n[2])!=0) {// --> rgb degilse xyz olup olmadigini kontrol etmek icin alanTmp 0'a esitlendi.
            alanTmp=0;
        }

        if(i==2 && strcmp(cumle,n[3])!=0) {
            if(alanTmp==0) {// --> AlanTmp 0'sa rgb degildir ayrıca xyz olmadıgı icin hata mesaji dondurukur
                printf("\n%d. satirda baslik bilgisi hatalidir.",i+1);
                fprintf(output,"%d. satirda baslik bilgisi hatalidir.\n",i+1);
                hataSayisi++;
                break;
            }
        }
        if(alanTmp==0)// --> alan bilgilerinde hata yoksa xyz dir
            rgbMi=0;
        else if(alanTmp==1)// --> alan bilgilerinde hata yoksa xyzrgb dir.
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
    fseek(dosya,0,SEEK_SET);
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
                    if(noktaKontrolAsciiRGB(dosya)==1) { //eger nokta kontrol hataliysa dosya kapanir.
                        fputs("Tum dosyalar uyumludur\n",output);
                        return 1;
                    } else {
                        return 0;
                        fputs("Dosyalar uyumlu degildir.\n\n",output);
                    }
                } else if(rgbMi==0) {
                    if(noktaKontrolAsciiXYZ(dosya)==1) { //eger nokta kontrol hataliysa dosya kapanir.
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

    noktaSayisiInt=sayac;

    fseek(dosya,boyut,SEEK_SET);
    char c;
    int dosyaBoyut=0;
    globalNoktalar = (struct noktaBilgisiRGB*)malloc(noktaSayisiInt * sizeof(struct noktaBilgisiRGB));
    while((c=fgetc(dosya))!=EOF) {
        dosyaBoyut++;
    }
    if(rgb==0) {
        if(dosyaBoyut%12!=0) {
            printf("\nBaslik Bilgisi Ile Nokta Sayisi Uyusmuyor\n");
            return 0;
        }//nokta kontrolu hataliysa fonksiyonu bitir.

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
            sayac1++;
        }
        return 1;
    } else if(rgb==1) {
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
            globalNoktalar[sayac1].i=noktalar.x;
            globalNoktalar[sayac1].j=noktalar.y;
            globalNoktalar[sayac1].k=noktalar.z;
            globalNoktalar[sayac1].r=(noktalar1.r%256);
            globalNoktalar[sayac1].g=(noktalar1.g%256);
            globalNoktalar[sayac1].b=(noktalar1.b%256);
            sayac1++;
        }
        return 1;
    }
    return 1;
}

int noktaKontrolAsciiXYZ(FILE *dosya) {
    fseek(dosya,0,SEEK_SET);
    globalNoktalar = (struct noktaBilgisiRGB*)malloc(noktaSayisiInt * sizeof(struct noktaBilgisiRGB));
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
    for(int i=0; i<noktaSayisiInt; i++) {
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
            fprintf(output,"%d. satirda Nokta verisi eksiktir.\n",satir2);
            return 0;
        }

        else if(boslukCount>2) {
            printf("\n%d. satirda Nokta verilerinde fazlalik vardir.",satir2);
            fprintf(output,"%d. satirda Nokta verilerinde fazlalik vardir.\n",satir2);
            return 0;
        }

        satir2++;
    }

    fseek(dosya,boyut-1,SEEK_SET);

    while(getc(dosya)!=EOF) {
        for(int i=0; i<noktaSayisiInt; i++) {
            fscanf(dosya,"%lf %lf %lf\n",&(globalNoktalar[i].i),&(globalNoktalar[i].j),&(globalNoktalar[i].k));
        }
    }
    return 1;

}

int noktaKontrolAsciiRGB(FILE *dosya) {
    fseek(dosya,0,SEEK_SET);
    globalNoktalar = (struct noktaBilgisiRGB*)malloc(noktaSayisiInt * sizeof(struct noktaBilgisiRGB));
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
    for(int i=0; i<noktaSayisiInt; i++) {
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
            fprintf(output,"%d. satirda Nokta verisi r g b bilgileri olmadan verilmistir.\n",satir2);
            return 0;
        }

        else if(boslukCount>5) {
            printf("\n%d. satirda Nokta verilerinde fazlalik vardir.",satir2);
            fprintf(output,"%d. satirda Nokta verilerinde fazlalik vardir.\n",satir2);
            return 0;
        }

        satir2++;
    }


    fseek(dosya,boyut-1,SEEK_SET);

    while(getc(dosya)!=EOF) {
        // The size of globalNoktalar[] will be change when we found the number of globalNoktalar lines.
        for(int i=0; i<noktaSayisiInt; i++) {
            fscanf(dosya,"%lf %lf %lf %d %d %d\n",&(globalNoktalar[i].i),&(globalNoktalar[i].j),&(globalNoktalar[i].k),&(globalNoktalar[i].r),&(globalNoktalar[i].g),&(globalNoktalar[i].b));
        }
    }


    int satir=6;

    for(int i=0; i<noktaSayisiInt; i++) {
        if((globalNoktalar[i].r)<0 || (globalNoktalar[i].r)>255) {
            printf("\n%d. satirda rgb bilgisi hatali",satir);
            fprintf(output,"%d. satirda rgb bilgisi hatali.\n",satir);
            return 0;
        }

        else if((globalNoktalar[i].g)<0 || (globalNoktalar[i].g)>255) {
            printf("\n%d. satirda rgb bilgisi hatali",satir);
            fprintf(output,"%d. satirda rgb bilgisi hatali.\n",satir);
            return 0;
        }

        else if((globalNoktalar[i].b)<0 || (globalNoktalar[i].b)>255) {
            printf("\n%d. satirda rgb bilgisi hatali",satir);
            fprintf(output,"%d. satirda rgb bilgisi hatali.\n",satir);
            return 0;
        }

        satir++;
    }
    return 1;
}

void karsilamaEkrani() {

    printf("\nISLEMLER \n----------------------------------------- \n"
           "	1.) Dosya Kontrolu \n"
           "	2.) En Yakin/Uzak Noktalar \n"
           "	3.) Tum Noktalari Icine Alan Kup \n"
           "	4.) Kurenin Icindeki Noktalar \n"
           "	5.) Nokta Uzakliklari Ortalamasi \n"
           "	6.) Yeni Nkt Dosyasina Gec \n"
           "-----------------------------------------\n"
           "(Secim yapmak istediginiz islemin numarisini girin Or:4):  ");
}

int islemSecim(char *dosyaAdi) {

    int secim;//secim sifir oldugu surece while dongusu calismaya devam eder
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
                if(kureSecim==0) { //kureyi kullanicidan sadece bir kere almak icin.
                    kureTanimla(kure);
                    kureSecim=1;
                }
                system("CLS");
                printf( "cx=%f\n"
                        "cy=%f\n"
                        "cz=%f\n"
                        "cr=%f\n",kure.x,kure.y,kure.z,kure.r);

                fprintf(output,"cx=%f\n"
                        "cy=%f\n"
                        "cz=%f\n"
                        "cr=%f\n",kure.x,kure.y,kure.z,kure.r);
                if(rgbMi==1) {
                    fputs("ALANLAR x y z r g b\n",output);
                } else {
                    fputs("ALANLAR x y z\n",output);
                }

                fprintf(output,"NOKTALAR %d\n",noktaSayisiInt);

                if(dataVeri==1) {
                    fputs("DATA ascii\n",output);
                } else {
                    fputs("DATA binary\n",output);
                }
                for(int i=0; i<noktaSayisiInt; i++) {
                    if(kureIciNoktalar(kure,globalNoktalar[i].i,globalNoktalar[i].j,globalNoktalar[i].k)==1) {
                        if(rgbMi==0) {
                            printf("%f %f %f\n",globalNoktalar[i].i,globalNoktalar[i].j,globalNoktalar[i].k);
                            fprintf(output,"%f %f %f\n",globalNoktalar[i].i,globalNoktalar[i].j,globalNoktalar[i].k);
                        } else if(rgbMi==1) {
                            printf("%f %f %f %d %d %d\n",globalNoktalar[i].i,globalNoktalar[i].j,globalNoktalar[i].k,globalNoktalar[i].r,globalNoktalar[i].g,globalNoktalar[i].b);
                            fprintf(output,"%f %f %f %d %d %d\n",globalNoktalar[i].i,globalNoktalar[i].j,globalNoktalar[i].k,globalNoktalar[i].r,globalNoktalar[i].g,globalNoktalar[i].b);
                        }
                    }
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
        case 6:
            return 0;
        default:
            printf("Yanlis Bir Deger Girdiniz (lutfen [1,5] arasi islem seciniz. Yeni nkt dosyasi icin 6.) \n");
            secim=0;
            break;

        }
        if(secim !=0)//degeri yanlis girmezse switch case den cikiyor
            break;
    }
    if(dosyaKapa==1) {
        return 1;
    } else
        return 0;
}

void kureTanimla() {

    printf("Kurenin merkezinin x koordinatini girin:");
    scanf("%f",&kure.x);
    printf("Kurenin merkezinin y koordinatini girin:");
    scanf("%f",&kure.y);
    printf("Kurenin merkezinin z koordinatini girin:");
    scanf("%f",&kure.z);
    printf("Kurenin yaricapini girin               :");
    scanf("%f",&kure.r);
}

int kureIciNoktalar(struct kureYarat kure, float xNokta, float yNokta, float zNokta) {
    float yaricap = kure.r;
    float noktaninMerkezeUzakligi = sqrt( pow(xNokta-kure.x,2) + pow(yNokta-kure.y,2) + pow(zNokta-kure.z,2) );
    if(noktaninMerkezeUzakligi<=yaricap) {
        return 1;
    } else
        return 0;
}

void noktalarArasiUzaklik() {
    int i,j;
    double karex;
    double karey;
    double karez;
    /*for(int n=0; n<noktaSayisiInt; n++){
        printf("%lf %lf %lf\n",globalNoktalar[n].i,globalNoktalar[n].j,globalNoktalar[n].k);
    }*/
    double baslangicx = pow((globalNoktalar[0].i-globalNoktalar[1].i),2);
    double baslangicy = pow((globalNoktalar[0].j-globalNoktalar[1].j),2);
    double baslangicz = pow((globalNoktalar[0].k-globalNoktalar[1].k),2);
    double enk = sqrt(baslangicx+baslangicy+baslangicz);
    double enb = sqrt(baslangicx+baslangicy+baslangicz);
    int yer1=0;
    int yer2=0;
    int yer3=0;
    int yer4=0;

    //temp about maximum value
    double temp_sayi;
    //temp about minimum value
    //double temp_min;


    for(i=0; i<noktaSayisiInt; i++) {
        for(j=i+1; j<noktaSayisiInt; j++) {
            karex = pow((globalNoktalar[i].i-globalNoktalar[j].i),2);
            karey = pow((globalNoktalar[i].j-globalNoktalar[j].j),2);
            karez = pow((globalNoktalar[i].k-globalNoktalar[j].k),2);
            temp_sayi = sqrt(karex+karey+karez);
            if(enk >= temp_sayi) {
                enk = temp_sayi;
                yer1 = i;
                yer2 = j;

            }
            if(enb <= temp_sayi) {
                enb = temp_sayi;
                yer3 = i;
                yer4 = j;

            }
        }

    }
    if(rgbMi==0) {
        printf("Iki nokta arasi en buyuk uzaklik %lf\n",enb);
        printf("Bu iki noktanin bilgileri;\nIlk nokta bilgileri: x: %lf y: %lf z: %lf\n",globalNoktalar[yer3].i,globalNoktalar[yer3].j,globalNoktalar[yer3].k);
        printf("Ikinci nokta bilgileri: x: %lf y: %lf z: %lf\n\n",globalNoktalar[yer4].i,globalNoktalar[yer4].j,globalNoktalar[yer4].k);


        printf("Iki nokta arasi en kucuk uzaklik %lf\n",enk);
        printf("Bu iki noktanin bilgileri;\nIlk nokta bilgileri: x: %lf y: %lf z: %lf\n",globalNoktalar[yer1].i,globalNoktalar[yer1].j,globalNoktalar[yer1].k);
        printf("Ikinci nokta bilgileri: x: %lf y: %lf z: %lf\n\n",globalNoktalar[yer2].i,globalNoktalar[yer2].j,globalNoktalar[yer2].k);

        fprintf(output,"%lf %lf %lf\n%lf %lf %lf\n",globalNoktalar[yer1].i,globalNoktalar[yer1].j,globalNoktalar[yer1].k,globalNoktalar[yer2].i,globalNoktalar[yer2].j,globalNoktalar[yer2].k);
        fprintf(output,"%lf %lf %lf\n%lf %lf %lf\n",globalNoktalar[yer3].i,globalNoktalar[yer3].j,globalNoktalar[yer3].k,globalNoktalar[yer4].i,globalNoktalar[yer4].j,globalNoktalar[yer4].k);
    } else if(rgbMi==1) {
        printf("Iki nokta arasi en buyuk uzaklik %lf\n",enb);
        printf("Bu iki noktanin bilgileri;\nIlk nokta bilgileri: x: %lf y: %lf z: %lf r: %d g: %d b: %d\n",globalNoktalar[yer3].i,globalNoktalar[yer3].j,globalNoktalar[yer3].k,globalNoktalar[yer3].r,globalNoktalar[yer3].g,globalNoktalar[yer3].b);
        printf("Ikinci nokta bilgileri: x: %lf y: %lf z: %lf r: %d g: %d b: %d\n\n",globalNoktalar[yer4].i,globalNoktalar[yer4].j,globalNoktalar[yer4].k,globalNoktalar[yer4].r,globalNoktalar[yer4].g,globalNoktalar[yer4].b);


        printf("Iki nokta arasi en kucuk uzaklik %lf\n",enk);
        printf("Bu iki noktanin bilgileri;\nIlk nokta bilgileri: x: %lf y: %lf z: %lf r: %d g: %d b: %d\n",globalNoktalar[yer1].i,globalNoktalar[yer1].j,globalNoktalar[yer1].k,globalNoktalar[yer1].r,globalNoktalar[yer1].g,globalNoktalar[yer1].b);
        printf("Ikinci nokta bilgileri: x: %lf y: %lf z: %lf r: %d g: %d b: %d\n\n",globalNoktalar[yer2].i,globalNoktalar[yer2].j,globalNoktalar[yer2].k,globalNoktalar[yer2].r,globalNoktalar[yer2].g,globalNoktalar[yer2].b);

        fprintf(output,"%lf %lf %lf %d %d %d\n",globalNoktalar[yer1].i,globalNoktalar[yer1].j,globalNoktalar[yer1].k,globalNoktalar[yer1].r,globalNoktalar[yer1].g,globalNoktalar[yer1].b);
        fprintf(output,"%lf %lf %lf %d %d %d\n",globalNoktalar[yer2].i,globalNoktalar[yer2].j,globalNoktalar[yer2].k,globalNoktalar[yer2].r,globalNoktalar[yer2].g,globalNoktalar[yer2].b);
        fprintf(output,"%lf %lf %lf %d %d %d\n",globalNoktalar[yer3].i,globalNoktalar[yer3].j,globalNoktalar[yer3].k,globalNoktalar[yer3].r,globalNoktalar[yer3].g,globalNoktalar[yer3].b);
        fprintf(output,"%lf %lf %lf %d %d %d\n",globalNoktalar[yer4].i,globalNoktalar[yer4].j,globalNoktalar[yer4].k,globalNoktalar[yer4].r,globalNoktalar[yer4].g,globalNoktalar[yer4].b);


    }
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

    for(i=0; i<noktaSayisiInt; i++) {
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
