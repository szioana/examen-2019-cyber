#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {

	char oras_1[20];
	char oras_2[20];
	float distanta_cm;
	float distanta;

}masuratoare;

typedef struct {

	int numarator_Km;
	int numitor_cm;
	int nr_masuratori;
	masuratoare *dist;

}harta;

harta citire_harta(const char*nume_fisier)
{

	FILE*pfile_harta = fopen(nume_fisier, "rt");
	if (!pfile_harta)
	{
		printf("Eroare deschidere fisier!\n");
		_getch();
		exit(1);
	}
	harta h1;
	char *vect = (char*)malloc(1025 * sizeof(char));

	fgets(vect, 1024, pfile_harta);
	h1.numarator_Km = atoi(strtok(vect, ":"));
	h1.numitor_cm = atoi(strtok(NULL, "\n"));
	fgets(vect, 1024, pfile_harta);
	h1.nr_masuratori = atoi(strtok(vect, "\n"));

	masuratoare *orase = (masuratoare*)malloc((h1.nr_masuratori + 1) * sizeof(masuratoare));

	int i = 0;

	while (fgets(vect, 1024, pfile_harta))
	{
		strcpy(orase[i].oras1, strtok(vect, "-"));

		strcpy(orase[i].oras2, strtok(NULL, ":"));

		orase[i].distanta_cm = atof(strtok(NULL, "\n"));

		orase[i].distanta = (orase[i].distanta_cm)*(h1.numitor_cm);

		i++;

	}
	h1.dist = orase;
	fclose(pfile_harta);
	return h1;

}

void afisare_harta(harta *h1)
{
	printf("%d:%d\n", h1->numarator_Km, h1->numitor_cm);
	printf("%d\n", h1->nr_masuratori);
	for (int i = 0; i < h1->nr_masuratori; i++)
	{
		printf("%s-%s:%.1f", h1->dist[i].oras1, h1->dist[i].oras2, h1->dist[i].distanta_cm);

		if (i < h1->nr_masuratori - 1)
			printf("\n");

	}
}

void ex_1(harta*h1)
{
	
	for (int i = 0; i < h1->nr_masuratori; i++)
	{
		float D = ((h1->dist[i].distanta_cm)*(h1->numitor_cm)) / 1000;
		printf("%s-%s:%.2f", h1->dist[i].oras1, h1->dist[i].oras2, D);

		if (i < h1->nr_masuratori - 1)
			printf("\n");
	}
	
}

void ex_2(harta *h1, int argc, char*argv[])
{
	if (argc < 1)
	{
		printf("Argumente insuficiente!\n");
		_getch();
		exit(2);
	}
	char*nume_fisier = argv[1]; 
	FILE*pfile_out = fopen(nume_fisier, "wt");

	for (int i = 0; i < h1->nr_masuratori; i++)
	{
		float D = ((h1->dist[i].distanta_cm)*(h1->numitor_cm)) / 1000;
		fprintf(pfile_out,"%s-%s:%.2f", h1->dist[i].oras1, h1->dist[i].oras2, D);

		if (i < h1->nr_masuratori - 1)
			fprintf(pfile_out,"\n");
	}
	fclose(pfile_out);
	printf("Datele au fost afisate in fisier cu succes!\n");

}

int ex_3(harta *h1, harta *h2)
{
	if (h1->nr_masuratori == h2->nr_masuratori)
	{
		for (int i = 0; i < h1->nr_masuratori; i++)
		{
			int ok = 0;
			for (int j = 0; j < h2->nr_masuratori; j++)
			{
				if (strcmp(h1->dist[i].oras1, h2->dist[j].oras1) == 0 && strcmp(h1->dist[i].oras2, h2->dist[j].oras2) == 0)
				{
					ok = 1;
				}
				if (strcmp(h1->dist[i].oras1, h2->dist[j].oras2) == 0 && strcmp(h1->dist[i].oras2, h2->dist[j].oras1) == 0)
				{
					ok = 1;
				}
			}
			if (ok == 0)
			{
				return 0;
			}
		}
	}
	else return 0;
	return 1;
}

int ex_4(harta *h1, harta *h2)
{
	if (ex_3(h1, h2) == 0)
	{
		printf("Nu corespund inregistrarile!\n");
	}
	else
	{
		for (int i = 0; i < h1->nr_masuratori; i++)
		{
			int ok = 0;
			for (int j = 0; j < h2->nr_masuratori; j++)
			{
				if (strcmp(h1->dist[i].oras1, h2->dist[j].oras1) == 0 && strcmp(h1->dist[i].oras2, h2->dist[j].oras2) == 0)
				{
					ok = 1;
					float D1 = ((h1->dist[i].distanta_cm)*(h1->numitor_cm)) / 1000;
					float D2 = ((h2->dist[j].distanta_cm)*(h2->numitor_cm)) / 1000;
					if (D1 != D2)
					{
						printf("%s-%s: %.1f<->%.1f\n", h1->dist[i].oras1, h2->dist[j].oras2, D1, D2);
						return 0;
					}
				}
				if (strcmp(h1->dist[i].oras1, h2->dist[j].oras2) == 0 && strcmp(h1->dist[i].oras2, h2->dist[j].oras1) == 0)
				{
					ok = 1;
					float D1 = ((h1->dist[i].distanta_cm)*(h1->numitor_cm)) / 1000;
					float D2 = ((h2->dist[j].distanta_cm)*(h2->numitor_cm)) / 1000;
					if (D1 != D2)
					{
						printf("%s-%s: %.1f<->%.1f\n", h1->dist[i].oras1, h1->dist[j].oras2, D1, D2);
						return 0;
					}
				}
			}
			if (ok == 0)
			{
				return 0;
			}
		}

	}
	return 1;
}

void concat(char**filelist, int nofiles, char*outputfile)
{

	harta *h = (harta*)malloc((nofiles + 1) * sizeof(harta));
	int nr_dreapta_max = 9999999;
	for (int i = 0; i < nofiles; i++)
	{
		h[i] = citire_harta(filelist[i]);
		if (h[i].numitor_cm < nr_dreapta_max)
		{
			nr_dreapta_max = h[i].numitor_cm;
		}
	}
	harta h_final;
	h_final.numarator_Km = 1;
	h_final.numitor_cm = nr_dreapta_max;

	int nr_masuratori_final = 0;
	masuratoare*control = (masuratoare*)malloc(sizeof(masuratoare));


	for (int i = 0; i < nofiles; i++)
	{
		for (int j = 0; j < h[i].nr_masuratori; j++)
		{
			masuratoare test;
			strcpy(test.oras1, h[i].dist[j].oras1);
			strcpy(test.oras2, h[i].dist[j].oras2);
			test.distanta = h[i].dist[j].distanta;
			int ok = 1;
			for (int k = 0; k < nr_masuratori_final; k++)
			{
				if ((strcmp(test.oras1, control[k].oras1) == 0 && strcmp(test.oras2, control[k].oras2) == 0) || (strcmp(test.oras1, control[k].oras2) == 0 && strcmp(test.oras2, control[k].oras1) == 0))
				{
					ok = 0;
				}
			}
			if (ok == 1)
			{
				control = (masuratoare*)realloc(control, (nr_masuratori_final + 1) * sizeof(masuratoare));
				strcpy(control[nr_masuratori_final].oras1, test.oras1);
				strcpy(control[nr_masuratori_final].oras2, test.oras2);
				control[nr_masuratori_final].distanta = test.distanta / h_final.numitor_cm;
				nr_masuratori_final++;
			}
		}

	}
	h_final.dist = control;
	h_final.nr_masuratori = nr_masuratori_final;
	FILE*pfile_out = fopen(outputfile, "wt");
	fprintf(pfile_out, "%d:%d\n", h_final.numarator_Km, h_final.numitor_cm);
	fprintf(pfile_out, "%d\n", h_final.nr_masuratori);

	for (int i = 0; i < h_final.nr_masuratori; i++)
	{
		fprintf(pfile_out, "%s-%s:%f\n", h_final.dist[i].oras1, h_final.dist[i].oras2, h_final.dist[i].distanta);
	}
	fclose(pfile_out);
	free(control);
	free(h);
	for (int i = 0; i < nofiles; i++)
	{
		free(filelist[i]);
	}
	free(filelist);

}
char**citire_nume_harti(const char*nume_fisier, int &nr_harti)
{
	FILE*pfile_in = fopen(nume_fisier, "rt");
	if (!pfile_in)
	{
		printf("Eroare deschidere fisier!\n");
		_getch();
		exit(1);
	}
	char**filelist = (char**)malloc(sizeof(char*));
	nr_harti = 0;
	char*buffer = (char*)malloc(1025 * sizeof(char));

	while (fgets(buffer, 1024, pfile_in) != NULL)
	{
		filelist = (char**)realloc(filelist, (nr_harti + 1) * sizeof(char*));

		filelist[nr_harti] = _strdup(strtok(buffer, "\n"));
		nr_harti++;

	}
	free(buffer);
	fclose(pfile_in);
	return filelist;
}

void main(int argc, char*argv[])
{
	harta h1 = citire_harta("masuratori1.txt");

	printf("Continutul fisierului este:\n\n");
	afisare_harta(&h1);
	printf("\n\n");

	printf("Exercitiul 1:\n");
	ex_1(&h1);
	printf("\n\n");

	printf("Exercitiul 2:\n");
	ex_2(&h1,argc,argv);
	printf("\n\n");

	printf("Exercitiul 3:\n");
	harta h2 = citire_harta("masuratori2.txt");
	if (ex_3(&h1, &h2) == 0)
	{
		printf("hartile nu sunt la fel!\n");
	}
	else
	{
		printf("hartile sunt identice!\n");
	}
	printf("\n\n");

	printf("Exercitiul 4:\n");
	ex_4(&h1, &h2);
	printf("\n\n");

	printf("Exercitiul 5:\n");
	int nofiles = 0;
	char**filelist = citire_nume_harti("files.txt", nofiles);
	char*outputfile = _strdup("harta_finala.txt");
	concat(filelist, nofiles, outputfile);
	printf("Concatenare efectuata cu succes!\n");
	printf("\n\n");

	_getch();
}
