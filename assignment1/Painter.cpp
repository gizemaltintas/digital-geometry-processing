#include "Painter.h"

SoSeparator* Painter::getShapeSep(Mesh* mesh, bool youWantToPaintEachVertexDifferently)
{
	SoSeparator* res = new SoSeparator();

	//transformation
	//not needed

	//color
	SoMaterial* mat = new SoMaterial();
	//mat->diffuseColor.setValue(0, 1, 0); //paint all vertices with this color
	//mat->transparency = 0.5f : 0.0f; //0 makes it completely opaque, the default
	
	if (youWantToPaintEachVertexDifferently){
		for (int i = 0; i < (int)mesh->verts.size(); i++) { //i = 0 obj->color above overwritten here
			///cout << (mesh->verts[i]->color)->r << " " << (mesh->verts[i]->color)->g << " " << (mesh->verts[i]->color)->b << "\n";
			mat->diffuseColor.set1Value(i, (mesh->verts[i]->color)->r, (mesh->verts[i]->color)->g, (mesh->verts[i]->color)->b); //vert color according to its x-y-z coord (for mesh1) and to the transferred color (for mesh2)
		}
	}
	if (youWantToPaintEachVertexDifferently) res->addChild(mat);
	SoShapeHints* hints = new SoShapeHints;
	hints->creaseAngle = 3.14;
	res->addChild(hints); //Gouraud shading
	
	if (youWantToPaintEachVertexDifferently)
	{
		SoMaterialBinding* materialBinding = new SoMaterialBinding; //for 2+ diffuse color usage on the same mesh
		materialBinding->value = SoMaterialBinding::PER_VERTEX_INDEXED;
		res->addChild(materialBinding);
	}
	

	//shape
	SoCoordinate3* coords = new SoCoordinate3();
	for (unsigned int c = 0; c < mesh->verts.size(); c++)
		coords->point.set1Value(c, mesh->verts[c]->coords[0], mesh->verts[c]->coords[1], mesh->verts[c]->coords[2]);
	SoIndexedFaceSet* faceSet = new SoIndexedFaceSet();
	for (unsigned int c = 0; c < mesh->tris.size(); c++)
	{
		faceSet->coordIndex.set1Value(c*4, mesh->tris[c]->v1i);
		faceSet->coordIndex.set1Value(c*4 + 1, mesh->tris[c]->v2i);
		faceSet->coordIndex.set1Value(c*4 + 2, mesh->tris[c]->v3i);
		faceSet->coordIndex.set1Value(c*4 + 3, -1);

		
		if (youWantToPaintEachVertexDifferently)
		{
			faceSet->materialIndex.set1Value(0 + 4*c, mesh->tris[c]->v1i);
			faceSet->materialIndex.set1Value(1 + 4*c, mesh->tris[c]->v2i);
			faceSet->materialIndex.set1Value(2 + 4*c, mesh->tris[c]->v3i);
			faceSet->materialIndex.set1Value(3 + 4 *c, -1);
		}
		
	}
	res->addChild(coords);
	res->addChild(faceSet);

	return res;
}

SoSeparator* Painter::getSpheresSeparator(Mesh* mesh, int vertexIndex)
{
	//returns a set of spheres to highlight each mesh.samples[i]

	SoSeparator* spheresSep = new SoSeparator();

	float radius = 0.05f;//4.0->cat,woman,centaur 0.05->man3,horse2,neptune

	//1 sphere for this sample
	SoSeparator* sphere1Sep = new SoSeparator;

		
	//transformation
	SoTransform* tra = new SoTransform();
	tra->translation.setValue(mesh->verts[vertexIndex]->coords[0], mesh->verts[vertexIndex]->coords[1], mesh->verts[vertexIndex]->coords[2]);
	sphere1Sep->addChild(tra);
		

	//material
	SoMaterial* ma = new SoMaterial;
		
	ma->diffuseColor.setValue(SbColor(0.0f, 0.7f, 0.0f));
		

	sphere1Sep->addChild(ma);

		//shape
	SoSphere* sph1 = new SoSphere();
	sph1->radius = radius;
	sphere1Sep->addChild(sph1); //whose position is decided by the translation applied above

	spheresSep->addChild(sphere1Sep);
	

	return spheresSep;
}

SoSeparator* Painter::getSpheresSeparatorRandomColor(Mesh* mesh, int vertexIndex)
{
	//returns a set of spheres to highlight each mesh.samples[i]

	SoSeparator* spheresSep = new SoSeparator();

	float radius = 0.05f; //4.0->cat,woman,centaur 0.05->man3,horse2,neptune


	//1 sphere for this sample
	SoSeparator* sphere1Sep = new SoSeparator;


	//transformation
	SoTransform* tra = new SoTransform();
	tra->translation.setValue(mesh->verts[vertexIndex]->coords[0], mesh->verts[vertexIndex]->coords[1], mesh->verts[vertexIndex]->coords[2]);
	sphere1Sep->addChild(tra);


	//material
	SoMaterial* ma = new SoMaterial;

	float r = 0.1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - 0.1f)));
	float g = 0.1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - 0.1f)));
	float b = 0.1f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1.0f - 0.1f)));
	ma->diffuseColor.setValue(SbColor(r, g, b));


	sphere1Sep->addChild(ma);

	//shape
	SoSphere* sph1 = new SoSphere();
	sph1->radius = radius;
	sphere1Sep->addChild(sph1); //whose position is decided by the translation applied above

	spheresSep->addChild(sphere1Sep);


	return spheresSep;
}


SoSeparator* Painter::getSpheresSeparatorColored(Mesh* mesh, int vertexIndex, float r, float g, float b)
{
	SoSeparator* spheresSep = new SoSeparator();

	float radius = 0.05f; //4.0f   //0.05f->horse,man3 //2.0f->cat //2.5f->gorilla


	//1 sphere for this sample
	SoSeparator* sphere1Sep = new SoSeparator;


	//transformation
	SoTransform* tra = new SoTransform();
	tra->translation.setValue(mesh->verts[vertexIndex]->coords[0], mesh->verts[vertexIndex]->coords[1], mesh->verts[vertexIndex]->coords[2]);
	sphere1Sep->addChild(tra);


	//material
	SoMaterial* ma = new SoMaterial;

	ma->diffuseColor.setValue(SbColor(r, g, b));


	sphere1Sep->addChild(ma);

	//shape
	SoSphere* sph1 = new SoSphere();
	sph1->radius = radius;
	sphere1Sep->addChild(sph1); //whose position is decided by the translation applied above

	spheresSep->addChild(sphere1Sep);


	return spheresSep;
}

SoSeparator* Painter::getSpheresSeparatorAGD(Mesh* mesh, int vertexIndex, float agd)
{
	SoSeparator* spheresSep = new SoSeparator();

	float radius = 0.009f;
	float r, g, b;

	//1 sphere for this sample
	SoSeparator* sphere1Sep = new SoSeparator;


	//transformation
	SoTransform* tra = new SoTransform();
	tra->translation.setValue(mesh->verts[vertexIndex]->coords[0], mesh->verts[vertexIndex]->coords[1], mesh->verts[vertexIndex]->coords[2]);
	sphere1Sep->addChild(tra);

	if (0 < agd && agd < 0.2) {
		r = 1.0f;
		g = 0.0f;
		b = 0.0f;
	} 
	else if (0.2 < agd && agd < 0.4) {
		r = 1.0f;
		g = 1.0f;
		b = 0.0f;
	}
	else if (0.4 < agd && agd < 0.6) {
		r = 0.0f;
		g = 1.0f;
		b = 0.0f;
	}
	else if (0.6 < agd && agd < 0.8) {
		r = 0.0f;
		g = 1.0f;
		b = 1.0f;
	}
	else {
		r = 0.0f;
		g = 0.0f;
		b = 1.0f;
	}

	//material
	SoMaterial* ma = new SoMaterial;

	ma->diffuseColor.setValue(SbColor(r, g, b));


	sphere1Sep->addChild(ma);

	//shape
	SoSphere* sph1 = new SoSphere();
	sph1->radius = radius;
	sphere1Sep->addChild(sph1); //whose position is decided by the translation applied above

	spheresSep->addChild(sphere1Sep);


	return spheresSep;
}

SoSeparator* Painter::makeThickEdge(Mesh* mesh, int edgeIndex) {
	SoSeparator* thickEdgeSep = new SoSeparator;
	//material
	SoMaterial* ma = new SoMaterial;
	ma->diffuseColor.set1Value(0, 1.0f, 0.0f, 0.0f);
	thickEdgeSep->addChild(ma);
	SoDrawStyle* sty = new SoDrawStyle;	sty->lineWidth = 5.0f;	thickEdgeSep->addChild(sty);

	//shape
	SoIndexedLineSet* ils = new SoIndexedLineSet;
	SoCoordinate3* co = new SoCoordinate3;

	
	SbVec3f end1 = mesh->verts[mesh->edges[edgeIndex]->v1i]->coords ,
			end2 = mesh->verts[mesh->edges[edgeIndex]->v2i]->coords;
	co->point.set1Value(0, end1);
	co->point.set1Value(1, end2);
	

	
	ils->coordIndex.set1Value(0, 0);	ils->coordIndex.set1Value(1, 1);
	ils->coordIndex.set1Value(2, -1); //end this edge with -1
	
	thickEdgeSep->addChild(co);	thickEdgeSep->addChild(ils);

	return thickEdgeSep;
}

