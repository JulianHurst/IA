#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
  long size;
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
	int ch,i,j=0,size=0;	
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
		for(int i=0;i<cl->size;i++){
		  cl->C[i].l=malloc(sizeof(int*)*50);
		  memset(cl->C[i].l,0,sizeof(int*)*50);
		}
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
		//cl->C[i].l[j]=0;
		j=0;
		i++;
	 }
		 fclose(fp);
	 }
	 else{
		fprintf(stderr,"Le fichier \"%s\" à lire n'a pas pu être ouvert en lecture\n",file);
		return NULL;
	}
	 return cl;
 }
 
 //lit les clauses dans un tableau 2D de INT selon où a=1 , -a=2 , b=3 ...
 clause *readficnum(char *file){
   clause *cl;
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
		cl=malloc(sizeof(clause*));
		cl->size=size;
		cl->C=malloc(sizeof(lit*)*(2*size));
		for(int i=0;i<cl->size;i++){
		  cl->C[i].l=malloc(sizeof(int*)*50);
		  memset(cl->C[i].l,0,sizeof(int*)*50);
		}
		i=0;
		while(!feof(fp)){
			fscanf(fp,"%d",&cl->C[i].l[j]);
			ch=fgetc(fp);			
			j++;
			if(ch=='\n'){						
				cl->C[i].size=j;
				j=0;
				i++;
			}						
		}	
		fclose(fp);
	}		 	
	 else{
		fprintf(stderr,"Le fichier \"%s\" à lire n'a pas pu être ouvert en lecture\n",file);
		return NULL;
	}
	 return cl;
 }
 
 void exportfic(char *file,clause *cl){
	 FILE *fp;
	 fp=fopen(file,"w");
	 if(fp){
		 for(int i=0;i<cl->size;i++){
			for(int j=0;j<cl->C[i].size;j++){
				if(j+1<cl->C[i].size)
					fprintf(fp,"%d ",cl->C[i].l[j]);
				else
					fprintf(fp,"%d",cl->C[i].l[j]);
			}
			fprintf(fp,"\n");
		}
		fclose(fp);
	 }
	 else
		fprintf(stderr,"Le fichier \"%s\" à exporter n'a pas pu être ouvert en écriture\n",file);
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
	 printf("size : %ld\n",cl.size);
  for(int i=0;i<cl.size;i++){
    for(int j=0;j<cl.C[i].size;j++)
      printf("%d ",cl.C[i].l[j]);
    printf("\n");
  }
 }

//Affiche les clauses pour la structure avec tableau de INT en CHAR
void printclausesch(clause cl){
  for(int i=0;i<cl.size;i++){
    for(int j=0;j<cl.C[i].size;j++){
		if(cl.C[i].l[j]%2==0)
			printf("-%c ",cl.C[i].l[j]/2+96);
		else
			printf("%c ",(cl.C[i].l[j]+1)/2+96);
		}
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
/*
//Renvoie 1 s'il y a une inconsistence dans l'ensemble de clauses (pas satisfiable)
int inconsistent(clause cl, lit taken){
  //int n=max(cl);
  for(int i=0;i<cl.size;i++)
    if(cl.C[i].size==1){
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
	}
  return 0;
}
*/
//Renvoie 1 s'il y a une inconsistence dans l'ensemble de clauses (pas satisfiable)
int inconsistent(clause cl){  
  for(int i=0;i<cl.size;i++)
    if(cl.C[i].size==1){
      if(!(cl.C[i].l[0]%2)){
    		for(int j=i+1;j<cl.size;j++)
    			if(cl.C[j].size==1 && cl.C[i].l[0]-1==cl.C[j].l[0])
    				return 1;
      }
      else{
          for(int j=i+1;j<cl.size;j++)
      			if(cl.C[j].size==1 && cl.C[i].l[0]+1==cl.C[j].l[0])
      				return 1;
	  }
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
    return 0;
}

//Confirme le choix d'un littéral et effectue les modifications nécessaires
void choicelit(int l,clause *cl){
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
  //return cl;
}

/*
1 a
2 -a

(p v q) n -p n (-p v q v -r)

(p v q v -r) n (p v -q) n -p n r n u
*/

//Applique dpll sur un ensemble de clauses
int dpll(clause *cl){
  int i=0,l;
  lit taken;
  taken.size=0;
  taken.l=malloc(sizeof(int*)*100);
  //Si vide renvoie vrai
  while(cl->size!=0){
	  // si pb renvoie faux
	  if(inconsistent(*cl))
		  return 0;
	  //vérifie mono-littéraux
	  if((l=monolit(*cl)));
	  else if((l=veriflitpurs(*cl)));
	  else
		  l=cl->C[0].l[0];
	  taken.l[i]=l;
	  taken.size++;
	  if(l%2==0)
		printf("l : -%c\n",l/2+96);
      else
		printf("l : %c\n",(l+1)/2+96);
	  choicelit(l,cl);
	  printclausesch(*cl);
	  if(cl->size==0)
		printf("vide\n\n");
	  else
		printf("\n");
		//vérification littéraux
		i++;
  }
  return 1;
}

int all(clause cl,lit taken_all){
	//printclauses(cl);
	//printf("size : %d\n",cl.size);
  for(int i=0;i<6;i++)
	//printf("%d\n",taken_all.l[i]);
  for(int i=0;i<cl.size;i++)
    for(int j=0;j<cl.C[i].size;j++){
		//printf("t %d\n",cl.C[i].l[j]);
      if(findlit(cl.C[i].l[j],taken_all)==-1)
        return 0;
	}
  return 1;
}

int chooseunsignedlit(clause cl){
  for(int i=0;i<cl.size;i++)
    for(int j=0;j<cl.C[i].size;j++)
      if((cl.C[i].l[j]%2)!=0)
        return cl.C[i].l[j];
  return 0;
}

int back(lit takenrand){
  for(int i=takenrand.size-1;i>=0;i--){
    if(takenrand.l[i]%2==0){
      if(findlit(takenrand.l[i]-1,takenrand)==-1)
        return takenrand.l[i]-1;
      }
      else{
        if(findlit(takenrand.l[i]+1,takenrand)==-1)
          return takenrand.l[i]+1;
      }
  }
  return 0;
}

clause * copy(clause *cl){
	clause *tmp;		
	tmp=malloc(sizeof(clause));
	tmp->C=malloc(sizeof(lit*)*(cl->size*2));
	tmp->size=cl->size;
	for(int i=0;i<cl->size;i++){
		  tmp->C[i].l=malloc(sizeof(int*)*cl->C[i].size);
		  memset(tmp->C[i].l,0,sizeof(int*)*cl->C[i].size);
	 }
	for(int i=0;i<cl->size;i++)
		for(int j=0;j<cl->C[i].size;j++){
			tmp->C[i].size=cl->C[i].size;
			tmp->C[i].l[j]=cl->C[i].l[j];
		}
	return tmp;
}

void copylit(clause *cl,lit t,int s){
	cl->C[s].size=t.size;
	for(int i=0;i<cl->C[s].size;i++)
		cl->C[s].l[i]=t.l[i];
}


clause * rollback(clause *cl, lit taken, int b){
	clause *tmp;
	tmp=copy(cl);
	for(int i=0;i<b;i++)
	  choicelit(taken.l[i],tmp);	
	return tmp;
}

int find(int x,int *T){
	for(int i=0;T[i]!=0;i++)
		if(T[i]==x)
			return i;
	return -1;
}

//Choisit le littéral qui apparait le plus dans la base de clauses
int firstsatisfy(clause cl){
	printf("firstsat\n");
	int count=0,k=0,max=0,max_ind=0;
	int *x;
	x=malloc(sizeof(int*)*20);
	memset(x,0,sizeof(int*)*20);
	x[k]=0;
	for(int i=0;i<cl.size;i++){
		for(int j=0;j<cl.C[i].size;j++){
			if(find(cl.C[i].l[j],x)==-1){
				x[k]=cl.C[i].l[j];
				x[k+1]=0;				
				for(int p=i;p<cl.size;p++)
					for(int q=0;q<cl.C[p].size;q++)
						if(cl.C[p].l[q]==x[k])
							count++;			
				if(count>max){
					max=count;
					max_ind=k;
				}
				k++;
			}
			count=0;
		}
	}
	
	return x[max_ind];
}

//L'oppposé QUI APPARAIT LE PLUS
int firstfail(clause cl){			
	printf("firstfail\n");
	int count=0,k=0,max=0,max_ind=0;
	int *x;
	x=malloc(sizeof(int*)*20);
	memset(x,0,sizeof(int*)*20);
	x[k]=0;
	for(int i=0;i<cl.size;i++){
		for(int j=0;j<cl.C[i].size;j++){
			if(find(cl.C[i].l[j],x)==-1){
				x[k]=cl.C[i].l[j];
				x[k+1]=0;				
				for(int p=i;p<cl.size;p++)
					for(int q=0;q<cl.C[p].size;q++)
						if(x[k]%2==0){
							if(cl.C[p].l[q]==x[k]-1)								
								count++;			
						}
						else
							if(cl.C[p].l[q]==x[k]+1)
								count++;											
				if(count>max){
					max=count;
					max_ind=k;
				}
				k++;
			}
			count=0;
		}
	}
	
	return x[max_ind];
}

int firstfailbis(clause cl){			
	printf("firstfailbis\n");	
	int count=0,k=0,max=0,max_ind=0,size=0,neq=0;
	for(int i=0;i<cl.size;i++){
		if(cl.C[i].size>size)
			size=cl.C[i].size;
	}
	for(int i=0;i<cl.size;i++)
		if(cl.C[i].size<size)
			neq=1;
	int *x;
	x=malloc(sizeof(int*)*20);
	memset(x,0,sizeof(int*)*20);
	x[k]=0;
	for(int i=0;i<cl.size;i++){
		for(int j=0;j<cl.C[i].size;j++){
			if(find(cl.C[i].l[j],x)==-1){
				x[k]=cl.C[i].l[j];
				x[k+1]=0;				
				for(int p=i;p<cl.size;p++)
					for(int q=0;q<cl.C[p].size;q++)
						if(x[k]%2==0){
							if(cl.C[p].l[q]==x[k]-1){
								if(neq){
									if(cl.C[p].l[q]<size)
										count++;
								}
								else
									count++;
							}
						}
						else{
							if(cl.C[p].l[q]==x[k]+1){
								if(neq){
									if(cl.C[p].l[q]<size)
										count++;
								}
								else
									count++;
							}
						}
				if(count>max){
					max=count;
					max_ind=k;
				}
				k++;
			}
			count=0;
		}
	}
	
	return x[max_ind];
}

/*
int firstfailbis(clause cl){
	int l,big=0;
	for(int i=0;i<cl.size;i++)
		if(cl.size>big)
			big=cl.size;
	for(int i=0;i<cl.size;i++){
		for(int j=0;j<cl.C[i].size;j++){
			if((cl.C[i].l[j]%2)!=0){
				if(findcl(cl.C[i].l[j]+1,cl))
					return cl.C[i].l[j];
			}
			else
				if(findcl(cl.C[i].l[j]-1,cl))
					return cl.C[i].l[j];			
		}
	}
}

int countlit(int l,clause cl){
	for(int i=0;i<cl.size;i++)
		for(int j=0;j<cl.C[i].size;j++)		
}
*/
/*taken_all contains all taken litterals since start of dpll
  taken contains taken litterals for a given path in dpll
  check taken for pure litteral
  delete all taken litterals after pure litteral from taken
  take opposite polarity of litteral or next litteral
  repeat till taken_all contains all litterals
*/

//takenrand = litts taken randomly

//Applique dpll sur un ensemble de clauses
/*
int dpll_all(clause *cl,int h){
  int i=0,l,random=0,b,r=0;
  lit taken,takenrand;
  clause *tmp;  
  tmp=copy(cl);
  taken.size=0;
  taken.l=malloc(sizeof(int*)*100);
    takenrand.size=0;
  takenrand.l=malloc(sizeof(int*)*100);
  //Si vide renvoie vrai
  //REPLACE WITH something ! Maybe while pure lit exists in takenrand maybe same as above 
  while(cl->size!=0){
      l=0;
	  // si pb renvoie faux
	  if(inconsistent(*cl)){
      if(random){
        if((l=back(takenrand))==0)  //go back to last pure litteral (from back to front) taken randomly
          return 0;
        else{
          if(l%2==0)
            b=findlit(l-1,taken);
          else
            b=findlit(l+1,taken);          
          taken.l[k]=l;
          taken.size=k+1;
          for(int p=0;taken.size;p++)
			taken.l[p]=0;
          takenrand.l[r]=l;
          takenrand.size++;
          r++;
          cl=rollback(tmp,taken,b);  //rolls back changes on cl to moment b (using all taken litterals up to b, even unitary or pure)
          //maybe resize takenrand
      }
      }
      else
		    return 0;
    }
      if(l==0){
	  //vérifie mono-littéraux ou lit purs
	  if((l=monolit(*cl)));
	  else if((l=veriflitpurs(*cl)));
	  else{
      	random=1;
      	if(h==1)
			l=firstsatisfy(*cl);
		else
			l=chooseunsignedlit(*cl);
		printf("takenrand : %d\n",l);
      	takenrand.l[r]=l;
      	takenrand.size++;
      	r++;
	  }
    }
	  taken.l[i]=l;
    taken.size++;
	  if(l%2==0)
		  printf("l : -%c\n",l/2+96);
    else
		  printf("l : %c\n",(l+1)/2+96);
	  choicelit(l,cl);
	  printclausesch(*cl);
	  if(cl->size==0)
		  printf("ensemble vide\n\n");
	  else
		  printf("\n");
		//vérification littéraux
		i++;
  }
  return 1;
}
* 
* 
* a e -e
* a -a
* a -a e
* a -a e -e  
* a -a e -e 
* */



void printlit(lit t){
	for(int i=0;i<t.size;i++)
		printf("%d ",t.l[i]);
}

int dpll_all_sol(clause *cl,int h,clause *solutions){
  int i=0,l,random=0,r=0,sat=0,k=0,s=0;
  lit taken,takenrand,taken_all;
  clause *tmp;
    
  tmp=copy(cl);
  taken.size=0;
  taken.l=malloc(sizeof(int*)*100);  
  takenrand.size=0;
  takenrand.l=malloc(sizeof(int*)*100);  
  taken_all.size=0;
  taken_all.l=malloc(sizeof(int*)*100);
  memset(taken_all.l,0,sizeof(int*)*100); 
  memset(taken.l,0,sizeof(int*)*100);
  memset(takenrand.l,0,sizeof(int*)*100);  
  //Si vide renvoie vrai
  //REPLACE WITH something ! Maybe while pure lit exists in takenrand maybe same as above 
  //while(!all(*tmp,taken_all)){
  while(1){
      l=0;
      if(!inconsistent(*cl) && cl->size==0)
		sat=1;
	  // si pb renvoie faux
	  if(inconsistent(*cl) || cl->size==0){
      if(random){
        if((l=back(takenrand))==0)  //go back to last pure litteral (from back to front) taken randomly          
          return sat;		
        else{
	  //l=back(takenrand);	
          if(l%2==0){
            k=findlit(l-1,taken);
            r=findlit(l-1,takenrand);
		  }
          else{
            k=findlit(l+1,taken);
            r=findlit(l+1,takenrand);
		  }
          taken.l[k]=l;
          for(int p=k+1;i<taken.size;p++)
			taken.l[p]=0;        
		  taken.size=k+1;
		  k++;
          takenrand.l[r+1]=l;
          for(int p=r+2;i<takenrand.size;p++)
			takenrand.l[p]=0; 
          takenrand.size=r+2;
          r+=2; 
          printf("rollback k %d\n",k);         
          cl=rollback(tmp,taken,k);  //rolls back changes on cl to moment b (using all taken litterals up to b, even unitary or pure)          
          //maybe resize takenrand
		}
      }
      else
		    return sat;
      }
      if(l==0){
	  //vérifie mono-littéraux ou lit purs
	  if((l=monolit(*cl)));
	  else if((l=veriflitpurs(*cl)));
	  else{
      	random=1;
      	if(h==1)
			l=firstsatisfy(*cl);
		else if(h==2)
			l=firstfail(*cl);
		else if(h==3)
			l=firstfailbis(*cl);
		else
			l=chooseunsignedlit(*cl);		
      	takenrand.l[r]=l;
      	takenrand.size++;
      	r++;
	  }
	taken.l[k]=l;
    taken.size++;
    k++;
	}
    taken_all.l[i]=l;
    taken_all.size++;
	/* if(l%2==0)
		  printf("l : -%c\n",l/2+96);
    else
		  printf("l : %c\n",(l+1)/2+96);*/
	printf("l : %d\n",l);
	  //printclausesch(*cl);
	  //printclauses(*cl);
	  choicelit(l,cl);
	  printclauses(*cl);
	  if(cl->size==0){
		  printf("ensemble vide\n\n");
		  solutions->size++;
		  copylit(solutions,taken,s);
		  s++;
	  }
	  else
		  printf("\n");
		//vérification littéraux
		i++;
	//printf("hi\n");
  }  
  return 1;
}

long fact(int n){
	long x=1;
	for(int i=1;i<=n;i++)
		x*=i;
	return x;
}

long comb(int n,int k){
	return (fact(n) / (fact(k)*fact(n-k)));
}

void pigeongen(char *file,int n){
	int l=1,k,x,q;
	clause *cl;
	cl=malloc(sizeof(clause*));
	//FORMULE DU PROF
	//cl->size=n+n*comb(n-1,2)+(n-1)*comb(n,2);
	cl->size=n+n+(n-1)*comb(n,2);
	printf("size : %ld\n",cl->size);
	cl->C=malloc(sizeof(lit*)*(2*cl->size));
	for(int i=0;i<cl->size;i++){
	  cl->C[i].l=malloc(sizeof(int*)*50);
	  memset(cl->C[i].l,0,sizeof(int*)*50);
	}	
	for(int i=0;i<n;i++){
		k=i+n;
		cl->C[i].size=(n-1);
		cl->C[k].size=(n-1);
		for(int j=0;j<n-1;j++){			
			cl->C[i].l[j]=l;
			cl->C[k].l[j]=l+1;
			l+=2;
		}
	}	
	
	x=l-(2*(n-1))+1;
	printf("x : %d size %ld comb %ld f14 %ld\n",x,cl->size,comb(n,2),fact(n));	
	for(int i=0;i<(n-1);i++){
		l=2*(i+1);
		q=l+(2*(n-1));	
		for(int j=(k+1);j<((k+1)+comb(n,2));j++){
			//printf("%d %d %d %d %d\n",l,i,k, j,comb(n,2));
			cl->C[j].size=2;
			cl->C[j].l[0]=l;
			cl->C[j].l[1]=q;
			if(q==x){
				l+=(2*(n-1));
				q=l+(2*(n-1));
			}
			else
				q+=(2*(n-1));	
		}		
		k+=comb(n,2);		
		x+=2;
	}	
	//printclauses(*cl);
	exportfic(file,cl);
	//return cl;
}

int min(int a,int b){
	return a<b ? a : b;
}

void damesgen(char *file,int n){
	int x,q,max,k,l=1,p;
	clause *cl;
	cl=malloc(sizeof(clause*));	
	//cl->size=n+n+n*comb(n,2);*
	cl->size=500;
	printf("size : %ld\n",cl->size);
	cl->C=malloc(sizeof(lit*)*(2*cl->size));
	for(int i=0;i<cl->size;i++){
	  cl->C[i].l=malloc(sizeof(int*)*50);
	  memset(cl->C[i].l,0,sizeof(int*)*50);
	}
	for(int i=0;i<n;i++){
		k=i+n;
		cl->C[i].size=n;
		cl->C[k].size=n;
		for(int j=0;j<n;j++){			
			cl->C[i].l[j]=l;
			cl->C[k].l[j]=l+1;
			l+=2;
		}
	}	
	max=l-1;
	x=max-2*(n-1);	
	for(int i=0;i<n;i++){
		l=2*(i+1);
		q=l+2*n;	
		for(int j=(k+1);j<(k+1)+comb(n,2);j++){
			cl->C[j].size=2;
			cl->C[j].l[0]=l;
			cl->C[j].l[1]=q;
			if(q==x){
				l+=2*n;
				q=l+2*n;
			}
			else
				q+=2*n;
		}
		k+=comb(n,2);
		x+=2;
	}
	l=2;
	printf("%d\n",k);
	for(int i=0;i<(n-1);i++){
		for(int j=0;j<n;j++){
			if(j!=0){
				q=l;
				for(p=(k+1);p<(k+1)+min(j,n-i-1);p++){
					q+=2*(n-1);
					cl->C[p].size=2;
					cl->C[p].l[0]=l;
					cl->C[p].l[1]=q;										
					//k++;
				}
				k+=p-(k+1);	
			}				
			if(j!=(n-1)){
				q=l;						
				for(p=(k+1);p<(k+1)+min(n-j-1,n-i-1);p++){
					q+=2*(n+1);
					cl->C[p].size=2;					
					cl->C[p].l[0]=l;
					cl->C[p].l[1]=q;										
					//k++;
				}
				k+=p-(k+1);
			}			
			l+=2;
		}
	}
	cl->size=k+1;
	printclauses(*cl);
	exportfic(file,cl);
}
/*
1 3
5 7
2 4
6 8
2 6
4 8

0
2 10

1
2 18

2 4 6 
8 10 12
14 16 18
*/

/*
-d11 v -d22
-d11 v -d33
-d22 v -d33

-d13 v -d22
-d13 v -d33
-d22 v -d31

-d12 v -d21 
-d12 v-d23 



-d

d11 v d12 v d13 v -d14
d21 v d22 v d23
d31 v d32 v d33

2	4	6
8	10	12
-d11 v -d12 v -d13
-d21 v -d22 v -d23
-d31 v -d32 v -d33

//same Line
-d11 v -d21	
-d11 v -d31 
-d11 v -d41
-d21 v -d31
-d12 v -d22
-d12 v -d32
-d22 v -d32
-d13 v -d23
-d13 v -d33
-d23 v -d33

//DIAG
-d11 v -d22
-d11 v -d33

-d21 v -d12
-d22 v -d13

-d21 v -d31
-d22 v -d32
-d23 v -d33
-d21 v -d32

	d1  d2  d3
1   x   
2
3
* */
int main(int argc,char **argv){
  int ret=0;
  clause *cl;
  clause *solutions;
  
  //clause *tmp;
  /*
  lit li;
  li.l=malloc(sizeof(int*)*100);
  cl=malloc(sizeof(clause));
  clauselit *cl;
  cl=malloc(sizeof(clauselit));
  */
  if(argc-1!=2){
	fprintf(stderr,"USAGE : %s fichier.sat heuristique\n",argv[0]);
	return -1;
  }

  if((cl=readficnum(argv[1]))==NULL)
	return -1; 
  
  solutions=malloc(sizeof(clause*));
  solutions->size=0;
  solutions->C=malloc(sizeof(lit*)*(2*cl->size));
  for(int i=0;i<cl->size;i++){
	solutions->C[i].l=malloc(sizeof(int*)*50);
	memset(solutions->C[i].l,0,sizeof(int*)*50);
  }
  
  printclauses(*cl);
  printf("\n");	
	
  if((ret=dpll_all_sol(cl,atoi(argv[2]),solutions)))
	printf("Satisfiable\n");
  else
	printf("Non satisfiable\n");
  
  printclauses(*solutions);
	
	//damesgen("dames3",3);
	//damesgen("dames2",2);
  /*pigeongen("satpig14",14);
  pigeongen("satpig13",13);
  pigeongen("satpig12",12);
  pigeongen("satpig11",11);*/
  //pigeongen("satpig6",6);
  //pigeongen("satpig10",10);
  //pigeongen("satpig7",7);
  //pigeongen("satpig8",8);
  //pigeongen("satpig9",9);
  return ret;
  
  /* TESTS Fonctionnels
  cl=choicelit(34,cl);
  //cl=choicelit(31,cl);
  printf("\n");
  printclauses(*cl);
  li.l[0]=35;
  li.size=1;
  printf("%d\n",inconsistent(*cl,li));
  printf("%d\n",monolit(*cl));
  printf("%d\n",veriflitpurs(*cl));*/

}
