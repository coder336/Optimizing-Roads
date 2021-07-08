#include <iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

//structure mypair to store a pair of two integers
struct mypair
{
	int first,second;
};

//class to store a dynamically allocated array of mypairs
class myvect
{
    private:
	/*
	ar stores the array, length is the current size of the array
	and max_size is the maximum no of elements the dynamically allocated array
	can store currently
	*/
	mypair *ar;
	int length,max_size;

    public:
	//constructor for the class
	myvect()
	{
		ar=(mypair *)malloc(sizeof(mypair));
		max_size=1;
		length=0;
	}

	//function to add a new pair into the array
	void add(int x,int y)
	{
		if(length==max_size)
		{
			ar=(mypair *)realloc(ar,(2*length)*sizeof(mypair));
			max_size*=2;
		}
		mypair tmp={x,y};
		ar[length]=tmp;
		length++;
		return;
	}

	//function that returns a pair stored at a given index
	mypair get(int index)
	{
		return ar[index];
	}

	//function to return the current size of the array
	int size()
	{
		return length;
	}
};

//function that finds the bridges and assigns a direction to each edge by doing a DFS
void direct_edges(int v,myvect *adjacency,myvect edges,int *lower,int *tin,bool *used_edges,char orient_edges[])
{
	//static variable time which increments itself continously as the DFS runs.
    static int time = 0;
    lower[v] = tin[v] = time++;
    for (int i=0;i<adjacency[v].size();i++)
    {
    	mypair p=adjacency[v].get(i);
    	//if the edge is already used no need to go further
        if (used_edges[p.second])
        	continue;
        used_edges[p.second] = true;
        //we assign direction to an edge as: from the parent node to the child node in the DFS.
        if( v == (edges.get(p.second)).first)
        	orient_edges[p.second] = '>';
        else
        	orient_edges[p.second] = '<';
        int nv = p.first;
        if (tin[nv] == -1)
        {
        	// if nv is not visited yet
            direct_edges(nv,adjacency,edges,lower,tin,used_edges,orient_edges);
            if (lower[nv] > tin[v])
            {
                //  v--nv is a bridge
                orient_edges[p.second]='?';
            }
        }
        lower[v] = min(lower[v], lower[nv]);
    }
}

//main function that takes the input from the user and then prints the desired output
int main()
{
    int n, m;
    cout<<"\nEnter the number of nodes (intersections): ";
    scanf("%d", &n);
    cout<<"\nEnter the number of edges (paths): ";
    scanf("%d", &m);
    // constructing the adjacency list which will contain adjacent vertex (first element) and edge no (second element)
    myvect *adjacency=new myvect[n+1];
    //list of edges represented by the edge number of a pair of vertices
    myvect edges;
    /*
    tin array is used to store the In time (or entry time) of a node while doing a DFS.
    array lower is used to store the minimum between tin of a particular node and the tin of its adjacent nodes
    (which can be a tree edge or a back edge)
    used_edges is a boolean array which stores whether an edge has been visited or not during DFS.
    orient_edges is a string which stores the way in which an edge is to be directed in the final graph.
    */
    int *tin=(int *)malloc((n+1)*sizeof(int));
    for(int i=0;i<n+1;i++)
    	tin[i]=-1;
    int *lower=(int *)malloc((n+1)*sizeof(int));
    for(int i=0;i<n+1;i++)
    	lower[i]=-1;
    bool *used_edges=(bool *)malloc(m*sizeof(bool));
    for(int i=0;i<m;i++)
    	used_edges[i]=false;
    char orient_edges[m];

    //bridge_cnt stores the no of bridges in the graph (which cant be assigned a particular direction)
    int bridge_cnt=0;
    cout<<"\nEnter the edges (paths):\n";

    //taking the undirected graph as input and forming adjacency list
    for (int i = 0; i < m; i++)
    {
        int x, y;
        cin>>x>>y;
        adjacency[x].add(y, i);
        adjacency[y].add(x, i);
        edges.add(x, y);
    }

    //calling direct_edges to assign a direction to each path
    direct_edges(1,adjacency,edges,lower,tin,used_edges,orient_edges);

    //printing the desired graph and the number of bidirectional paths
    cout<<"\nThe desired final graph is as follows: \n";

    for (int i = 0; i < m; ++i)
    	if(orient_edges[i]=='>')
    		cout<<(edges.get(i)).first<<" --> "<<(edges.get(i)).second<<"\n";
    	else if(orient_edges[i]=='<')
    		cout<<(edges.get(i)).first<<" <-- "<<(edges.get(i)).second<<"\n";
    	else
    	{
    		cout<<(edges.get(i)).first<<" <--> "<<(edges.get(i)).second<<"\n";
    		bridge_cnt++;
    	}
    cout<<"The number of roads which have to be kept bidirectional: "<<bridge_cnt<<"\n";
    return 0;
}
