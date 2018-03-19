#include <stdio.h>
#define t 3

struct node
{
	int key;
	char country[3];
	char grate[4];
	int score;
	int rate;
}key_array[1000001];
typedef struct node node_t;

struct treenode
{
	node_t x[2 * t];   //	node_t x[2 * t - 1];  //indexing from 1 not from 0
	
	int c[2 * t + 1]; // treenode* c[2 * t]; links
	int n;
	int leaf;
	//root

}tree_array[500000];

void create_Btree();
void insert(node_t ele);
void insert_nonfull(int temp,node_t ele);
void split_child(int temp,int i);
int search(int temp, int k);
void display();

void delete(int key);
void delete_from_leaf();
void delete_from_nonleaf(int k);
node_t get_pred(int c_ix);
node_t get_succ(int c_ix);
void merge(int in_p,int inx,int in_c1,int in_c2);


int root=1; //root starts from 1
int a=1;  //index of array which keeps increasing as we keep allocating
int n_ins=1000000; //number of elements insertng
int del=-1; //index of node to be deleted.
int in_x;
int flag;
//int n_nodes = 52; //number of nodes in tree_array


int main()
{
	FILE *fp = fopen("Assignment 3 - dataset.csv","r");
	int key;
	//store the file in an array.
	for(int i=1;i<=n_ins;i++)
		fscanf(fp,"%d,%[^,],%[^,],%d,%d",&key_array[i].key,key_array[i].country,key_array[i].grate,&key_array[i].score,&key_array[i].rate);

	create_Btree();
	//passing key,use for loop later
	printf("Btree being created...\n");
	for(int i=1;i<=n_ins;i++)
		insert(key_array[i]);
	printf("Btree created!\n");

	//display();
	
	//printf("\nNumber of nodes used: %d ",a);

	int s;
	while(1)
	{
		printf("\n------------------------------------------");
		printf("\n\nEnter\n1 to search key\n2 to delete node\n");
		scanf("%d",&s);
		switch(s)
		{
			case 1:
			{
				printf("Enter the key to be searched: ");
				scanf("%d",&key);
				search(root,key);
				if(flag)
				{
					printf("\n*** search successfull***\n");
					printf("Record with key %d is  ",key);
					printf("%s,%s,%d,%d",tree_array[del].x[in_x].country,tree_array[del].x[in_x].grate,tree_array[del].x[in_x].score,tree_array[del].x[in_x].rate);
				}
				else
				{
					printf("\n***search failed****\n");
					printf("Record not found\n");
				}
				break;
			}
			case 2:
			{
				printf("Enter key to be deleted: ");
				scanf("%d",&key);
				search(root,key);
				if(flag)
				{
					printf("\n***Deletion success***\n");
					printf("Record with key %d is  ",key);
					printf("%s,%s,%d,%d",tree_array[del].x[in_x].country,tree_array[del].x[in_x].grate,tree_array[del].x[in_x].score,tree_array[del].x[in_x].rate);
				}
				else
				{
					printf("\n***Deletion failed****\n");
					printf("Record not found\n");
				}
				delete(key);
				break;
			}
		}
	}

	printf("\n");
	return 0;
}

void delete(int key)
{

	if(flag)
	{
		if(tree_array[del].leaf == 1)
		{
			printf("\n-----this is leaf");
			delete_from_leaf();
			//printf("\n----Deleted----");
		}
		else
		{
			printf("\n-----not leaf");
			delete_from_nonleaf(key);
		}		
	}

	return;

}

void delete_from_leaf()
{
	for(int i=tree_array[del].n;i>in_x;i--)
	{
		//printf("%d",tree_array[del].x[i-1].key);
		tree_array[del].x[i-1] = tree_array[del].x[i];
	}
	tree_array[del].n--;
	return;
}

void delete_from_nonleaf(int k)
{
	int p_index = del;
	int c1_index = tree_array[del].c[in_x];
	int c2_index = tree_array[del].c[in_x+1];
	//if predecessor has atlest t keys
	if(tree_array[c1_index].n >= t)
	{
		node_t pred = get_pred(c1_index);
		tree_array[del].x[in_x] = pred;
		delete(pred.key);
	}
	//if successor has atleast t keys
	else if(tree_array[c2_index].n >= t)
	{
		node_t succ = get_succ(c2_index);
		tree_array[del].x[in_x] = succ;
		delete(succ.key);
	}
	else 
	{
    // If both C[idx] and C[idx+1] has less that t keys,merge k and all of C[idx+1]
    // into C[idx]
    // Now C[idx] contains 2t-1 keys
    // Free C[idx+1] and recursively delete k from C[idx]
		merge(p_index,in_x,c1_index,c2_index);
		delete(tree_array[del].x[in_x].key);
	}
	return;
}

// A function to merge C[idx] with C[idx+1]
void merge(int in_p,int inx,int in_c1,int in_c2)
{
    // Pulling a key from the current node and inserting it into (t-1)th
    // position of C[idx]
	tree_array[in_c1].x[t] = tree_array[del].x[in_x];

    // Copying the keys from C[idx+1] to C[idx] at the end
    for(int i=1;i<=tree_array[in_c2].n;i++)
    	tree_array[in_c1].x[t+i] = tree_array[in_c2].x[i];

    // Copying the child pointers from C[idx+1] to C[idx]
    if(tree_array[in_c1].leaf!=1)
    {
    	for(int i=1;i<tree_array[in_c2].n+1;i++)
    		tree_array[in_c1].c[t+i] = tree_array[in_c2].c[i];
    }

    // Moving all keys after idx in the current node one step before -
    // to fill the gap created by moving keys[idx] to C[idx]
    for(int i=inx;i<=tree_array[in_p].n;i++)
    	tree_array[in_p].x[i] = tree_array[in_p].x[i+1];

    // Moving the child pointers after (idx+1) in the current node one
    // step before
	for(int i=inx+1;i<=tree_array[in_p].n;i++)
		tree_array[in_p].c[i] = tree_array[in_p].c[i+1];

    // Updating the key count of child and the current node	
    tree_array[in_p].n--;   
	tree_array[in_c1].n += tree_array[in_c2].n+1;

	return;

}

//to get predecessor of key
node_t get_pred(int c_ix)
{
	// Keep moving to the right most node until we reach a leaf
	while(tree_array[c_ix].leaf!=1)
		c_ix = tree_array[c_ix].c[tree_array[c_ix].n+1];

	// Return the last node of the leaf
	return tree_array[c_ix].x[tree_array[c_ix].n];
}

//to get successor of the key
node_t get_succ(int c_ix)
{
	// Keep moving the left most node starting from C[idx+1] until we reach a leaf
	while(tree_array[c_ix].leaf!=1)
		c_ix = tree_array[c_ix].c[1];

	// Return the first key of the leaf
	return tree_array[c_ix].x[1];
}

//creating a btree 
void create_Btree()
{
	//initialize c[i] to -1
	for(int k=1;k<=52;k++)
		for(int i=1;i<2*t+1;i++)
			tree_array[k].c[i] = -1;

	tree_array[root].leaf = 1;	
	tree_array[root].n=0;
}

//inserting the element in the Btree
void insert(node_t ele)
{
	int r;
	r = root;
	if(tree_array[r].n == 2*t-1)
	{
		int s;
		s = ++a; //add new index
		root = s;
		tree_array[s].leaf = 0;
		tree_array[s].n=0;
		tree_array[s].c[1] = r; 
		split_child(s,1);   //pasing parent index and 1
		insert_nonfull(s,ele);
	}
	else
		insert_nonfull(r,ele);
}


//temp is the non full node
//takes index of root which is not full.
//when called recursively than it takes index of the nonfull parent 
void insert_nonfull(int temp,node_t ele)
{
	int i = tree_array[temp].n;
	if(tree_array[temp].leaf==1)
	{
		while(i>=1 && ele.key<tree_array[temp].x[i].key)
		{
			tree_array[temp].x[i+1] = tree_array[temp].x[i];
			i = i-1;
		}
		tree_array[temp].x[i+1] = ele;
		tree_array[temp].n = tree_array[temp].n+1;
	}
	else
	{
		while(i>=1 && ele.key<tree_array[temp].x[i].key)
			i = i-1;
		i = i+1;  //link of child
		//if temp's child is full split 
		if(tree_array[tree_array[temp].c[i]].n == 2*t-1)
		{
			split_child(temp,i);
			if(ele.key > tree_array[temp].x[i].key)
				i = i+1;
		}
		insert_nonfull(tree_array[temp].c[i],ele);
	}
}

//here temp is parent index of full child,and i is index of the c array in parent which stores index of child in c[i]
void split_child(int temp,int i)
{
	int j;
	int z,y;
	z = ++a;  //z also gets a new tree_array node.
	y = tree_array[temp].c[i];

	tree_array[z].leaf = tree_array[y].leaf;
	tree_array[z].n = t-1;

	for(j=1 ; j<= (t-1) ; j++)		
		tree_array[z].x[j] = tree_array[y].x[j+t];

	//if y i.e child is not a leaf,copy the remaining pointer(c[j]) to z
	if(tree_array[y].leaf == 0)
	{
		for(j=1;j<=t ;j++)			
			tree_array[z].c[j] = tree_array[y].c[j+t];
	}
	
	tree_array[y].n = t-1;

	for(j=tree_array[temp].n+1 ; j>=i+1 ;j--)
		tree_array[temp].c[j+1] = tree_array[temp].c[j];
	tree_array[temp].c[j+1] = z;

	for(j = tree_array[temp].n ; j>=i ; j--)
		tree_array[temp].x[j+1] = tree_array[temp].x[j];
	tree_array[temp].x[i] = tree_array[y].x[t];  //put median
	tree_array[temp].n = tree_array[temp].n+1;  

}


//searching for an element
int search(int temp, int k)
{
	int i=1;

	while(i<=tree_array[temp].n && k > tree_array[temp].x[i].key)
		i = i+1;
	if(i<=tree_array[temp].n && k == tree_array[temp].x[i].key)
	{
		del = temp;
		in_x = i;
		flag=1;
	}
	else if(tree_array[temp].leaf == 1)
	{
		flag=0;
		//return 0;
	}
	else
		return search(tree_array[temp].c[i],k);
}

//displays the fetched input from .csv file
void display()
{
	for(int i=1;i<=n_ins;i++)
		printf("%d,%s,%s,%d,%d\n",key_array[i].key,key_array[i].country,key_array[i].grate,key_array[i].score,key_array[i].rate);

}


