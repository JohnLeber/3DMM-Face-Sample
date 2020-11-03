# 3DMM-Face-Sample

**Introduction** 

This project was done to help me understand what 3DMM models are and how they are used with face reconstruction DNNs. 

Deep Neural Networks (DNNs) that take a 2D image of a human face as an input, and output a 3D mesh (e.g. Deep3DFaceReconstruction) often incorporate a 3D Morphable Face Model (3DMM) in their pipeline. One such example is Basel as used by Deep3DFaceReconstruction. When encountering such a DNN (e.g. here on github) for the first time, one might expect that the mesh, which consists of vertices and indices, is outputdirectly by the final layer of the DNN. This is not the case however, as the basel face mesh used in Deep3DFaceReconstruction has over 70,000 triangles. 

Each mesh is represented as a series of shape vectors:

<img src="https://render.githubusercontent.com/render/math?math={S^0,%20S^1%20%20%20...%20%20%20S^{m-1} }">

Each shape is represented as a column vector with n vertices:

<img src="https://render.githubusercontent.com/render/math?math={S^{i} =  \begin{bmatrix}   x^{i}_{0}   \\ \vdots  \\x_{n-1}\\y^{i}_{0}   \\ \vdots  \\y_{n-1}\\z^{i}_{0}   \\ \vdots  \\z_{n-1}  \end{bmatrix}  \in  \Re ^{3n}}">

where the mean/average emesh is defined as: 

<img src="https://render.githubusercontent.com/render/math?math={\overline{S} =   \frac{1}{m} \sum_{i=0}^{m-1} S^i %20}">

If we subtract the average from each mesh 

<img src="https://render.githubusercontent.com/render/math?math={ \bigtriangleup S^i=(S^i -\overline{S}) }">

New models are expressed as the mean plus a linear combination of the m shape vectors:

<img src="https://render.githubusercontent.com/render/math?math={  S^{model}  =\overline{S}%2B\sum_{i=0}^{m-1} \alpha^{i}  \bigtriangleup S^i}">

The dimensionality of the system is m but we can use Principle Component Analysis to determine a new basis vectors for the system such that the most dominant components are included first and the rest can be ignored. i.e.

<img src="https://render.githubusercontent.com/render/math?math={  S^{model}  =\overline{S}%2B\sum_{i=0}^{k-1} \alpha^{i}  \bigtriangleup E^i}">

k is the new dimension (hopefully significantly smaller than m) and E represents the Eigenvectors returned from the Singular Value Decomposition (WVD) method that we use to perform the Principle Component Analysis.



![alt text](https://github.com/nodecomplete/3DMM-Face-Sample/blob/master/FaceMorph/ScreenShot2.jpg)

[comment]: <> (<img src="https://render.githubusercontent.com/render/math?math={\bigtriangleup S=\begin{bmatrix} \vdots   \\ {\bigtriangleup S^0  \bigtriangleup S^1  \ldots  \bigtriangleup S^{m-1}} \\ {\vdots  } \\ \end{bmatrix}}">)
 
![alt text](https://github.com/nodecomplete/3DMM-Face-Sample/blob/master/FaceMorph/ScreenShot.jpg)


