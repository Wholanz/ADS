#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifndef _AVLTREE_H/*AVL树声明*/

struct Avlnode;
typedef struct Avlnode *Avltree;
typedef struct Avlnode *Avlposition;

Avltree Amakeempty( Avltree T );
Avlposition Afind( char *s,Avltree T );
Avlposition Afindmin( Avltree T );
Avlposition Afindmax( Avltree T );
Avltree Ainsert( char *s, Avltree T );
Avltree Adelete( char *s, Avltree T ); 
Avlposition singlerotatewithleft( Avlposition k2 );
Avlposition singlerotatewithright( Avlposition k2 );
Avlposition doublerotatewithleft( Avlposition k3 );
Avlposition doublerotatewithright( Avlposition k3 );
int Aheight( Avltree T );

#endif /*_AVLTREE_H*/

#ifndef _TRIE_H/*字典树声明*/

struct Trienode;
typedef struct Trienode *Ttree;
typedef struct Trienode *Tposition;

Ttree Tmakeempty( Ttree T );
Tposition Tfind( char *s,Ttree T );
void Tinsert( char *s,Ttree T );
void Tdelete( char *s,Ttree T);
int wordtonumber( char ch );/*将字母转换成序号*/
int max( int a,int b );

#endif/*_TRIE_H*/



struct Trienode
{
	int eow;/*end of word,0表示非词尾，1表示词尾*/
	Tposition next[26];
};

struct Avlnode
{
	char s[35];/*最长的通用单词有34个字母*/
	Avltree left;
	Avltree right;
	int height;
};

Avltree Amakeempty( Avltree T ) /*清空树*/
{
	if( T != NULL )
	{
		Amakeempty( T->left );
		Amakeempty( T->right );
		free( T );
	}
	return NULL;
}

Avlposition Afind( char *s, Avltree T )/*查找*/
{
	if( T==NULL )
		return NULL;
	if( strcmp(s,T->s)<0 )
		return Afind( s,T->left );
	else if( strcmp(s,T->s)>0 )
		return Afind( s,T->right );
	else
		return T;
}

Avlposition Afindmin( Avltree T )
{
	if( T==NULL )
		return NULL;
	else if( T->left==NULL )
		return T;
	else return Afindmin( T->left );
}

Avlposition Afindmax( Avltree T )
{
	if( T==NULL )
		return NULL;
	else if( T->right==NULL )
		return T;
	else return Afindmax( T->right );
}

Avltree Ainsert( char *s,Avltree T )/*插入*/
{
	if( T==NULL )
	{
		T=( struct Avlnode * )malloc( sizeof( struct Avlnode ) );
		if( T==NULL )
		{
			printf("Out of space!!!\n");
			exit(0);
		}
		else
		{
			strcpy(T->s,s);
			T->height=0;
			T->left=T->right=NULL;
		}
	}
	else if( strcmp( s,T->s )<0 )
	{
		T->left = Ainsert( s,T->left );
		if( Aheight(T->left)-Aheight(T->right)==2 )
			if( strcmp( s,T->left->s )<0 )
				T=singlerotatewithleft( T );
			else
				T=doublerotatewithleft( T );
	}
	else if( strcmp( s,T->s )>0 )
	{
		T->right=Ainsert( s,T->right );
		if( Aheight(T->right)-Aheight(T->left)==2 )
			if( strcmp( s,T->right->s )>0 )
				T=singlerotatewithright( T );
			else
				T=doublerotatewithright( T );
	}
	else /*说明要插入的内容已经存在，可以直接返回*/
	{
		return T;
	}
	T->height=max( Aheight( T->left ),Aheight( T->right) )+1;
	return T;
}

Avltree Adelete( char *s,Avltree T )/*删除*/
{
	Avlposition TmpCell;

	if( T==NULL )
		return NULL;
	else
	{
		if( strcmp(s,T->s)<0 )
		{
			T->left=Adelete( s,T->left );
			if( Aheight(T->right)-Aheight(T->left)==2 )
			{
				if( T->right->left )
					T=doublerotatewithright( T );
				else
					T=singlerotatewithright( T );
			}
		}
		else if( strcmp( s,T->s )>0 )
		{
			T->right=Adelete( s,T->right );
			if( Aheight(T->left)-Aheight(T->right)==2 )
			{
				if( T->left->right )
					T=doublerotatewithleft( T );
				else
					T=singlerotatewithleft( T );
			}
		}
		else if ( T->left&&T->right )
		{
			TmpCell=Afindmin( T->right );
			strcpy( T->s,TmpCell->s );
			T->right=Adelete(T->s,T->right);
		}
		else
		{
			TmpCell=T;
			if( T->left == NULL )
				T = T->right;
			else if( T->right==NULL )
				T = T->left;
			free( TmpCell );
		}
		if( T!=NULL )
			T->height=max(Aheight(T->left),Aheight(T->right))+1;
	}
	return T;
}

Avlposition singlerotatewithleft( Avlposition k2 )/*左-左单旋转*/
{
	Avlposition k1;

	k1=k2->left;
	k2->left=k1->right;
	k1->right=k2;
	k2->height=max( Aheight( k2->left ),Aheight( k2->right ) )+1;
	k1->height=max( Aheight( k1->left ),k2->height )+1;
	return k1;
}

Avlposition singlerotatewithright( Avlposition k1 )/*右-右单旋转*/
{
	Avlposition k2;
	
	k2=k1->right;
	k1->right=k2->left;
	k2->left=k1;
	k1->height=max( Aheight( k1->left ),Aheight( k2->right ) )+1;
	k2->height=max( Aheight( k2->right),k1->height )+1;
	return k2;
}

Avlposition doublerotatewithleft( Avlposition k3 )/*左-右双旋转*/
{
	k3->left=singlerotatewithright( k3->left );
	return singlerotatewithleft( k3 );
}

Avlposition doublerotatewithright( Avlposition k3 )/*右-左双旋转*/
{
	k3->right=singlerotatewithleft( k3->right );
	return singlerotatewithright( k3 );
}

int max( int a,int b )/*求两者之间较大值*/
{
	if( a>b )
		return a;
	else return b;
}

int Aheight( Avltree T )/*求AVL树节点高度，以处理子树为NULL情形*/
{
	if( T==NULL )
		return -1;
	else
		return T->height;
}

Ttree Tmakeempty( Ttree T )/*清空字典树*/
{
	int i;

	if( T!=NULL )
	{
		for(i=0;i<26;i++)
			Tmakeempty( T->next[i] );
		free( T );
	}
	return NULL;
}

void Tinsert( char *s,Ttree T )/*字典树插入*/
{
	int i=0,n,j;

	while( s[i] )
	{
		n=wordtonumber( s[i] );
		if( T->next[n]==NULL )
		{
			T->next[n]=( Ttree )malloc( sizeof( struct Trienode ) );
			T=T->next[n];
			if( T==NULL )
			{
				printf("Out of sapce!!!\n");
				exit(0);
			}
			for( j=0;j<26;j++)
				T->next[j]=NULL;
		}
		else
			T=T->next[n];
		i++;
	}
	T->eow=1;
}

Tposition Tfind( char *s,Ttree T )/*字典树查找*/
{
	int i=0,n;

	while( s[i] )
	{
		n=wordtonumber( s[i] );
		if( T->next[n]==NULL )
			return NULL;
		else
		{
			T=T->next[n];
			i++;
		}
	}
	if( T->eow==1 )
		return T;
	else
		return NULL;
}

void Tdelete( char *s,Ttree T )/*字典树删除*/
{
	Tposition p;
	p=Tfind( s,T );
	if( p!=NULL )
		p->eow=0;/*只删除最后一个字母的词尾标志即可*/
}

int wordtonumber( char ch )/*将字母转换成序号*/
{
	if( ch>='a'&&ch<='z' )
		return ch-'a';
	else if( ch>='A'&&ch<='Z' )
		return ch-'A';
	else
	{
		printf("Illegal character!!!\n");
		return -1;
	}
}

int main()
{
	clock_t start,end;
	FILE *input;
	char str[35],fname[100];
	int ctrl;
	int count,size;
	double average;
	Avltree avl=NULL;
	Ttree trie=NULL;
	Avlposition pa;
	Tposition ta;
	int i;

	while(1)/*便于测试*/
	{
		system("cls");
		printf("Please choose the number of what we will do.\n");
		printf("(1)Insertion of AVL tree.\n");
		printf("(2)Search of AVl tree.\n");
		printf("(3)Deletion of AVL tree.\n");
		printf("(4)Insertion of Trie.\n");
		printf("(5)Search of Trie.\n");
		printf("(6)Deletion of Trie.\n");
		printf("(7)Clear the memory.\n");
		printf("(0)Exit.\n");
		scanf("%d",&ctrl);
		switch(ctrl)
		{
		case 0:
			exit(0);
			break;
		case 1:
			printf("Please input the filename.\n");
			scanf("%s",&fname);
			input=fopen(fname,"r");
			if( input==NULL )
			{
				printf("Open error!\n");
				exit(0);
			}
			count=size=0;
			average=0;
			start=clock();/*开始计时*/
			while(!feof(input))
			{
				fscanf(input,"%s",&str);
				size+=strlen(str);
				count++;
				avl=Ainsert(str,avl);
			}
			end=clock();/*结束计时*/
			average=size*1.0/count;
			printf("Insertion cost %f seconds. There are %d words and the average size is %f.\n",(end-start)*1.0/CLK_TCK,count,average);
			fclose(input);
			break;
		case 2:
			printf("Please input the filename.\n");
			scanf("%s",&fname);
			input=fopen(fname,"r");
			if( input==NULL )
			{
				printf("Open error!\n");
				exit(0);
			}
			count=size=0;
			average=0;
			start=clock();
			while(!feof(input))
			{
				fscanf(input,"%s",&str);
				size+=strlen(str);
				count++;
				pa=Afind(str,avl);
			}
			end=clock();
			average=size*1.0/count;
			printf("Search cost %f seconds. There are %d words and the average size is %f.\n",(end-start)*1.0/CLK_TCK,count,average);
			fclose(input);
			break;
		case 3:
			printf("Please input the filename.\n");
			scanf("%s",&fname);
			input=fopen(fname,"r");
			if( input==NULL )
			{
				printf("Open error!\n");
				exit(0);
			}
			count=size=0;
			average=0;
			start=clock();
			while(!feof(input))
			{
				fscanf(input,"%s",&str);
				size+=strlen(str);
				count++;
				avl=Adelete(str,avl);
			}
			end=clock();
			average=size*1.0/count;
			printf("Deletion cost %f seconds. There are %d words and the average size is %f.\n",(end-start)*1.0/CLK_TCK,count,average);
			fclose(input);
			break;
		case 4:
			printf("Please input the filename.\n");
			scanf("%s",&fname);
			input=fopen(fname,"r");
			if( input==NULL )
			{
				printf("Open error!\n");
				exit(0);
			}
			count=size=0;
			average=0;
			trie=(struct Trienode *)malloc(sizeof(struct Trienode));
			for(i=0;i<26;i++)
				trie->next[i]=NULL;
			start=clock();
			while(!feof(input))
			{
				fscanf(input,"%s",&str);
				size+=strlen(str);
				count++;
				Tinsert(str,trie);
			}
			end=clock();
			average=size*1.0/count;
			printf("Insertion cost %f seconds. There are %d words and the average size is %f.\n",(end-start)*1.0/CLK_TCK,count,average);
			fclose(input);
			break;
		case 5:
	        printf("Please input the filename.\n");
			scanf("%s",&fname);
			input=fopen(fname,"r");
			if( input==NULL )
			{
				printf("Open error!\n");
				exit(0);
			}
			count=size=0;
			average=0;
			start=clock();
			while(!feof(input))
			{
				fscanf(input,"%s",&str);
				size+=strlen(str);
				count++;
				ta=Tfind(str,trie);
			}
			end=clock();
			average=size*1.0/count;
			printf("Search cost %f seconds. There are %d words and the average size is %f.\n",(end-start)*1.0/CLK_TCK,count,average);
			fclose(input);
			break;
		case 6:
			printf("Please input the filename.\n");
			scanf("%s",&fname);
			input=fopen(fname,"r");
			if( input==NULL )
			{
				printf("Open error!\n");
				exit(0);
			}
			count=size=0;
			average=0;
			start=clock();
			while(!feof(input))

			{
				fscanf(input,"%s",&str);
				size+=strlen(str);
				count++;
				Tdelete(str,trie);
			}
			end=clock();
			average=size*1.0/count;
			printf("Deletion cost %f seconds. There are %d words and the average size is %f.\n",(end-start)*1.0/CLK_TCK,count,average);
			fclose(input);
			break;
		case 7:
			avl=Amakeempty(avl);
			trie=Tmakeempty(trie);
			break;
		default:
			break;
		}
		system("pause");
	}
	return 0;
}
