# 3DMM-Face-Sample

**Introduction** 

Deep Neural Networks (DNN) that take a 2D image of a human face as an input, and output a 3D mesh (e.g. Deep3DFaceReconstruction) often incorporate a 3D Moprphable Face Model (3DMM) in their pipeline (Basel is one example). We attempt to demonstrate what a 3DMM is and how and why it is used in this pipeline.

When encountering a face reconstruction solution for the first time, one might expect that the mesh, which consists of vertices and face indices, is output by the DNN directly. This is not the case however. The basel face mesh used in Deep3DFaceRecosntruction has over 70,000 triangles. 

![alt text](https://github.com/nodecomplete/3DMM-Face-Sample/blob/master/FaceMorph/ScreenShot.jpg)

![alt text](https://github.com/nodecomplete/3DMM-Face-Sample/blob/master/FaceMorph/ScreenShot2.jpg)
