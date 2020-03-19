/***************************************************************************
 * Copyright 1998-2020 by authors (see AUTHORS.txt)                        *
 *                                                                         *
 *   This file is part of LuxCoreRender.                                   *
 *                                                                         *
 * Licensed under the Apache License, Version 2.0 (the "License");         *
 * you may not use this file except in compliance with the License.        *
 * You may obtain a copy of the License at                                 *
 *                                                                         *
 *     http://www.apache.org/licenses/LICENSE-2.0                          *
 *                                                                         *
 * Unless required by applicable law or agreed to in writing, software     *
 * distributed under the License is distributed on an "AS IS" BASIS,       *
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*
 * See the License for the specific language governing permissions and     *
 * limitations under the License.                                          *
 ***************************************************************************/

#include <boost/format.hpp>

#include "luxrays/core/exttrianglemesh.h"
#include "slg/shapes/pointiness.h"
#include "slg/scene/scene.h"

using namespace std;
using namespace luxrays;
using namespace slg;

class Edge {
public:
	Edge(const u_int va, const u_int vb) : v0(va), v1(vb) { }
	~Edge() { }

	// Used by std::set
	bool operator<(const Edge &e) const {
		if (((e.v0 == v0) && (e.v1 == v1)) ||
				((e.v0 == v1) && (e.v1 == v0)))
			return false;
		else
			return this < &e;
	}

	const u_int v0, v1;
};

bool PointinessShape::IsSameVertex(const ExtTriangleMesh *srcMesh,
		const u_int vertex1Index, const u_int vertex2Index) const {
	if (DistanceSquared(srcMesh->GetVertex(Transform::TRANS_IDENTITY, vertex1Index),
			srcMesh->GetVertex(Transform::TRANS_IDENTITY, vertex2Index)) > DEFAULT_EPSILON_STATIC)
		return false;

	if (srcMesh->HasNormals() && (Dot(srcMesh->GetShadeNormal(Transform::TRANS_IDENTITY, vertex1Index),
			srcMesh->GetShadeNormal(Transform::TRANS_IDENTITY, vertex2Index)) < 1.f - DEFAULT_EPSILON_STATIC))
		return false;

	if (srcMesh->HasUVs(0) && (srcMesh->GetUV(vertex1Index, 0) != 
			srcMesh->GetUV(vertex2Index, 0)))
		return false;

	if (srcMesh->HasColors(0) && (srcMesh->GetColor(vertex1Index, 0) != 
			srcMesh->GetColor(vertex2Index, 0)))
		return false;

	if (srcMesh->HasAlphas(0) && (srcMesh->GetAlpha(vertex1Index, 0) != 
			srcMesh->GetAlpha(vertex2Index, 0)))
		return false;

	return true;
}

PointinessShape::PointinessShape(ExtTriangleMesh *srcMesh, const u_int destAOVIndex) {
	SDL_LOG("Pointiness shape " << srcMesh->GetName());

	const double startTime = WallClockTime();

	const u_int originalVertCount = srcMesh->GetTotalVertexCount();
	const u_int triCount = srcMesh->GetTotalTriangleCount();

	const Point *originalVertices = srcMesh->GetVertices();

	// Built a mapping to have all very near vertices 
	vector<u_int> uniqueVertices(originalVertCount);
	vector<bool> uniqueVerticesDone(originalVertCount, false);
	u_int uniqueVertCount = 0;
	for (u_int i = 0; i < originalVertCount; ++i) {
		if (uniqueVerticesDone[i])
			continue;

		// The current one is an unique vertex
		uniqueVertices[i] = i;
		++uniqueVertCount;

		#pragma omp parallel for
		for (
				// Visual C++ 2013 supports only OpenMP 2.5
#if _OPENMP >= 200805
				unsigned
#endif
				int j = i + 1; j < originalVertCount; ++j) {
			if (IsSameVertex(srcMesh, i, j)) {
				uniqueVertices[j] = i;
				uniqueVerticesDone[j] = true;
			}
		}
	}
	SDL_LOG("Pointiness shape has " << uniqueVertCount << " unique vertices over " << originalVertCount);

	const Triangle *tris = srcMesh->GetTriangles();

	// Build the edge information
	set<Edge> edges;
	for (u_int i = 0; i < triCount; ++i) {
		edges.insert(Edge(uniqueVertices[tris[i].v[0]], uniqueVertices[tris[i].v[1]]));
		edges.insert(Edge(uniqueVertices[tris[i].v[1]], uniqueVertices[tris[i].v[2]]));
		edges.insert(Edge(uniqueVertices[tris[i].v[2]], uniqueVertices[tris[i].v[0]]));
	}

	// Build the vertex information
	vector<Vector> vertexEdgeVecs(originalVertCount, Vector());
	vector<u_int> vertexCounters(originalVertCount, 0);
	BOOST_FOREACH(const Edge &e, edges) {
		const Vector ev = Normalize(originalVertices[e.v0] - originalVertices[e.v1]);

		vertexEdgeVecs[e.v0] += ev;
		vertexCounters[e.v0]++;

		vertexEdgeVecs[e.v1] -= ev;
		vertexCounters[e.v1]++;
	}

	// Build the normal information
	vector<Normal> vertexNormal(originalVertCount);
	if (srcMesh->HasNormals()) {
		for (u_int i = 0; i < originalVertCount; ++i) 
			vertexNormal[i] = srcMesh->GetShadeNormal(Transform::TRANS_IDENTITY, i);
	} else {
		for (u_int i = 0; i < triCount; ++i) {
			const Normal triNormal = srcMesh->GetGeometryNormal(Transform::TRANS_IDENTITY, i);

			vertexNormal[uniqueVertices[tris[i].v[0]]] += triNormal;
			vertexNormal[uniqueVertices[tris[i].v[1]]] += triNormal;
			vertexNormal[uniqueVertices[tris[i].v[2]]] += triNormal;
		}
		for (u_int i = 0; i < uniqueVertCount; ++i) {
			// Ignore not unique vertices
			if (uniqueVertices[i] != i)
				continue;

			vertexNormal[i] = Normalize(vertexNormal[i]);
		}
	}

	// Build the curvature information
	vector<float> rawCurvature(originalVertCount);
	for (u_int i = 0; i < originalVertCount; ++i) {
		if (vertexCounters[i] > 0)
			rawCurvature[i] = -Dot(vertexNormal[i], vertexEdgeVecs[i] / vertexCounters[i]);
		else
			rawCurvature[i] = 0.f;
	}

	// Blur the curvature information
	float *curvature = new float[originalVertCount];
	fill(curvature, curvature + originalVertCount, 0.f);
	fill(vertexCounters.begin(), vertexCounters.end(), 1);
	BOOST_FOREACH(const Edge &e, edges) {
		curvature[e.v0] += rawCurvature[e.v1];
		vertexCounters[e.v0]++;

		curvature[e.v1] += rawCurvature[e.v0];
		vertexCounters[e.v1]++;
	}

	// Normalize and factor also old vertex alphas
	for (u_int i = 0; i < originalVertCount; ++i) {
		if (uniqueVertices[i] == i) {
			// It is an unique vertex
			curvature[i] *= (srcMesh->HasAlphas(0) ? srcMesh->GetAlpha(i, 0) : 1.f) / vertexCounters[i];
		} else {
			// It is a duplicate, just copy the already compute curvature
			curvature[i] = curvature[uniqueVertices[i]];
		}
	}

	if (destAOVIndex == NULL_INDEX) {
		// Make a copy of the original mesh and overwrite vertex color informations
		mesh = srcMesh->Copy(NULL, NULL, NULL, NULL, NULL, curvature);
	} else {
		mesh = srcMesh->Copy();

		assert (destAOVIndex < EXTMESH_MAX_DATA_COUNT);
		mesh->SetVertexAOV(destAOVIndex, curvature);
	}

	const double endTime = WallClockTime();
	SDL_LOG("Pointiness time: " << (boost::format("%.3f") % (endTime - startTime)) << "secs");
}

PointinessShape::~PointinessShape() {
	if (!refined)
		delete mesh;
}

ExtTriangleMesh *PointinessShape::RefineImpl(const Scene *scene) {
	return mesh;
}
