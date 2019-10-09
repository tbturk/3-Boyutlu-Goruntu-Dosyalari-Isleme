#include <stdio.h>
#include <unistd.h>
//#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <math.h>
#include <windows.h>
//batuhanin yazdigi kisim
//ibr
//alo
struct noktaBilgisi{
	int i,j,k;
	int r,g,b;
};

struct kureYarat{
    float x;
    float y;
    float z;
    float r;
};

void kureTanimla(struct kureYarat *kure){

    printf("Kurenin merkezinin x koordinatini girin:");
    scanf("%f",&kure->x);
    printf("Kurenin merkezinin y koordinatini girin:");
    scanf("%f",&kure->y);
    printf("Kurenin merkezinin z koordinatini girin:");
    scanf("%f",&kure->z);
    printf("Kurenin yaricapini girin               :");
    scanf("%f",&kure->r);
}

void icindeMi(float xNokta, float yNokta, float zNokta, float xKure, float yKure, float zKure, float rKure){
    if(fabs(xNokta) <= xKure+rKure){ //x ekseninin sinirlarini belirledik
        if(fabs(yNokta) <= sqrt( pow(rKure,2) - pow(xNokta,2) ) ){//x'e gore y'nin max alabilecegi degeri belirledik
            if(fabs(zNokta) <= sqrt( pow(rKure,2) - pow(xNokta,2) - pow(yNokta,2)) ){//x ve y'ye gore z'nin max alabilecegi degeri belirledik
                printf("\nx=%f y=%f z=%f",xNokta,yNokta,zNokta);
            }
        }
    }
}

void kureIciNoktalar1(struct kureYarat kure, float xNokta, float yNokta, float zNokta){

    if(kure.x==0 && kure.y==0 && kure.z==0){
        icindeMi(xNokta, yNokta, zNokta, kure.x, kure.y, kure.z, kure.r);
    }

    else if(kure.x==0 && kure.y!=0 && kure.z==0){
        float yNoktaTmp = fabs(yNokta);
        yNoktaTmp -= kure.y;
        icindeMi(xNokta, yNoktaTmp, zNokta, 0, 0, 0, kure.r);

    }

    else if(kure.x!=0 && kure.y==0 && kure.z==0){
        float xNoktaTmp = fabs(xNokta);
        xNoktaTmp -= kure.x;
        icindeMi(xNoktaTmp, yNokta, zNokta, 0, 0, 0, kure.r);
    }

    else if(kure.x==0 && kure.y==0 && kure.z!=0){
        float zNoktaTmp = fabs(zNokta);
        zNoktaTmp -= kure.z;
        icindeMi(xNokta, yNokta, zNoktaTmp, 0, 0, 0, kure.r);
    }

    else if(kure.x!=0 && kure.y!=0 && kure.z!=0){
        float xNoktaTmp = fabs(xNokta);
        xNoktaTmp -= kure.x;
        float yNoktaTmp = fabs(yNokta);
        yNoktaTmp -= kure.y;
        float zNoktaTmp = fabs(zNokta);
        zNoktaTmp -= kure.z;
        icindeMi(xNoktaTmp, yNoktaTmp, zNoktaTmp, 0, 0, 0, kure.r);
    }

    else if(kure.x!=0 && kure.y!=0 && kure.z==0){
        float xNoktaTmp = fabs(xNokta);
        xNoktaTmp -= kure.x;
        float yNoktaTmp = fabs(yNokta);
        yNoktaTmp -= kure.y;
        icindeMi(xNoktaTmp, yNoktaTmp, zNokta, 0, 0, 0, kure.r);
    }

    else if(kure.x!=0 && kure.y==0 && kure.z!=0){


        float xNoktaTmp = fabs(xNokta);
        xNoktaTmp -= kure.x;
        float zNoktaTmp = fabs(zNokta);
        zNoktaTmp -= kure.z;
        icindeMi(xNoktaTmp, yNokta, zNoktaTmp, 0, 0, 0, kure.r);
    }
    else if(kure.x==0 && kure.y!=0 && kure.z!=0){
        float yNoktaTmp = fabs(yNokta);
        yNoktaTmp -= kure.y;
        float zNoktaTmp = fabs(zNokta);
        zNoktaTmp -= kure.z;
        icindeMi(xNokta, yNoktaTmp, zNoktaTmp, 0, 0, 0, kure.r);
    }

}

void kureIciNoktalar2(struct kureYarat kure, float xNokta, float yNokta, float zNokta){
    float yaricap = kure.r;
    float noktaninMerkezeUzakligi = sqrt( pow(xNokta-kure.x,2) + pow(yNokta-kure.y,2) + pow(zNokta-kure.z,2) );
    if(noktaninMerkezeUzakligi<=yaricap){
        printf("\n x=%f y=%f z=%f \n",xNokta,yNokta,zNokta);
    }
}

void karsilamaEkrani();
void islemSecim();
void dosyaKontrol();
void listdir();

int ListDirectoryContents(const char *sDir)
{
    WIN32_FIND_DATA fdFile;
    HANDLE hFind = NULL;

    char sPath[2048];

    //Specify a file mask. *.* = We want everything!
    sprintf(sPath, "%s\\*.*", sDir);

    if((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
    {
        printf("Path not found: [%s]\n", sDir);
        return 0;
    }

    do
    {
        //Find first file will always return "."
        //    and ".." as the first two directories.
        if(strcmp(fdFile.cFileName, ".") != 0
                && strcmp(fdFile.cFileName, "..") != 0)
        {
            //Build up our file path using the passed in
            //  [sDir] and the file/foldername we just found:
            sprintf(sPath, "%s\\%s", sDir, fdFile.cFileName);

            //Is the entity a File or Folder?
            if(fdFile.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
            {
                printf("Directory: %s\n", sPath);
                ListDirectoryContents(sPath); //Recursion, I love it!
            }
            else{
                printf("File: %s\n", sPath);
            }
        }
    }
    while(FindNextFile(hFind, &fdFile)); //Find the next file.

    FindClose(hFind); //Always, Always, clean things up!

    return 1;
}



int main(int argc, char **argv)
{
    ListDirectoryContents("./");

	karsilamaEkrani();
	islemSecim();
    //listdir(".",0);



	return 0;
}



/*
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
*/


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

			case 4:{
			struct kureYarat kure;
			kureTanimla(&kure);
			kureIciNoktalar2(kure,-3,-3,sqrt(6.9));
			break;
			}
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
/*
void dosyaKontrol(){




int uzaklik()//buraya 3 boyutlu noktalar arası uzaklık formülü yazılacak


}
*/
