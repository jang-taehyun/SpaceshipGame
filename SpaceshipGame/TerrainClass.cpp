#include "pch.h"
#include "QuadTreeClass.h"
#include "SkyDomeClass.h"
#include "TerrainClass.h"

bool Graphic::Terrain::TerrainClass::IsInitialize = false;

Graphic::Terrain::TerrainClass::TerrainClass()
{
	assert(!IsInitialize);
	IsInitialize = true;
}

Graphic::Terrain::TerrainClass::~TerrainClass()
{
	IsInitialize = false;
}
