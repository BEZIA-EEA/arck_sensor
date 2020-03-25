#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lib.h"


int main(int argc,char** argv)
{
	int Msize,cont=0,i=0,indice=0;
	char ctmp[500];
	char** line;
	double datmp,datmpl=0,speed,Moy;

	FILE* capts =fopen("capteurs.txt","r+");
	FILE* res =fopen("result.txt","w");

	if( capts==NULL )
	{
		printf("ereur : impossible d'acceder aux données \n");
		exit(1);
	}

	printf("donnez Le nombre d’échantillon sur lequel il faut moyenner : ");
	scanf("%d",&Msize);
	double* X= (double*) malloc(Msize*sizeof(double));
	for (int i = 0; i < Msize; i++)
	{
		X[i]=0;
	}
	
// affichage des ficher
	printf("lecture de l'en-tete \n");
	for(int i=0;i<6;i++)
	{
		fgets (ctmp,sizeof ctmp, capts);
		//printf("lin - %d : %s",i,ctmp);
	}

	fgets (ctmp,sizeof ctmp,capts);
	line = split(ctmp,';');
	printf("Angle   |  %s | Phi_v   |Moy   \n",line[6]);
	printf("------------------------------------\n");

	fprintf(res,"Angle   |  %s | Phi_v   |Moy   \n",line[6]);
	fprintf(res,"------------------------------------\n");

	while (!feof(capts))
	{
		fgets (ctmp,sizeof ctmp, capts);
		line = split(ctmp,';');
		if (line[6]!=NULL)
			{
				cont++;
				datmp=atof(line[6]);
				speed=(datmp-datmpl)/0.005;
				datmpl=datmp;

				X[indice]=datmp;
				Moy=0;
				for(i=0;i<Msize;i++)
				{
					Moy+=X[i];
					//printf("i = %d X[i] = %f indice = %d    Moy =  %f \n",i,X[i],indice,Moy);
				}
				indice=(indice+1)%Msize;
				Moy=Moy/Msize;
				
				printf("%3.f     | %.4f   %.4f   %.4f   \n",(float) cont,datmp,speed,Moy);
				fprintf(res,"%3.f     | %.4f   %.4f   %.4f   \n",(float) cont,datmp,speed,Moy);

			}
	}

	return 0;
}
