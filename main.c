#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
//batuhanin yazdigi kisim
//ibr
//alo
struct noktaBilgisi{
	int i,j,k;
	int r,g,b;
};

void karsilamaEkrani();
void islemSecim();
void dosyaKontrol();
void listdir();


int main(int argc, char **argv)
{
	karsilamaEkrani();
	islemSecim();
	listdir("../",0);



	return 0;
}




void listdir(const char *name, int indent)
{
    DIR *dir;
    struct dirent *entry;

    if (!(dir = opendir(name)))
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", name, entry->d_name);
            printf("%*s[%s]\n", indent, "", entry->d_name);
            listdir(path, indent + 2);
        } else {
            printf("%*s- %s\n", indent, "", entry->d_name);
        }
    }
    closedir(dir);
}



void karsilamaEkrani(){
	printf("Lutfen bir secim yapiniz: \n----------------------------------------- \n"
	"	1.) Dosya Kontrolu \n"
	"	2.) En Yakin/Uzak Noktalar \n"
	"	3.) Tum Noktalari Icine Alan Küp \n"
	"	4.) Kurenin Icindeki Noktalar \n"
	"	5.) Nokta Uzakliklari Ortalamasi \n"
	"-----------------------------------------\n"
	"(Secim yapmak istediginiz isin numarisini girin Or:4): \n");
}
void islemSecim(){
	int secim;
	while(1){
		scanf("%d", &secim);
		switch(secim){
			case 1: printf("1 secilmistir \n");
			break;

			case 2: printf("2 secilmistir \n");
			break;

			case 3: printf("3 secilmistir \n");
			break;

			case 4: printf("4 secilmistir \n");
			break;

			case 5: printf("5 secilmistir \n");
			break;

			default: printf("yanlis bir deger girdiniz (lutfen [1,5] arasi deger giriniz) \n");
			secim=0;
			break;

		}
		if(secim !=0)
			break;
	}
}

void dosyaKontrol(){

<<<<<<< Updated upstream


int uzaklik()//buraya 3 boyutlu noktalar arası uzaklık formülü yazılacak
//https://fcvideo.info/20-analitik-geometri-uzayda-vekt%C3%B6rler-%C3%BC%C3%A7-boyutlu-uzayda-iki-nokta-aras%C4%B1ndaki-uzakl%C4%B1k_FanClubyeRncNjCC0I.html
=======
}
>>>>>>> Stashed changes
