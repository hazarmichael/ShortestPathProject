/*
    Hazar Michael
    1201838
    sec: 1
*/

#include <stdio.h>
#include  <string.h>
#include  <stdlib.h>
#include <limits.h>

#define LENGTH 200

char cities [LENGTH][LENGTH];
int Graph[LENGTH][LENGTH]; //represents the graph using an adjacency matrix
int size=0;

//To find the number of a given city, if it was not in the array, then returns -1
int getCityNumber(char targetCity[]){
    for(int i=0;i<size;i++){
        if (strcmp(cities[i],targetCity)==0){
            return i;
        }
    }
    return -1;
}

//Read the file and fill the graph
void readFile()
{
    FILE *input = fopen("cities.txt","r");
    char line[LENGTH];
    char from[LENGTH],to[LENGTH],distance[LENGTH];
    
    // Read each line from the file
    while (fgets(line,LENGTH,input)!=NULL)
    {
        char *token = strtok(line,"\t");
        strcpy(from,token);
        token = strtok(NULL,"\t");
        strcpy(to,token);
        token = strtok(NULL,"\n");
        strcpy(distance,token);
        int cost = atoi(distance);
        if (getCityNumber(from)==-1)
        {
            strcpy(cities[size],from);
            size++;  // Increment the size of the cities array 
        }
        if (getCityNumber(to)==-1)
        {
            strcpy(cities[size],to);
            size++;
        }
        Graph[getCityNumber(from)][getCityNumber(to)]=cost;
        Graph[getCityNumber(to)][getCityNumber(from)]=cost;
    }
    fclose(input);
}


//The function below prints the path of cities
void printPath(int parent[], int destination)
{
    if (parent[destination] == - 1)
    {
        return; // If the parent of the destination is -1, we have reached the source city
    }
    printPath(parent, parent[destination]);
    printf("%s->",cities[parent[destination]]);  // Print the name of the city

}

//Find the min vertex of the distance array
int selectMinVertex(int distance[],int processed[])
{
    int minimum = INT_MAX;
    int vertex;
    for(int i=0; i<size; ++i)
    {
        if(processed[i]==0 && distance[i]<minimum)
        {
            vertex = i;
            minimum = distance[i];
        }
    }
    return vertex;  // Return the vertex with the minimum distance value
}

// function to print the path of cities from source to destination to an output file
void printPathOutputFile(int parent[], int destination, FILE* out)
{
    if (parent[destination] == - 1)
    {
        return;
    }
    printPathOutputFile(parent, parent[destination], out);
    fprintf(out,"%s->",cities[parent[destination]]);
}



// function to print the solution of Dijkstra's algorithm
void printSolutionDijkstra(int dist[], int parent[], int source,int destination, FILE*out)
{
    if (dist[destination]!=INT_MAX)
    {
        printf("Source to Destination\t\tDistance\t\tPath\n");
        printf("%s to %s \t\t %d km\t\t",cities[source], cities[destination], dist[destination]);
        fprintf(out,"using Dijkstra: %s to %s \t\t %d\t\t",cities[source], cities[destination], dist[destination]);
    }
    else
    {
        printf("Source to Destination\t Distance\tPath\n"); 
        printf("%s to %s \t\t No path found\n",cities[source], cities[destination]); 
        fprintf(out,"%s to %s \t\t No path found\n",cities[source], cities[destination]);
        return;
    }
    printPath(parent,destination);// print the path to the destination city
    printPathOutputFile(parent, destination, out);// print the path to the destination city to the output file 
    printf("%s\n",cities[destination]); 
    fprintf(out,"%s\n",cities[destination]); // print the destination city to the output file
}

//dijkstra function that calculate the shortest path between two cities 
void dijkstra(int source, int destination, FILE* out)
{
    int parent[size];
    int distance[size];
    int processed[size];
    for (int i = 0; i < size; i++)
        distance[i] = INT_MAX, processed[i] = 0,parent[i]=-1;

    distance[source] = 0;

    //Include (V-1) edges to cover all V-vertices
    for(int i=0; i<size-1; ++i)
    {
        //Select best Vertex by applying greedy method
        int U = selectMinVertex(distance,processed);
        processed[U] = 1;    //Include new Vertex in shortest Path Graph

        //Relax adjacent vertices (not yet included in shortest path graph)
        for(int j=0; j<size; ++j)
        {
            if(Graph[U][j]!=0 && processed[j]==0 && (distance[U]+Graph[U][j] < distance[j]))
            {
                distance[j] = distance[U]+Graph[U][j];
                parent[j] = U;
            }
        }
    }
    printSolutionDijkstra(distance,parent,source,destination,out);
}

// function to print the solution of BFS's algorithm
void printSolutionBFS(int dist[], int parent[], int source,int destination, FILE*out, int edges[])
{
    if (dist[destination]!=INT_MAX)
    {
        
        printf("Source to Destination\t\tDistance\t\tPath\n");
        printf("%s to %s \t\t %d km\t\t",cities[source], cities[destination], dist[destination]);
        fprintf(out,"using BFS: %s to %s \t\t %d\t\t%d",cities[source], cities[destination], dist[destination],edges[destination]);
    }
    else
    {
        printf("Source to Destination\t Distance\tPath\n");
        printf("%s to %s \t\t No path found\n",cities[source], cities[destination]);
        fprintf(out,"%s to %s \t\t No path found\n",cities[source], cities[destination]);
        return;
    }
    printPath(parent,destination);
    printPathOutputFile(parent, destination, out);
    printf("%s\n",cities[destination]);
    fprintf(out,"%s\n",cities[destination]);
}

// funtion to find the shortest path using Breadth First Search BFS
void bfs_shortest_path(int source, int destination, FILE* out) {
    int visited[size];
    int parent[size];
    int distance[size];
    int edges[size];  // Array to store the number of edges to reach each vertex
    memset(visited, 0, sizeof(visited));
    memset(parent, -1, sizeof(parent));
    memset(distance, -1, sizeof(distance));
    memset(edges, -1, sizeof(edges));

    visited[source] = 1;
    distance[source] = 0;
    edges[source] = 0;
    parent[source] = -1;

    // Create a queue for BFS
    int queue[size];
    int front = 0, rear = 0;
    queue[rear++] = source;

    while (front < rear) {
        int current = queue[front++];
        if (current == destination)
            break;

        for (int i = 0; i < size; i++) {
            if (Graph[current][i] != 0 && !visited[i]) {
                queue[rear++] = i;
                visited[i] = 1;
                distance[i] = distance[current] + Graph[current][i];
                edges[i] = edges[current] + 1;  // Increment the number of edges
                parent[i] = current;
            }
        }
    }

    if (distance[destination] != -1) {
        printSolutionBFS(distance, parent, source, destination, out, edges);
        printf("And the number of edges is: %d\n\n", edges[destination] );

    } else {
        printf("Source to Destination\t Distance\tEdges\tPath\n");
        printf("%s to %s \t\t No path found\n", cities[source], cities[destination]);
    }
}

int main()
{
    int choice=0,readingFileFlag=0,source=-1,destination;
    FILE* out = fopen("shortest_path.txt","w");
    fprintf(out,"\t\t\tSource to Destination\t\t Distance\tPath\n");


    while (choice!=5)     // run the menu loop until the user chooses to exit

    {
        printf("**************      MENU      **************\n");
        printf("please choose an operation\n"
        "1-Load Cities\n"
        "2-Enter Source City\n"
        "3-Enter Destination City and find the shortest path and distance using dijkstra.\n"
        "4-Enter Destination City and find the path and distance using Breadth First Search.\n"
        "5-Exit the program and save the results to the output file. \n\n");


        scanf("%d",&choice);
        switch (choice)
        {
        case 1: // load the cities from the file
            readingFileFlag=1;
            readFile(); 
            printf("Data has been loaded from the file successfully!\n");
            break;

        case 2 : // enter the source city 
            if(readingFileFlag==0)
                printf("Data should be loaded from the file first to implement the graph. \n\n");
            else
            {
                for(int i=0;i<size;i++){
                    printf("%d %s\n",i,cities[i]);
                }
                printf("Please enter the source city number from the list above : \n");
                scanf("%d",&source);
                while(source<0 || source>size-1){
                    printf("Please enter the source city number from the list above : \n");
                    scanf("%d",&source);
                }
            }
            break;
        case 3: // calculate the dijkstra for a chosen destination
            if(readingFileFlag==0)
                printf("The file should be read first.\n\n");
            else if(source==-1)
                printf("The source city should be determined first.\n");
            else
            {
                for(int i=0;i<size;i++){
                    printf("%d %s\n",i,cities[i]);
                }
                printf("Please enter the destination city number from the list above : \n");
                scanf("%d",&destination);
                while(destination < 0 || destination>size-1){
                    printf("Please enter the destination city number from the list above : \n");
                    scanf("%d",&destination);
                }
                printf("\nThe full route of the shortest path and the distance between these two cities is: \n\n");
                dijkstra(source,destination,out);
            }
            break;


        case 4: // calculate the BFS for a chosen destination
         if(readingFileFlag==0)
                printf("The file should be read first.\n\n");
            else if(source==-1)
                printf("The source city should be determined first.\n");
            else
            {
                for(int i=0;i<size;i++){
                    printf("%d %s\n",i,cities[i]);
                }
                printf("Please enter the destination city number from the list above : \n");
                scanf("%d",&destination);
                while(destination < 0 || destination>size-1){
                    printf("Please enter the destination city number from the list above : \n");
                    scanf("%d",&destination);
                }
                printf("\nThe full route of the path and the distance between these two cities is: \n\n");
                bfs_shortest_path(source,destination,out);
            }
            break;

        case 5: // exit the program and save the data to the output file 
           break;
        default:
            printf("Undefined operator. Please enter your choice between 1-4.\n");
            break;
        }
    }
    printf("Program exited. Thank you and have a good day!\n");
    fclose(out);
    return 0;
}