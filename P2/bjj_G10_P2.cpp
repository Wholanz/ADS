#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define M 3 /*The oreder of the B tree*/
#define True 1
#define False 0

#ifndef _BTREE_H  /*Declartion of the B tree*/

struct bTreenode;
typedef struct bTreenode *btree;
typedef struct bTreenode *Node;

btree insert(int X,btree T);
int search(int X,btree T);
void  printout_levelorder(btree T);
btree initialize(btree T);
Node create_newnode();
void split(Node ch,Node newparent); 
btree MakeEmpty(btree T);

#endif/*_BTREE_H*/

struct bTreenode
{
	int keynum; /*store keynum Data*/
	int data[M+1];  /*store each Data of the node*/
	btree child[M+1]; 
	btree parent;
	int is_leaf; /*1 represents it is a leaf,0 represents it isn't*/
};

int main()
{
	btree T=NULL;	
	int i;	/*for the loop*/
	int N;
	int X;
	char * s;
	
	T= initialize(T);  /*Initialize the btree*/
	
	scanf("%d",&N);
	for(i=0;i<N;i++)
	{
		scanf("%d",&X);
		T=insert(X,T);
	}
	
	printout_levelorder(T);

	T=MakeEmpty(T);
		system("pause");
	return 0;		
	system("pause");


}

btree initialize(btree T)
{
	T = create_newnode();
    if(T==NULL) {
   		printf("Out of Space!");
   		return NULL;
	}
    return T;
}

Node create_newnode()
{
	int i;
	Node newnode=NULL;
    newnode = (Node)malloc(sizeof(struct bTreenode));
    if(newnode==NULL) {    
		printf("Out of Space!");
		return NULL;
    }

    for(i = 0; i <= M; i++)      /*initialize the data*/
        newnode->data[i] = -1;

    for(i = 0; i <=M; i++) 		/*initialize the child*/   
        newnode->child[i] = NULL;

    newnode->keynum = 0;
    newnode->is_leaf = True;  
	newnode->parent = NULL;
	
	return newnode;
}


int search(int X,btree T)	/*1 represents X is in the btree,0 represents it isn't*/
{
	int i;
	if(T->is_leaf==True)         
	{
		for(i=0;i<T->keynum;i++)
		{
			if(X==T->data[i]) 	/*Find X*/
				return 1;
		}	
		return 0;              /*X is not in the leaf*/
	}
	if(T->is_leaf==False)
	{
		for(i=0;i<T->keynum;i++)
		{
			if(X==T->data[i])		/*Find X*/
				return 1;
			else if(X<T->data[i])       /*X is in the child*/
				return search(X,T->child[i]);		
		}
		return search(X,T->child[i]);
	}
}

btree  insert ( int X,  btree T ) 
{ 
	int i,j;
	/*The case that X is in the btree*/
	if(search(X,T)==1)        
	{
		printf("Key %d is duplicated\n",X);
		return T;
	}	
	
	/*Search from root to leaf for X and find the proper leaf node*/
	while(T->is_leaf==False)                          
	{
		for(i=0;i<T->keynum;i++)
		{
			if(X<T->data[i])      
			{
				T=T->child[i];
				break;
			}	
		}
		if(i==T->keynum)
			T=T->child[i];
	}	 
	/*End-Searching*/
	
	/*Insert X in the leaf*/
	for(i=0;i<T->keynum;i++)
	{
		if(X<T->data[i])
		{
			for(j=T->keynum;j>i;j--)
				T->data[j]=T->data[j-1];
			T->data[i]=X;
			break;			
		}			
	}
	if(i==T->keynum)
		T->data[i]=X;	
	T->keynum++;
	/*End-Inserting X*/
	
	/*while this node has M+1 keys or M+1 children*/
	while((T->is_leaf==True&&T->keynum==M+1)||T->is_leaf==False&&T->keynum==M)
	{
		if(T->parent==NULL)                    /*this node is the root.create a new root with two children;*/
			{
				Node new_root = create_newnode();
				new_root->keynum=0;
				new_root->child[0]=T;
				new_root->is_leaf=False;
				T->parent=new_root;	
			}
		split(T,T->parent);
		if(T->parent!=NULL)
			T=T->parent;	/*check its parent*/					
	}/*End-While*/
	
	/*Return the root*/
	while(T->parent!=NULL)
		T=T->parent;
	return T;
} 


void split(btree ch,btree newparent) /*split the node into 2 nodes with 2 keys, respectively*/
/*ch is the old child ,new_child is the new child. newparent is the 2 children's parent*/
{
	int i;
	Node new_child = create_newnode();
	  
	new_child->is_leaf = ch->is_leaf;
	
	int pos;            /*Get the position where ch is in the newparent->child[]*/	
	for(i=0;i<=newparent->keynum;i++)
	{
		if(newparent->child[i]==ch) 
		{
			pos=i;
			break;
		}
	}
		
	/*update the information when if is a leaf or isn't respectively*/ 
	if(new_child->is_leaf==True) 
	{
    	/*Update the child*/
		new_child->parent=newparent;	
		new_child->data[0]=ch->data[2];
		new_child->data[1]=ch->data[3];
		ch->keynum=(M+1)/2;
		new_child->keynum=(M+1)/2;
		
		/*update the parent*/
	    for(i = newparent->keynum; i > pos; i--)
			newparent->child[i+1] = newparent->child[i];
		newparent->child[pos+1] = new_child;

		for(i = newparent->keynum - 1; i >= pos; i--)
        	newparent->data[i+1] = newparent->data[i];
    	newparent->data[pos] = new_child->data[0];
    	
		newparent->keynum += 1;
	}
	
	if(new_child->is_leaf==False)
	{
		/*update the child*/
		new_child->parent=newparent;	
		new_child->data[0]=ch->data[M-1];
		ch->keynum=M/2;
		new_child->keynum=M/2;
		
	  	new_child->child[0]=ch->child[2];
	  	new_child->child[1]=ch->child[3];
	  	ch->child[2]->parent=new_child;
	  	ch->child[3]->parent=new_child;
	  	
	  	/*update the parent*/
	   	for(i = newparent->keynum; i > pos; i--)
			newparent->child[i+1] = newparent->child[i];
		newparent->child[pos+1] = new_child;

		for(i = newparent->keynum - 1; i >= pos; i--)
        	newparent->data[i+1] = newparent->data[i];
    	newparent->data[pos] = ch->data[M/2];
    	
		newparent->keynum += 1;  	
	}				
}

void  printout_levelorder(btree T)/*Use a queue to print the btree by levelorder*/
{	
    Node queue[10000] = {NULL};
	Node node=NULL;
    int front = 0;
    int rear = 0;
	int i;
	int store_rear;    /*Useful for reprenting whether one level has all been printed out*/
		
    queue[rear++] = T;    /*Enqueue*/
	store_rear=rear;

    while(front < rear) {
        node = queue[front++];  /*Dequeue*/

        printf("[");
        for(i = 0; i < node->keynum-1; i++)
            printf("%d,", node->data[i]);
       	printf("%d", node->data[i]);
        printf("]");
		
		for( i = 0; i <= node->keynum; i++) 
			if( node->child[i]!=NULL) 
	            queue[rear++] = node->child[i];               
    
		if(front==store_rear)   /*One level has been all printed out*/
		{
			printf("\n");
			store_rear=rear;			
		}
	}
}

btree MakeEmpty(btree T)   /*Free the memory*/
{
	int i;
	if(T!=NULL)
	{
		for(i=0;i<=T->keynum;i++)
		{
			T->child[i]=MakeEmpty(T->child[i]);
		}
		free(T);	
	}	
	return NULL;	
}
