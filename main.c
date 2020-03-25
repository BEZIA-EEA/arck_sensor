#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lib.h"
//#include"spidevlib.c"

/*  la bibliotheque de qui gere la partie SPI est telecharge du site  https://linux-sunxi.org/SPIdev  */

int main(int argc,char** argv)
{
	// declaration des varibles 
	int Msize,cont=0,i=0,indice=0;
	char ctmp[500];
	char** line;
	double datmp,datmpl=0,speed,Moy;
	// ouverture des fichiers 
	FILE* capts =fopen("capteurs.txt","r+");
	FILE* res =fopen("result_espace.txt","w");
	FILE* res2 =fopen("result_point_virgule.txt","w");
/*
	partie SPI
	file = spi_init("/dev/spidev0.0"); //dev
*/
	if( capts==NULL )
	{
		printf("ereur : impossible d'acceder aux donnees \n");
		exit(1);
	}

	printf("donnez Le nombre d echantillon sur lequel il faut moyenner : ");
	scanf("%d",&Msize);
	// creation d'un tableau dynamique pour garder les derniers echantillons
	double* X= (double*) malloc(Msize*sizeof(double));
	for (int i = 0; i < Msize; i++)
	{
		X[i]=0;
	}
	
	// lecture des 6 premieres lignes
	printf("lecture de l'en-tete \n");
	for(int i=0;i<6;i++)
	{
		fgets (ctmp,sizeof ctmp, capts);
		//printf("lin - %d : %s",i,ctmp);
	}
	// lecture d'une lignes a partir du fichier 
	fgets (ctmp,sizeof ctmp,capts);
	// la fonction spli() transforme la line lu vers un tableau 
	line = split(ctmp,';');
	// affichage sur l'ecran 
	// on prend le 7eme element de la ligne car il contient l'angle phi
	printf("Angle   |  %s | Phi_v   |Moy   \n",line[6]);
	printf("------------------------------------\n");
	// ecriture dans les fichiers :
	// ecriture avec espases comme separateur 
	fprintf(res,"Angle   |  %s | Phi_v   |Moy   \n",line[6]);
	fprintf(res,"------------------------------------\n");
	// ecriture avec point virgule comme separateur
	fprintf(res2,"itr;%s;Phi_v;Moy \n",line[6]);
	// lecture des donnes capteur a partir du fichier 
	while (!feof(capts))
	{
		// lecture d'une line a partir du fichier 
		fgets (ctmp,sizeof ctmp, capts);
		// la fonction split() transforme la line lu vers un tableau 
		line = split(ctmp,';');
		// on prend le 7eme element de la line car il contient l'angle phi
		if (line[6]!=NULL)
			{
				// on increment un compteur pour les lignes lus  
				cont++;
				// on prend le 7eme element et on le convert vers un double avec la fonction atof();
				datmp=atof(line[6]);
				// calcule de la vittese 
				speed=(datmp-datmpl)/0.005;
				datmpl=datmp;
				// calcule du moyene glissant 
				X[indice]=datmp;
				Moy=0;
				for(i=0;i<Msize;i++)
				{
					Moy+=X[i];
					// printf("i = %d X[i] = %f indice = %d    Moy =  %f \n",i,X[i],indice,Moy);
				}
				indice=(indice+1)%Msize;
				Moy=Moy/Msize;
				// afficahe dans l'ecran 
				printf("%3.f     | %.4f   %.4f   %.4f   \n",(float) cont,datmp,speed,Moy);
				// ecriture dans les fichiers :
				// ecriture avec des espases comme separateur 
				fprintf(res,"%3.f     | %.4f   %.4f   %.4f   \n",(float) cont,datmp,speed,Moy);
				// ecriture avec un point virgule comme separateur
				fprintf(res2,"%d;%.4f;%.4f;%.4f\n",cont,datmp,speed,Moy);

				/*
				ecruture de la valeur de line[6] a l'address 0xE60E
				spi_write(0xE6,0x0E,2,line[6],file); 
				*/
			}
	}
	// liberation d'espace alloue
	free(X);
	// fermeture des fichiers 
	fclose(capts);
	fclose(res);
	fclose(res2);
/*
	close(file);
*/
	printf("Appuyer sur une touche pour continuer... \n");
	getchar();
	return 0;
}
