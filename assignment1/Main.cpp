#include "Mesh.h"
#include "Painter.h"
#include "FibonacciHeap.h"
#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <limits>
#include <iostream>
#include <fstream>
#include <chrono>

typedef pair<float, int> Pair;

int minDistanceIndex(vector<float> shortest_dist, vector<bool> isVisited, int n)
{
	// Initialize min value
	float min_distance = numeric_limits<float>::infinity();
	int min_index;

	for (int v = 0; v < n; v++) {
		if (isVisited[v] == false && shortest_dist[v] <= min_distance) {
			min_distance = shortest_dist[v];
			min_index = v;
		}
	}

	return min_index;
}

void dijkstraArray(Mesh* mesh, int src, vector<float> &shortest_dist, vector<int> &predecessor, int n)
{
	//vector, first: distance second: vertex id
	vector<bool> isVisited(n, false); //initially all unvisited								
	shortest_dist[src] = 0.0;

	/*  (while vertices remain unvisited) */
	for (int c = 0; c < n; c++)
	{
		//visit unvisited vertex with smallest known distance from start vertex
		int current = minDistanceIndex(shortest_dist, isVisited, n);
		//array.erase(array.begin());

		// 'i' is used to get all adjacent vertices of a vertex
		vector<int>::iterator i;
		int adj_count = 0;
		for (i = mesh->verts[current]->vertList.begin(); i < mesh->verts[current]->vertList.end(); ++i)
		{
			int adj_vertex_id = *i;
			if (isVisited[adj_vertex_id] == false) {
				// Get length of adjacent edge with current vertex.
				float length_of_edge = mesh->verts[current]->adjVertLengthList[adj_count];

				//  If there is shorted path to adj_vertex_id through current.
				if (shortest_dist[adj_vertex_id] > shortest_dist[current] + length_of_edge)
				{
					// Updating distance of adj_vertex_id
					shortest_dist[adj_vertex_id] = shortest_dist[current] + length_of_edge;

					//update the prev vertex with the current vertex
					predecessor[adj_vertex_id] = current;
				}
			}
			isVisited[current] = true;
			adj_count++;
		}
	}
}

void dijkstraMinHeap(Mesh* mesh, int src, vector<float> &shortest_dist, vector<int> &predecessor, int n)
{
	//min priority queue, first: distance second: vertex id
	priority_queue< Pair, vector <Pair>, greater<Pair> > pq;

	vector<bool> isVisited(n, false); //initially all unvisited

	pq.push(make_pair(0.0, src)); // let distance of start vertex from start_vertex = 0
	shortest_dist[src] = 0.0;

	/* Looping till priority queue becomes empty (while vertices remain unvisited) */
	while (!pq.empty())
	{
		//visit unvisited vertex with smallest known distance from start vertex
		int current = pq.top().second;
		pq.pop();

		if (isVisited[current] == false) {
			// 'i' is used to get all adjacent vertices of a vertex
			vector<int>::iterator i;
			int adj_count = 0;
			for (i = mesh->verts[current]->vertList.begin(); i < mesh->verts[current]->vertList.end(); ++i)
			{
				int adj_vertex_id = *i;
				if (isVisited[adj_vertex_id] == false) {
					// Get length of adjacent edge with current vertex.
					float length_of_edge = mesh->verts[current]->adjVertLengthList[adj_count];
						//mesh->getLengthOfEdgeByAdjacentVertexId(current, adj_vertex_id);

					//  If there is shorted path to adj_vertex_id through current.
					if (shortest_dist[adj_vertex_id] > shortest_dist[current] + length_of_edge)
					{
						// Updating distance of adj_vertex_id
						shortest_dist[adj_vertex_id] = shortest_dist[current] + length_of_edge;

						pq.push(make_pair(shortest_dist[adj_vertex_id], adj_vertex_id));
						//update the prev vertex with the current vertex
						predecessor[adj_vertex_id] = current;
					}
				}
				adj_count++;
			}
			isVisited[current] = true;
		}
	}
}


void dijkstraFibonacciHeap(Mesh* mesh, int src, vector<float> &shortest_dist, vector<int> &predecessor, int n)
{
	//fibonacci heap, first: distance second: vertex id
	FibonacciHeap<Pair> fh;

	vector<bool> isVisited(n, false); //initially all unvisited

	fh.insert(make_pair(0.0, src)); // let distance of start vertex from start_vertex = 0
	shortest_dist[src] = 0.0;

	/* Looping till priority queue becomes empty (while vertices remain unvisited) */
	while (!fh.isEmpty())
	{
		//visit unvisited vertex with smallest known distance from start vertex
		int current = fh.getMinimum().second;
		fh.removeMinimum();

		if (isVisited[current] == false) {
			// 'i' is used to get all adjacent vertices of a vertex
			vector<int>::iterator i;
			int adj_count = 0;
			for (i = mesh->verts[current]->vertList.begin(); i < mesh->verts[current]->vertList.end(); ++i)
			{
				int adj_vertex_id = *i;
				if (isVisited[adj_vertex_id] == false) {
					// Get length of adjacent edge with current vertex.
					float length_of_edge = mesh->verts[current]->adjVertLengthList[adj_count];

					//  If there is shorted path to adj_vertex_id through current.
					if (shortest_dist[adj_vertex_id] > shortest_dist[current] + length_of_edge)
					{
						// Updating distance of adj_vertex_id
						shortest_dist[adj_vertex_id] = shortest_dist[current] + length_of_edge;

						fh.insert(make_pair(shortest_dist[adj_vertex_id], adj_vertex_id));
						//update the prev vertex with the current vertex
						predecessor[adj_vertex_id] = current;
					}
				}
				adj_count++;
			}
			isVisited[current] = true;
			
		}
	}
}

void printDistanceArray(vector<float> dist) {
	vector<float>::iterator ptr;
	ofstream myfile;
	myfile.open("geodesic distance matrix.txt");

	cout << "The distance vector elements are : ";
	for (ptr = dist.begin(); ptr < dist.end(); ptr++){
		cout << *ptr << " ";
		myfile << *ptr << " ";
	}
	cout << "\n";

	myfile.close();
}


void printDistanceArrayToFile(Mesh* mesh) {
	int n = mesh->verts.size();
	float infinity = numeric_limits<float>::infinity();
	
	ofstream myfile;
	const char *path = "geodesic-distance-matrix.txt";
	myfile.open(path);

	//cout << "file writing started\n";
	int start_vertex;
	// Printing the desired unique matrix
	for (int i = 0; i < n; i++)
	{
		vector<float> shortest_dist(n, infinity); //// Create a vector for distances and initialize all distances as infinite (INF)
		vector<int> predecessor(n); //previous vertex array
		start_vertex = i;

		dijkstraFibonacciHeap(mesh, start_vertex, shortest_dist, predecessor, n);

		vector<float>::iterator ptr;
		for (ptr = shortest_dist.begin(); ptr < shortest_dist.end(); ptr++) {
			myfile << *ptr << " ";
		}
		myfile << "\n";

		shortest_dist.clear();
		predecessor.clear();
	}

	//cout << "file writing finished\n";
	myfile.close();
}

void printPredecessorArray(vector<int> predecessor) {
	vector<int>::iterator ptr;
	
	cout << "The predecessor vector elements are : ";
	for (ptr = predecessor.begin(); ptr < predecessor.end(); ptr++) 
		cout << *ptr << " ";
	
	cout << "\n";
}

void findShortestPath(Mesh* mesh, int src, int dest, vector<int> predecessor, vector<int>& shortest_path_edges, vector<int> & shortest_path_vertices) {

	int current = dest;
	shortest_path_vertices.push_back(current);

	while (current != src) {
		int prev = predecessor[current];
		shortest_path_vertices.push_back(prev);
		int edge_id = mesh->getEdgeId(prev, current);
		shortest_path_edges.push_back(edge_id);
		current = prev;
	}

	reverse(shortest_path_edges.begin(), shortest_path_edges.end());
	reverse(shortest_path_vertices.begin(), shortest_path_vertices.end());
	
}

pair<int, float> maxDistanceAndIndex(vector<float> arr, int n)
{
	// Initialize min value
	float max_distance = 0.0;
	int max_index;

	for (int v = 0; v < n; v++) {
		if (arr[v] >= max_distance) {
			max_distance = arr[v];
			max_index = v;
		}
	}

	return make_pair(max_index, max_distance);
}

pair<int, float> maxDistanceAndIndex(vector<pair<int, float>> arr, int n)
{
	// Initialize min value
	float max_distance = 0.0;
	int max_index;

	for (int v = 0; v < n; v++) {
		if (arr[v].second >= max_distance) {
			max_distance = arr[v].second;
			max_index = arr[v].first;
		}
	}

	return make_pair(max_index, max_distance);
}

float minDistance(float* arr, int n)
{
	float infinity = numeric_limits<float>::infinity();
	// Initialize min value
	float min_distance = infinity;

	for (int v = 0; v < n; v++) {
		if (arr[v] < min_distance && arr[v]>0.0) {
			min_distance = arr[v];
		}
	}

	return min_distance;
}

void getFPSPoints(Mesh* mesh, int source,int n, vector<int> &farthestPointIndices, int fps_count) {

	float infinity = numeric_limits<float>::infinity();

	farthestPointIndices[0] = source;

	//allocate the array
	float** geodesic_dist = new float*[fps_count];
	for (int i = 0; i < fps_count; i++)
		geodesic_dist[i] = new float[n];

	//float geodesic_dist[10][1000];
	for (int i = 1; i < fps_count; i++) {

		float max_dist = 0.0;
		int max_dist_index = 0;

		vector<float> shortest_dist(n, infinity); // Create a vector for distances and initialize all distances as infinite (INF)
		vector<int> predecessor(n); //previous vertex array

		dijkstraFibonacciHeap(mesh, farthestPointIndices[i-1], shortest_dist, predecessor, n);

		for (int j = 0; j < n; j++) {
			geodesic_dist[i-1][j] = shortest_dist[j];
		}

		for (int j = 0; j < n; j++) {

			float min_dist = infinity;
			for (int t = 0; t < i; t++) {		
				float current_dist = geodesic_dist[t][j];
				if (current_dist < min_dist)
					min_dist = current_dist;
			}

			if (max_dist < min_dist)
			{
				max_dist_index = j;
				max_dist = min_dist;
			}

		}
		farthestPointIndices[i] = max_dist_index;

		shortest_dist.clear();
		predecessor.clear();
	}

	//deallocate the array
	for (int i = 0; i < 10; i++)
		delete[] geodesic_dist[i];
	delete[] geodesic_dist;

}

pair<int, float> maxVertexByAGD(Mesh* mesh) {

	float infinity = numeric_limits<float>::infinity();
	
	const int n = mesh->verts.size();

	vector<float> cumulative_dist(n);
	for (int i = 0; i < 10; i++) {
		
		vector<float> shortest_dist(n, infinity); //// Create a vector for distances and initialize all distances as infinite (INF)
		vector<int> predecessor(n); //previous vertex array

		int start_vertex = i;
		dijkstraFibonacciHeap(mesh, start_vertex, shortest_dist, predecessor, n);

		float cumulative = 0;
		for (int j = 0; j < n; j++) {
			cumulative += shortest_dist[j];
		}
		cumulative_dist[i] = cumulative;

		shortest_dist.clear();
		predecessor.clear();
	}

	return maxDistanceAndIndex(cumulative_dist, n);

}

bool localMaxOneRingNeighbor(Mesh* mesh, int vertex_id, vector<float> geodesic_dist) {

	int adj_vert_count = mesh->verts[vertex_id]->vertList.size();

	float current_geodesic_dist = geodesic_dist[vertex_id];
	int count = 0;
	for (int i = 0; i < adj_vert_count; i++) {
		int adj_index = mesh->verts[vertex_id]->vertList[i];
		if (current_geodesic_dist > geodesic_dist[adj_index]) {
			count++;
		}
	}

	if (count != 0 && count == adj_vert_count) {
		return true;
	}
	else return false;
}


void averageOverLocalRing(Mesh* mesh, int n, vector<float>& average_geodesic_dist) {

	
	for (int v = 0; v < n; v++) {
		int adj_vert_count = mesh->verts[v]->vertList.size();
		float avg_dist = 0.0;

		int count = 0;
		for (int i = 0; i < adj_vert_count; i++) {
			int adj_index = mesh->verts[v]->vertList[i];
			avg_dist += average_geodesic_dist[adj_index];

		}
		average_geodesic_dist[v] = avg_dist / adj_vert_count;
	}

}

bool localMinOneRingNeighbor(Mesh* mesh, int vertex_id, vector<float> average_geodesic_dist) {

	int adj_vert_count = mesh->verts[vertex_id]->vertList.size();

	float current_geodesic_dist = average_geodesic_dist[vertex_id];
	int count = 0;
	for (int i = 0; i < adj_vert_count; i++) {
		int adj_index = mesh->verts[vertex_id]->vertList[i];
		if (current_geodesic_dist >= average_geodesic_dist[adj_index]) {
			return false;
		}
		else {
			count++;
		}
	}


	if (count != 0 && count == adj_vert_count) {
		return true;
	}
	else return false;

}

void computeAGDForFarthestPoints(Mesh* mesh, float** geodesic_dist, vector<int> farthestPointIndices, int farthest_point_count) {
	
	float infinity = numeric_limits<float>::infinity();
	const int n = mesh->verts.size();


	for (int i = 0; i < farthest_point_count; i++) {

		vector<float> shortest_dist(n, infinity); //Create a vector for distances and initialize all distances as infinite (INF)
		vector<int> predecessor(n); //previous vertex array

		int start_vertex = farthestPointIndices[i];
		dijkstraFibonacciHeap(mesh, start_vertex, shortest_dist, predecessor, n);

		for (int j = 0; j < n; j++) {
			geodesic_dist[i][j] = shortest_dist[j];
		}

		shortest_dist.clear();
		predecessor.clear();
	
	}
	//cout << "\n";
}

void AGD(Mesh* mesh, vector<float>&average_geodesic_dist, float** geodesic_dist, int farthest_point_count) {

	const int n = mesh->verts.size();

	for (int i = 0; i < n; i++) {
		average_geodesic_dist[i] = 0.0;

		for (int j = 0; j < farthest_point_count; j++) {
			average_geodesic_dist[i] += geodesic_dist[j][i];	
		}
		average_geodesic_dist[i] = average_geodesic_dist[i] / farthest_point_count;
	}

}

void MGD(Mesh* mesh, vector<int> s1, vector<float>& minimal_geodesic_dist) {
	const int n = mesh->verts.size();
	float infinity = numeric_limits<float>::infinity();

	int s1_size = s1.size();
	float** geodesic_dist = new float*[s1_size];
	for (int i = 0; i < s1_size; i++)
		geodesic_dist[i] = new float[n];
	
	for (int i = 0; i < s1_size; i++) {
		float min_dist = 0.0;

		vector<float> shortest_dist(n, infinity); //Create a vector for distances and initialize all distances as infinite (INF)
		vector<int> predecessor(n); //previous vertex array

		dijkstraFibonacciHeap(mesh, s1[i], shortest_dist, predecessor, n);

		for (int j = 0; j < s1_size; j++) {
			geodesic_dist[i][j] = shortest_dist[s1[j]];	
		}

		shortest_dist.clear();
		predecessor.clear();
	}

	//cout << "MGD\n";
	for (int i = 0; i < s1_size; i++) {
		minimal_geodesic_dist[i] = minDistance(geodesic_dist[i], s1_size);
		//cout << minimal_geodesic_dist[i] << " ";
	}
	//cout << "\n";
	//deallocate the array
	for (int i = 0; i < s1_size; i++)
		delete[] geodesic_dist[i];
	delete[] geodesic_dist;
}

void MGDGeneral(Mesh* mesh, vector<int> s1, vector<float>& minimal_geodesic_dist) {
	const int n = mesh->verts.size();
	float infinity = numeric_limits<float>::infinity();

	int s1_size = s1.size();
	float** geodesic_dist = new float*[n];
	for (int i = 0; i < n; i++)
		geodesic_dist[i] = new float[s1_size];

	for (int i = 0; i < s1_size; i++) {
		float min_dist = 0.0;

		vector<float> shortest_dist(n, infinity); //Create a vector for distances and initialize all distances as infinite (INF)
		vector<int> predecessor(n); //previous vertex array

		dijkstraFibonacciHeap(mesh, s1[i], shortest_dist, predecessor, n);

		for (int j = 0; j < n; j++) {
			geodesic_dist[j][i] = shortest_dist[j];
		}

		shortest_dist.clear();
		predecessor.clear();
	}

	for (int i = 0; i < n; i++) {
		minimal_geodesic_dist[i] = minDistance(geodesic_dist[i], s1_size);
	}
	//deallocate the array
	for (int i = 0; i < n; i++)
		delete[] geodesic_dist[i];
	delete[] geodesic_dist;
}

int main(int, char ** argv)
{
	HWND window = SoWin::init(argv[0]);

	SoWinExaminerViewer * viewer = new SoWinExaminerViewer(window);

	//make a dead simple scene graph by using the Coin library, only containing a single cone under the scenegraph root
	SoSeparator * root = new SoSeparator;
	root->ref();

	Mesh* mesh = new Mesh();
	Painter* painter = new Painter();

	char* filename = "0.off";
	mesh->loadOff(filename);

	int n = mesh->verts.size();
	float infinity = numeric_limits<float>::infinity();
	mesh->fillAdjVertLengthList();
	
	/*******************FARTHEST POINT SAMPLING*******************/
	int max_vertex_index = maxVertexByAGD(mesh).first;
	//cout << "AGD maximizing vertex: " << max_vertex_index << "\n";
	int farthest_point_count = 100;
	vector<int> farthestPointIndices(farthest_point_count);

	getFPSPoints(mesh, max_vertex_index, n, farthestPointIndices, farthest_point_count);

	/****** VISUALIZATION**************************
	for (int i = 0; i < 10; i++) {
		//cout << farthestPointIndices[i] << " ";
		root->addChild(painter->getSpheresSeparatorRandomColor(mesh, farthestPointIndices[i]));
	}
	/***********************************************/
	//cout << "\n";


	/**************************************************************/

	int start_vertex = farthestPointIndices[0];
	clock_t start_time;
	double time;
	/******************* DIJKSTRA CALCULATION -REPORT ARRAY TIMING ****************
	vector<float> shortest_dist2(n, infinity); // Create a vector for distances and initialize all distances as infinite (INF)
	vector<int> predecessor2(n); //previous vertex array

	start_time = clock();

	dijkstraArray(mesh, start_vertex, shortest_dist2, predecessor2, n);
	time = clock() - start_time;

	std::cout << "Time difference of array implementation = " << time / (double)CLOCKS_PER_SEC << "[secs]" << std::endl;

	shortest_dist2.clear();
	predecessor2.clear();
	
	/******************* DIJKSTRA CALCULATION -REPORT MIN HEAP TIMING ****************/

	vector<float> shortest_dist(n, infinity); // Create a vector for distances and initialize all distances as infinite (INF)
	vector<int> predecessor(n); //previous vertex array

	start_time = clock();

	dijkstraMinHeap(mesh, start_vertex, shortest_dist, predecessor, n);
	time = clock() - start_time;

	std::cout << "Time difference of min-heap implementation = " << time / (double)CLOCKS_PER_SEC << "[secs]" << std::endl;

	shortest_dist.clear();
	predecessor.clear();
	/******************* DIJKSTRA CALCULATION -REPORT FIB HEAP TIMING ****************/
	
	vector<float> shortest_dist3(n, infinity); // Create a vector for distances and initialize all distances as infinite (INF)
	vector<int> predecessor3(n); //previous vertex array

	start_time = clock();
	dijkstraFibonacciHeap(mesh, start_vertex, shortest_dist3, predecessor3, n);
	time = clock() - start_time;

	std::cout << "Time difference of fibonacci-heap implementation = " << time / (double)CLOCKS_PER_SEC << "[secs]" << std::endl;
	
	/*****************************************************************************/

	/*********PRINT NXN GEODESIC DISTANCE MATRIX M**********************/
	//printDistanceArrayToFile(mesh); // print to the "geodesic-distance-matrix.txt" file
	/*******************************************************************/

	/**********VISUALIZE THE SHORTEST PATH BETWEEN TWO QUERY POINT**********/
	vector<int> shortest_path_edges;
	vector<int> shortest_path_vertices;

	int end_vertex = farthestPointIndices[1];

	findShortestPath(mesh, start_vertex, end_vertex, predecessor3, shortest_path_edges, shortest_path_vertices);

	vector<int>::iterator ptr;
	vector<int>::iterator ptr2;
	//cout << "The shortest path edge elements are : ";
	/***
	for (ptr = shortest_path_edges.begin(); ptr < shortest_path_edges.end(); ptr++) {
	//cout << *ptr << " ";
	root->addChild(painter->makeThickEdge(mesh, *ptr));
	}
	//cout << "\n";
	**/
	//root->addChild(painter->getSpheresSeparator(mesh, start_vertex)); // visualize start and end vertex of shortest path
	//root->addChild(painter->getSpheresSeparator(mesh, end_vertex));  
	
	/***********************************************************************/
	
	root->addChild(painter->getShapeSep(mesh, false));

	/*****************AGD FUNCTION AND SYMM INVARIANT SAMPLING***********/
	//allocate the array
	float** geodesic_dist = new float*[farthest_point_count];
	for (int i = 0; i < farthest_point_count; i++)
		geodesic_dist[i] = new float[n];
	
	computeAGDForFarthestPoints(mesh, geodesic_dist, farthestPointIndices, farthest_point_count);

	vector<float> avg_geodesic_dist(n);
	AGD(mesh, avg_geodesic_dist, geodesic_dist, farthest_point_count);

	//deallocate the array
	for (int i = 0; i < farthest_point_count; i++)
		delete[] geodesic_dist[i];
	delete[] geodesic_dist;

	averageOverLocalRing(mesh, n, avg_geodesic_dist);

	vector<int> s1; // AGD local maximas
	for (int v = 0; v < n; v++) {
		if (localMaxOneRingNeighbor(mesh, v, avg_geodesic_dist)) {
			s1.push_back(v);
			//root->addChild(painter->getSpheresSeparatorColored(mesh, v, 1.0f, 0.0f, 0.0f)); //visualize AGD local maximas
		}
	}
	
	/********************************************************************/
	bool agdVisualize = false;

	/*/***********************AGD FUNCTION VISUALIZE*****************************/
	if (agdVisualize) {
		float max_dist = maxDistanceAndIndex(avg_geodesic_dist, n).second;
		//cout << "Average Geodesic Function\n";
		for (int i = 0; i < n; i++) {

			avg_geodesic_dist[i] /= max_dist;
			float agd = avg_geodesic_dist[i];
			//cout << agd << " ";
			if (0.0 < agd && agd <= 0.2) {
				mesh->verts[i]->color = new Color(1.0f, 0.0f, 0.0f);
			}
			else if (0.2 < agd && agd <= 0.4) {
				mesh->verts[i]->color = new Color(1.0f, 1.0f, 0.0f);
			}
			else if (0.4 < agd && agd <= 0.6) {
				mesh->verts[i]->color = new Color(0.0f, 0.0f, 1.0f);
			}
			else if (0.6 < agd && agd < 0.7) {
				mesh->verts[i]->color = new Color(0.0f, 1.0f, 1.0f);
			}
			else {
				mesh->verts[i]->color = new Color(0.0f, 1.0f, 0.0f);
			}

			//cout << average_geodesic_dist[i] << " ";

		}
		//cout << "\n";
	}
	/******************************************************************/
	//root->addChild(painter->getShapeSep(mesh, agdVisualize));
	bool mgdVisualize = false;
	/***********************MGD VISUALIZE*****************************/
	if (mgdVisualize) {
		vector<float> minimal_geodesic_dist_general(n);
		MGDGeneral(mesh, s1, minimal_geodesic_dist_general);
		float max_dist = maxDistanceAndIndex(minimal_geodesic_dist_general, n).second;
		//cout << "Minimal Geodesic Function\n";
		for (int i = 0; i < n; i++) {

			minimal_geodesic_dist_general[i] /= max_dist;
			float mgd = minimal_geodesic_dist_general[i];
			//cout << mgd << " ";
			if (0.0 <  mgd &&  mgd <= 0.25) {
				mesh->verts[i]->color = new Color(0.0f, 0.0f, 1.0f);
			}
			else if (0.25 < mgd && mgd <= 0.6) {
				mesh->verts[i]->color = new Color(0.0f, 1.0f, 0.0f);
			}
			else if (0.6 < mgd &&  mgd <= 0.7) {
				mesh->verts[i]->color = new Color(1.0f, 0.0f, 0.0f);
			}
			else if (0.7 <  mgd &&  mgd< 0.8) {
				mesh->verts[i]->color = new Color(1.0f, 1.0f, 0.0f);
			}
			else {
				mesh->verts[i]->color = new Color(0.0f, 1.0f, 1.0f);
			}

			//cout << average_geodesic_dist[i] << " ";

		}
		//cout << "\n";
	}
	/******************************************************************/
	
	/**************************MGD CRITICAL POINTS**********************/
	vector<float> minimal_geodesic_dist(n);
	MGD(mesh, s1, minimal_geodesic_dist);

	vector<int> s2;
	for (int v = 0; v < n; v++) {
		if (localMaxOneRingNeighbor(mesh, v, minimal_geodesic_dist)) {
			s2.push_back(v);
			//root->addChild(painter->getSpheresSeparatorColored(mesh, s1[v], 1.0f, 0.0f, 0.0f)); //visualize MGD local maximas
		}
	}
	/*******************************************************************/

	viewer->setSize(SbVec2s(640, 480));
	viewer->setSceneGraph(root);
	viewer->show();

	SoWin::show(window);
	SoWin::mainLoop();
	delete viewer;
	root->unref();
	return 0;
}
