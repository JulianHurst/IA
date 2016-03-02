#include <stdio.h>

/* a = 1
 * -a=2
 * b=3
 * -b=4
 * ...
 */
 
 // etat_var combien de valeurs essayées pr une variable (vecteur 1D à 0 au début)
 // longeueur clause = 2 au départ
 // etat_clause
 
 typedef struct{
	 int val;
	 lit *suiv
 }lit;
 
 typedef struct{
	 
 }clause;
 
 void readfic(char *file,){
	 FILE *fp;
	 int ch;
	 fp=fopen(file,"r");
	 if(fp){
		 while(ch=getc(fp)!=EOF){
			 
		 }
 }
 
 int dpll(){
	 
 }
 
int main(int argc,char **argv){
	
}
