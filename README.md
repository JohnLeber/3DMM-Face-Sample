# 3DMM-Face-Sample

**Introduction** 

This project was done to help me understand what 3DMM models are and how they are used with face reconstruction DNNs. 

Deep Neural Networks (DNNs) that take a 2D image of a human face as an input, and output a 3D mesh (e.g. Deep3DFaceReconstruction) often incorporate a 3D Morphable Face Model (3DMM) in their pipeline. One such example is Basel as used by Deep3DFaceReconstruction. When encountering such a DNN (e.g. here on github) for the first time, one might expect that the mesh, which consists of vertices and indices, is outputdirectly by the final layer of the DNN. This is not the case however, as the basel face mesh used in Deep3DFaceReconstruction has over 70,000 triangles. 

![alt text](https://github.com/nodecomplete/3DMM-Face-Sample/blob/master/FaceMorph/ScreenShot.jpg)

![alt text](https://github.com/nodecomplete/3DMM-Face-Sample/blob/master/FaceMorph/ScreenShot2.jpg)
