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
  int *l;
}lit;

typedef struct{
  int size;
  lit *C;
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
		cl->C=malloc(sizeof(char*)*500);
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
 clause *readfic(char *file){
   clause *cl;
	FILE *fp;
	int ch,i,j=0,size=0,sizel=0;
	fp=fopen(file,"r");
	if(fp){
		while((ch=fgetc(fp))!=EOF){
			if(ch=='\n')
				size++;
		}
    printf("size : %d\n",size);
		rewind(fp);
		cl=malloc(sizeof(clause*));
		cl->size=size;
		cl->C=malloc(sizeof(lit*)*(2*size));
		for(int i=0;i<cl->size;i++)
		  cl->C[i].l=malloc(sizeof(int*)*50);

    i=0;
		while((ch=fgetc(fp))!=EOF){
		while(ch!='\n'){
				if(ch=='-'){
          ch=fgetc(fp);
          cl->C[i].l[j]=(int)ch-96;
          cl->C[i].l[j]=cl->C[i].l[j]*2;
          j++;
        }
        else if(ch!=' '){
					cl->C[i].l[j]=(int)ch-96;
          cl->C[i].l[j]=cl->C[i].l[j]*2-1;
					j++;
				}
        ch=fgetc(fp);
			}
			cl->C[i].size=j;
      cl->C[i].l[j]=0;
      j=0;
			i++;
		 }
		 fclose(fp);
	 }
	 return cl;
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
    for(int j=0;j<cl.C[i].size;j++)
      printf("%d ",cl.C[i].l[j]);
    printf("\n");
  }
 }

//Cherche un littéral parmi une clause ou une liste de littéraux
int findlit(int x,lit taken){
  for(int i=0;i<taken.size;i++)
    if(taken.l[i]==x)
      return i;
  return -1;
}

//Cherche un littéral parmi toutes les clauses
int findcl(int x,clause cl){
  for(int i=0;i<cl.size;i++)
    for(int j=0;j<cl.C[i].size;j++)
      if(cl.C[i].l[j]==x)
        return 1;
  return 0;

}

int inconsistent(clause cl, lit taken){
  //int n=max(cl);
  for(int i=0;i<cl.size;i++)
    if(cl.C[i].size==1)
      if(!(cl.C[i].l[0]%2)){
        if(findlit(cl.C[i].l[0]-1,taken)!=-1)
			return 1;
		for(int j=i+1;j<cl.size;j++)
			if(cl.C[j].size==1 && cl.C[i].l[0]-1==cl.C[j].l[0])
				return 1;
      }
      else{
        if(findlit(cl.C[i].l[0]+1,taken)!=-1)
          return 1;
          for(int j=i+1;j<cl.size;j++)
			if(cl.C[j].size==1 && cl.C[i].l[0]+1==cl.C[j].l[0])
				return 1;
	}
  return 0;
}


//Cherche un mono-littéral
int monolit(clause cl){
  for(int i=0;i<cl.size;i++){
    if(cl.C[i].size==1)
      return cl.C[i].l[0];
  }
  return 0;
}

//Cherche un littéral pur
int veriflitpurs(clause cl){
  int x;
  for(int i=0;i<cl.size;i++)
    for(int j=0;j<cl.C[i].size;j++){
      x=cl.C[i].l[j];
      if(x%2==0){
        if(!findcl(x-1,cl))
          return x;
      }
      else if(!findcl(x+1,cl))
        return x;
    }
}

clause *choicelit(int l,clause *cl){
  int x=0,t=0,reset=0;
  for(int i=0;i<cl->size;i++){
    if(findlit(l,cl->C[i])!=-1){
      for(t=i;t<cl->size-1;t++)
        cl->C[t]=cl->C[t+1];
      //cl.C[t]=-1;
      cl->size--;
      reset=1;
    }
    //adjust list like above and decrement size
    if(l%2==0)
      x=findlit(l-1,cl->C[i]);
    else
		x=findlit(l+1,cl->C[i]);
	if(x!=-1){
		for(t=x;t<cl->C[i].size-1;t++)
			cl->C[i].l[t]=cl->C[i].l[t+1];
		cl->C[i].l[t]=0;
		cl->C[i].size--;
	}
	if(reset){
		i--;
		reset=0;
	}
  }
  return cl;
}

/*
1 a
2 -a

(p v q) n -p n (-p v q v -r)

(p v q v -r) n (p v -q) n -p n r n u
*/
int dpll(clause *cl){
  int i=0,n,l;
  lit taken;
  taken.size=0;
  taken.l=malloc(sizeof(int*)*100);
  //Si vide renvoie vrai
  while(cl->size!=0){
	  // si pb renvoie faux
	  if(inconsistent(*cl,taken))
		return 0;
	  //vérifie mono-littéraux
	  if(l=monolit(*cl));
	  else if(l=veriflitpurs(*cl));
	  else
		l=cl->C[0].l[0];
	  taken.l[i]=l;
	  taken.size++;
	  printf("\nl : %d\n",l);
	  cl=choicelit(l,cl);
	  printclauses(*cl);
		//vérification littéraux
		i++;
  }
  return 1;
}



int main(int argc,char **argv){
  clause *cl;
  lit li;
  li.l=malloc(sizeof(int*)*100);
  //cl=malloc(sizeof(clause));
  //clauselit *cl;
  //cl=malloc(sizeof(clauselit));

  cl=readfic(argv[1]);
  printclauses(*cl);
  printf("\n");
  /*
  cl=choicelit(34,cl);
  //cl=choicelit(31,cl);
  printf("\n");
  printclauses(*cl);
  li.l[0]=35;
  li.size=1;
  printf("%d\n",inconsistent(*cl,li));
  /*printf("%d\n",monolit(*cl));
  printf("%d\n",veriflitpurs(*cl));*/
  printf("%d\n",dpll(cl));
}
