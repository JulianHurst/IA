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
 void readfic(char *file,char **C){
	 FILE *fp;
	 int ch,i,j=0,size=0;
	 fp=fopen(file,"r");
	 if(fp){
		while(ch=fgetc(fp)!=EOF){
			printf("%c\n",ch);
			if(ch=='\n')
				size++;
		}
		printf("hi %d\n",size);
		rewind(fp);
		C=malloc(sizeof(char*)*5);
		for(i=0;i<size;i++)
			C[i]=malloc(sizeof(char*)*size);		
		while(ch=fgetc(fp)!=EOF){
			while(ch!='\n'){
				if(ch!=' '){
					C[i][j]=ch;
					j++;
				}
			} 
			i++;
		 }
		 fclose(fp);
	 }
 }
 
 
 int dpll(){}
 
int main(int argc,char **argv){
	char **C;
	
	readfic(argv[1],C);	
}
