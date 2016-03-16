#include <stdio.h>
#include <stdlib.h>

/* a = 1
 * -a=2
 * b=3
 * -b=4
 * ...
 */

 // etat_var combien de valeurs essayées pr une variable (vecteur 1D à 0 au début)
 // longeueur clause = 2 au départ
 // etat_clause

 /*
 typedef struct{
	 int val;
	 lit *suiv
 }lit;

 typedef struct{

 }clause;
 */

typedef struct{
  int size;
  char **C;
}clauselit;

typedef struct{
  int size;
  int **C;
}clause;


//lit les clauses dans un tableau 2D de CHAR
 void readficlit(char *file,clauselit *cl){
	 FILE *fp;
	 int ch,i,j=0,size=0;
	 fp=fopen(file,"r");
	 if(fp){
		while((ch=fgetc(fp))!=EOF){
			if(ch=='\n')
				size++;
		}
		printf("size : %d\n",size);
		rewind(fp);
    cl->size=size;
		cl->C=malloc(sizeof(char*)*size+1);
		for(i=0;i<size;i++)
			cl->C[i]=malloc(sizeof(char*)*size);
    i=0;
		while((ch=fgetc(fp))!=EOF){
			while(ch!='\n'){
				if(ch!=' '){
					cl->C[i][j]=ch;
					j++;
				}
        ch=fgetc(fp);
			}
      cl->C[i][j]='\0';
      j=0;
			i++;
		 }
		 fclose(fp);
	 }
 }

//lit les clauses dans un tableau 2D de INT selon où a=1 , -a=2 , b=3 ...
 void readfic(char *file,clause *cl){
   FILE *fp;
	 int ch,i,j=0,size=0;
	 fp=fopen(file,"r");
	 if(fp){
		while((ch=fgetc(fp))!=EOF){
			if(ch=='\n')
				size++;
		}
    printf("size : %d\n",size);
		rewind(fp);
    cl->size=size;
		cl->C=malloc(sizeof(int*)*size+1);
		for(i=0;i<size;i++)
			cl->C[i]=malloc(sizeof(int*)*size);
    i=0;
		while((ch=fgetc(fp))!=EOF){
			while(ch!='\n'){
				if(ch=='-'){
          ch=fgetc(fp);
          cl->C[i][j]=(int)ch-96;
          cl->C[i][j]=cl->C[i][j]*2;
          j++;
        }
        else if(ch!=' '){
					cl->C[i][j]=(int)ch-96;
          cl->C[i][j]=cl->C[i][j]*2-1;
					j++;
				}
        ch=fgetc(fp);
			}
      cl->C[i][j]=0;
      j=0;
			i++;
		 }
		 fclose(fp);
	 }
 }

//affiche les clauses pour la structure avec tableau de CHARR
 void printclauseslit(clauselit cl){
  for(int i=0;i<cl.size;i++){
    for(int j=0;cl.C[i][j]!='\0';j++)
      printf("%c ",cl.C[i][j]);
    printf("\n");
  }
 }

//affiche les clauses pour la structure avec tableau de INT
 void printclauses(clause cl){
  for(int i=0;i<cl.size;i++){
    for(int j=0;cl.C[i][j]!=0;j++)
      printf("%d ",cl.C[i][j]);
    printf("\n");
  }
 }


int dpll(clause cl){
  int i,n;
  if(empty(cl))
    return 1;
  if(inconsistent(cl,l_prises))
    return 0;
  //vérifie mono-littéraux
  for(i=0;i<cl.size && size(cl.C)!=1;i++);
  if(i!=cl.size)
    l=cl.C[i][0];
  else{
    //vérification littéraux purs
    n=max(cl); //maxsize of clause
    for(i=0;i<cl.size;i++)
      for(int j=0;j<n;j++)
  }
}

int main(int argc,char **argv){
  clause *cl;
  cl=malloc(sizeof(clause));
  //clauselit *cl;
  //cl=malloc(sizeof(clauselit));

	readfic(argv[1],cl);
  printclauses(*cl);
}
