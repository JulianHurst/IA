#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int FILE_TYPE=-1;
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
  int size;
  int *l;
}lit;

typedef struct{
  long size;
  lit *C;
}clause;

//Renvoie le type de fichier (fichier avec clauses en char ou en int)
int checkfic(char *file){
	int ch;
	FILE *fp;
	fp=fopen(file,"r");
	if(fp){
		ch=fgetc(fp);
		printf("%d\n",ch);
		if((ch>96 && ch<123) || ch==45){			
			FILE_TYPE=0;
		}
		else{
			FILE_TYPE=1;
		}
		fclose(fp);				
	}
	else
		fprintf(stderr,"Le fichier \"%s\" à lire n'a pas pu être ouvert en lecture\n",file);		
	return FILE_TYPE;
}

//lit les clauses dans un struct clause où a=1 , -a=2 , b=3 ...
 clause *readficlit(char *file){
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
 
 //lit les clauses dans un struct clause (uniquement des int)
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
		cl=malloc(sizeof(clause*)*(size));
		cl->size=size;
		cl->C=malloc(sizeof(lit*)*(2*(size+1)));
		for(int i=0;i<cl->size;i++){
		  cl->C[i].l=malloc(sizeof(int*)*50);
		  memset(cl->C[i].l,0,sizeof(int*)*50);
		}
		i=0;
		while(!feof(fp)){
			printf("%d %d\n",i,j);
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
 
 //Détermine le type de fichier et éffectue la lecture de fichier correspondante
  clause * readfic(char *file){
	 if(checkfic(file))
		return readficnum(file);
	 return readficlit(file);
 }
 
 //exporte l'ensemble de clauses cl dans un fichier file
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
//Retourne l'indice dans le tableau de l'emplacement de la valeur ou -1 si la valeur n'est pas trouvée
int findlit(int x,lit taken){
  for(int i=0;i<taken.size;i++)
    if(taken.l[i]==x)
      return i;
  return -1;
}

//Cherche un littéral parmi toutes les clauses et renvoie vrai s'il le trouve et faux sinon
int findcl(int x,clause cl){
  for(int i=0;i<cl.size;i++)
    for(int j=0;j<cl.C[i].size;j++)
      if(cl.C[i].l[j]==x)
        return 1;
  return 0;

}

//Trouve une valueur parmi un tableau de int
//Retourne l'indice dans le tableau de l'emplacement de la valeur ou -1 si la valeur n'est pas trouvée
int find(int x,int *T){
	for(int i=0;T[i]!=0;i++)
		if(T[i]==x)
			return i;
	return -1;
}

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


//Cherche le premier mono-littéral dans cl
int monolit(clause cl){
  for(int i=0;i<cl.size;i++){
    if(cl.C[i].size==1)
      return cl.C[i].l[0];
  }
  return 0;
}

//Cherche le premier littéral pur dans cl
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
      cl->size--;
      reset=1;
    }    
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
}

//vérifie si tous les littéraux de l'ensemble de clauses cl sont contenus dans taken_all
int all(clause cl,lit taken_all){	
  for(int i=0;i<6;i++)	
  for(int i=0;i<cl.size;i++)
    for(int j=0;j<cl.C[i].size;j++){		
      if(findlit(cl.C[i].l[j],taken_all)==-1)
        return 0;
	}
  return 1;
}

//Choisit le premier littéral positif dans cl
int chooseunsignedlit(clause cl){
  for(int i=0;i<cl.size;i++)
    for(int j=0;j<cl.C[i].size;j++)
      if((cl.C[i].l[j]%2)!=0)
        return cl.C[i].l[j];
  return 0;
}

//Cherche le dernier littéral pur de takenrand puis renvoie son inverse
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

//Copie l'ensemble de clauses cl et renvoie la copie
clause * copy(clause *cl){
	clause *tmp;		
	tmp=malloc(sizeof(clause));
	tmp->C=malloc(sizeof(lit*)*(cl->size*2));
	tmp->size=cl->size;
	for(int i=0;i<cl->size;i++){
		  tmp->C[i].l=malloc(sizeof(int*)*(cl->C[i].size+1));
		  memset(tmp->C[i].l,0,sizeof(int*)*(cl->C[i].size+1));
	 }
	for(int i=0;i<cl->size;i++)
		for(int j=0;j<cl->C[i].size;j++){
			tmp->C[i].size=cl->C[i].size;
			tmp->C[i].l[j]=cl->C[i].l[j];
		}
	return tmp;
}

//copie les littéraux de t dans la clause cl->C[s]
void copylit(clause *cl,lit t,int s){
	cl->C[s].size=t.size;
	for(int i=0;i<cl->C[s].size;i++)
		cl->C[s].l[i]=t.l[i];
}

//Applique le choix des b littéraux de taken sur un ensemble de clauses
clause * rollback(clause *cl, lit taken, int b){
	clause *tmp;
	tmp=copy(cl);
	for(int i=0;i<b;i++)
	  choicelit(taken.l[i],tmp);	
	return tmp;
}

//Heuristique 1 : Choisit le littéral qui apparait le plus dans la base de clauses
int firstsatisfy(clause cl){
	printf("firstsat\n");
	int count=0,k=0,max=0,max_ind=0,ret;
	int *x;
	x=malloc(sizeof(int*)*cl.size);
	memset(x,0,sizeof(int*)*cl.size);
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
	
	ret=x[max_ind];
	free(x);
	return ret;
}

//Heuristique 2 : L'oppposé QUI APPARAIT LE PLUS
int firstfail(clause cl){			
	printf("firstfail\n");
	int count=0,k=0,max=0,max_ind=0;
	int *x;
	x=malloc(sizeof(int*)*cl.size);
	memset(x,0,sizeof(int*)*cl.size);
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

//Heuristique 3
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
	x=malloc(sizeof(int*)*cl.size);
	memset(x,0,sizeof(int*)*cl.size);
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
int countlit(int l,clause cl){
	for(int i=0;i<cl.size;i++)
		for(int j=0;j<cl.C[i].size;j++)		
}
*/

void printlit(lit t){
	for(int i=0;i<t.size;i++)
		printf("%d ",t.l[i]);
}

//Applique dpll sur un ensemble de clauses
int dpll(clause *cl,int h,clause *solutions){
  int i=0,l,random=0,r=0,sat=0,k=0,s=0,inc=0;
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
  while(1){
      l=0;
      inc=inconsistent(*cl);
      if(!inc && cl->size==0)
		sat=1;	  
	  if(inc || cl->size==0){
      if(random){
        if((l=back(takenrand))==0){            
          printf("Nombre de noeuds explorés : %d\n",taken_all.size);
          return sat;		
	  }
        else{	  
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
          //printf("rollback k %d\n",k); 
          if(inc){
			printf("Inconsistent!\n");
			if(FILE_TYPE==0)
				printclausesch(*cl);
			else
				printclauses(*cl);
			printf("\n");
		  }
		  printf("Backtrack...\n");   
          cl=rollback(tmp,taken,k);           
		}
      }
      else{
		    printf("Nombre de noeuds explorés : %d\n",taken_all.size);
		    return sat;
		}
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
    if(FILE_TYPE==0){
		if(l%2==0)
			printf("l : -%c\n",l/2+96);
		else
			printf("l : %c\n",(l+1)/2+96);
		printclausesch(*cl);
	}
	else{
		printf("l : %d\n",l);	  
		printclauses(*cl);
	}
	  choicelit(l,cl);	  
	  if(cl->size==0){
		  printf("\nensemble vide\n\n");
		  solutions->size++;
		  copylit(solutions,taken,s);
		  s++;
	  }
	  else
		  printf("\n");		
		i++;	
  }  
  return 1;
}

//Renvoie factorielle n
long fact(int n){
	long x=1;
	for(int i=1;i<=n;i++)
		x*=i;
	return x;
}

//Renvoie le résultat de k parmi n
long comb(int n,int k){
	return (fact(n) / (fact(k)*fact(n-k)));
}

//Génère un fichier file contenant les clauses correspondantes au problème des n-pigeons
void pigeongen(char *file,int n){
	int l=1,k,x,q;
	clause *cl;
	cl=malloc(sizeof(clause*));	
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
	exportfic(file,cl);	
}

//renvoie le min de a et de b
int min(int a,int b){
	return a<b ? a : b;
}

//Génère un fichier file contenant les clauses correspondantes au problème des n-dames
void damesgen(char *file,int n){
	int x,q,max,k,l=1,p;
	clause *cl;
	cl=malloc(sizeof(clause*));	
	//cl->size=n+n+n*comb(n,2);
	cl->size=10000;
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

int main(int argc,char **argv){
  int ret=0,h;
  clause *cl;
  clause *solutions;
  
  if(argc-1<2 || (argc-1)%2!=0){
	fprintf(stderr,"USAGE : %s [-x pigeon|dames n fichiersortie] [fichier.sat heuristique]\n",argv[0]);
	return -1;
  }
  
  if(strcmp(argv[1],"-x")==0 && argc-1>=4){
	  if(strcmp(argv[2],"pigeon")==0)
		pigeongen(argv[4],atoi(argv[3]));
	  else if(strcmp(argv[2],"dames")==0)
		damesgen(argv[4],atoi(argv[3]));
	  else{
		fprintf(stderr,"Le générateur \"%s\" n'existe pas!\n",argv[2]); 
		return -1;
	  }
  }
  else{
    if((cl=readfic(argv[1]))==NULL)
		return -1; 
	h=atoi(argv[2]);
  }
 
  if(argc-1>4){
	if((cl=readfic(argv[5]))==NULL)
		return -1; 
    h=atoi(argv[6]);
  }
  else if(argc-1==4)
	return ret;
	
  solutions=malloc(sizeof(clause*)*(cl->size+1));
  solutions->size=0;
  solutions->C=malloc(sizeof(lit*)*(2*(cl->size+1)));
  for(int i=0;i<cl->size;i++){
	solutions->C[i].l=malloc(sizeof(int*)*50);
	memset(solutions->C[i].l,0,sizeof(int*)*50);
  }
	
  if((ret=dpll(cl,h,solutions)))
	printf("Satisfiable\n");
  else
	printf("Non satisfiable\n");
	
  if(solutions->size){
	printf("\nSolutions : \n");
	if(FILE_TYPE==0)
		printclausesch(*solutions);
	else
		printclauses(*solutions);
  }
  return ret;
}
