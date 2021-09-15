#include "Mesh.h"
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTransform.h>
#include <Inventor/nodes/SoSphere.h>
#include <Inventor/nodes/SoCoordinate3.h>
#include <Inventor/nodes/SoIndexedFaceSet.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoShapeHints.h>
#include <Inventor/nodes/SoDrawStyle.h>
#include <Inventor/nodes/SoIndexedLineSet.h>
//#include <Inventor/nodes/SoCone.h>




class Painter
{
public:
	SoSeparator* getShapeSep(Mesh* mesh, bool youWantToPaintEachVertexDifferently);
	SoSeparator* getSpheresSeparator(Mesh* mesh, int vertexIndex);
	SoSeparator* getSpheresSeparatorColored(Mesh* mesh, int vertexIndex, float r, float g, float b);
	SoSeparator* getSpheresSeparatorRandomColor(Mesh* mesh, int vertexIndex);
	SoSeparator* getSpheresSeparatorAGD(Mesh* mesh, int vertexIndex, float agd);
	SoSeparator* makeThickEdge(Mesh* mesh, int edgeIndex);
};
