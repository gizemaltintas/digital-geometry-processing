# digital-geometry-processing
Assignments of CENG789 Master's course on Digital Geometry Processing

The base code and detailed course catalog can be found in https://user.ceng.metu.edu.tr/~ys/ceng789-dgp/. The code can be executed by running .sln file. I have used
Coin3D Open Inventor for 3D SDK which is built on top of OpenGL.

## Assignment-1
In this assignment, I have implemented following three parts:

- ### Geodesics on Meshes by applying Dijkstra algorithm

 ![alt text](assignment1/Results/1-geodesic-on-meshes/shortesth-path-man4.PNG?raw=true)
 ![alt text](assignment1/Results/1-geodesic-on-meshes/shortesth-path2-centaur.PNG)

- ### Farthest Point Sampling algorithm

  For the meshes that have huge vertex size, the computation of AGD function by calculating the N-size geodesic distance 
  array for each vertex was really costly. It was a nice and efficient solution to take 100 Farthest Point samples to 
  find the other's geodesic distances. I have observed that Farthest Point Samples was crucial to have an accurate 
  and average results without any extra cost. 

  After applying this, also visualizing the resulting AGD function for the horse 
  input was interesting for me. At first, I got a few more maxima points but I have solved it by averaging over local 1-rings.
  I got a closer and cool image similar to the figure in the paper.On the other hand, for man and gorilla inputs, 
  the output was more remarkable (especially in the details like finger tips , eyes or nose etc.)

  I have tried with different numbers of farthest point samples to calculate agd; however, 
  I couldn't realize big differences between 100 or 200 points. The visualization was almost same.
  
   ![alt text](assignment1/Results/2-farthest-point-sampling/FPS-cat.PNG)

- ### Symmetry-Invariant Sampling 

  I have implemented Section 4 of the paper titled Mobius Transformations For Global Intrinsic Symmetry Analysis

  AGD-function : 
  
  ![alt text](assignment1/Results/3-symmetry-invariant-sampling/AGD-function-horse1.PNG)
  
  AGD local-maxima points : 
  
  ![alt text](assignment1/Results/3-symmetry-invariant-sampling/AGD-local-maxima-centaur.PNG)
  
  MGD-function : 
  
  ![alt text](assignment1/Results/3-symmetry-invariant-sampling/MGD-function-horse1.PNG)

  I have put effort to extract local maximas of a subset S2 based on Minimal Geodesic Distance(MGD) 
  from the set of S1 which consists of the local maximas of AGD function. The resulting set S2 was also a
  symmetry-invariant function. This has enlightened me to get an idea of how closely related geodesic distance concept and 
  selecting a set of critical points of a mesh are.
  
 ## Assignment-2
  
 The resulting files and screenshots can be found on the "\Results" directory. There are two subdirectories which are 
 A-Star-Search and Subdivision corresponding to each of the subsection in the assignment.

 The code can be executed by running .sln file. The executable is placed \Debug\Assignment2.exe.
 While trying you can put or delete the slash"\" on the end of 987th line 
 for A*star search and 1061th line for subdivision algorithms.

 - ### A* search:
   
  1-) For A* search algorithm, I have realized a removing operation for the priority queue was needed since 
  when visiting already visited vertex, we should remove the old key and replace with new key if its f(n) = g(n)+h(n)
  value is less than the cost itself. So, I have implemented this remove functionality although it is not provided 
  by STL library default and used it.

  2-) I have implemented 3 different heuristics, and they are the following: Eucledian distance, Manhattan and 
  Diagonal Distance. In comparison to Dijkstra algo., by putting the Eucledian distance, the visited nodes decreased 
  sufficiently; however, I have observed that when applying a factor to it by multiplying the value with let's say 3, 
  the visited nodes are decreased more significantly. For Manhattan and Diagonal Distances(indeed octile distance), 
  I have realized it gives a similar result without a factor.

  3-)  I have put my outputs for time differences and shortest path length to comparison.txt under the 
  Results/A-Star-Search folder. For execution times, the A* is almost always much faster than Dijkstra algorithm. 
  On the other hand, path length of A* is may not be the shortest/most-optimal one, which is a trade for the speed.
  The visited vertex count of Dijkstra algorithm is 14021 while A* algo.'s best count is 234. Among the heuristics, 
  I suppose there are no huge change among heuristics, and as I have mentioned above applying factor decreases the number 
  of visited nodes.

  4-) For the below screenshot image, the red line shows the A* search path and its visited nodes are 
  highlighted with red color also. The blue line depicts the shortest path with Dijkstra.

  ![centaur-a-star-search](https://user-images.githubusercontent.com/69903875/133669729-d12c6ca3-dd20-4b3d-86b0-6b5195616fe0.PNG)
  
   For the other screenshot in the below, the blue line shows the A* search path and its visited nodes are highlighted with magenta color. 
  The black line depicts the shortest path with Dijkstra. All other images are compatible with file name.

  ![gorilla-a-star-search](https://user-images.githubusercontent.com/69903875/133669866-c553c8ff-4451-4aaf-aed6-4d0f8ca4c693.PNG)


 - ### Subdivision Surfaces:

  I have put my outputs for the number of triangles and total surface areas to comparison.txt under the 
  Results/Subdivision folder. Total number of triangles have increased 3-4 times by the original one. 
  The number is higher for the 4-to-1 Subdivision. Total surface area seems not changed much for 4-to-1 Subdivision 
  but for the Sqrt3 subdivision's output, it is decreased a little.

  For the painted images, I have followed the way of mapping distances to 0-2. While going from 0->1, the color changes from red to 
  green and 1->2 the color changes from green to magenta (actually I have used blue but then it cannot be seen obviously, 
  so I have changed it).

   - #### Square-Root-3-Subdivision:
   
   1-) While adding new midpoint vertices/triangles, updating the vertex's adjacent vertices or triangles is where 
   I have faced difficulties. Lazy deleting saved my life ,i.e, marking original triangles' removed flag as true. 

   - #### 4-to-1 Subdivision:
   
   1-) After adding the all necessary edges to the flip set F, I have observed that the size of F is equals to the 
    triangle size. I have reasoned it as follows: Connecting the end-vertex with the opposite edge's middle point creates
    2 new triangles but these two triangles' one of the vertices will not be an original point and for the remaining 
    2 edges of our old triangle we can only add a new edge from these already split-vertex (middle) not from original vertices.
    So each triangle have 1 flipped edges as far as I observed at the end.

   2-) Because we are not changing the coordinates of existing vertices, in the painted form of 4-to-1 algo.
    we can see more red points than the output of SquareRoot3 algo. indicating the their distances to the closest points
    in the base input mesh is closer to 0.0. 
   
   ![comp-subdi](https://user-images.githubusercontent.com/69903875/133671994-71599a48-f30e-4b59-9a56-60c9cb4dbb52.png)


  
