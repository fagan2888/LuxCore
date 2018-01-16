#include <string>
namespace slg { namespace ocl {
std::string KernelSource_texture_funcs = 
"#line 2 \"texture_funcs.cl\"\n"
"\n"
"/***************************************************************************\n"
" * Copyright 1998-2018 by authors (see AUTHORS.txt)                        *\n"
" *                                                                         *\n"
" *   This file is part of LuxCoreRender.                                   *\n"
" *                                                                         *\n"
" * Licensed under the Apache License, Version 2.0 (the \"License\");         *\n"
" * you may not use this file except in compliance with the License.        *\n"
" * You may obtain a copy of the License at                                 *\n"
" *                                                                         *\n"
" *     http://www.apache.org/licenses/LICENSE-2.0                          *\n"
" *                                                                         *\n"
" * Unless required by applicable law or agreed to in writing, software     *\n"
" * distributed under the License is distributed on an \"AS IS\" BASIS,       *\n"
" * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.*\n"
" * See the License for the specific language governing permissions and     *\n"
" * limitations under the License.                                          *\n"
" ***************************************************************************/\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// ConstFloat texture\n"
"//\n"
"// NOTE: the code for this type of texture is not dynamically generated in order\n"
"// to reduce the number of kernels compilations\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_CONST_FLOAT)\n"
"\n"
"float ConstFloatTexture_ConstEvaluateFloat(__global const Texture *tex) {\n"
"	return tex->constFloat.value;\n"
"}\n"
"\n"
"float3 ConstFloatTexture_ConstEvaluateSpectrum(__global const Texture *tex) {\n"
"	return tex->constFloat.value;\n"
"}\n"
"\n"
"// Note: ConstFloatTexture_Bump() is defined in texture_bump_funcs.cl\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// ConstFloat3 texture\n"
"//\n"
"// NOTE: the code for this type of texture is not dynamically generated in order\n"
"// to reduce the number of kernels compilations\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_CONST_FLOAT3)\n"
"\n"
"float ConstFloat3Texture_ConstEvaluateFloat(__global const Texture *tex) {\n"
"	return Spectrum_Y(VLOAD3F(tex->constFloat3.color.c));\n"
"}\n"
"\n"
"float3 ConstFloat3Texture_ConstEvaluateSpectrum(__global const Texture *tex) {\n"
"	return VLOAD3F(tex->constFloat3.color.c);\n"
"}\n"
"\n"
"// Note: ConstFloat3Texture_Bump() is defined in texture_bump_funcs.cl\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// ImageMap texture\n"
"//\n"
"// NOTE: the code for this type of texture is not dynamically generated in order\n"
"// to reduce the number of kernels compilations\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_IMAGEMAP) && defined(PARAM_HAS_IMAGEMAPS)\n"
"\n"
"float ImageMapTexture_ConstEvaluateFloat(__global const Texture *tex,\n"
"		__global HitPoint *hitPoint\n"
"		IMAGEMAPS_PARAM_DECL) {\n"
"	__global const ImageMap *imageMap = &imageMapDescs[tex->imageMapTex.imageMapIndex];\n"
"\n"
"	const float2 uv = VLOAD2F(&hitPoint->uv.u);\n"
"	const float2 mapUV = TextureMapping2D_Map(&tex->imageMapTex.mapping, hitPoint);\n"
"\n"
"	return tex->imageMapTex.gain * ImageMap_GetFloat(\n"
"			imageMap,\n"
"			mapUV.s0, mapUV.s1\n"
"			IMAGEMAPS_PARAM);\n"
"}\n"
"\n"
"float3 ImageMapTexture_ConstEvaluateSpectrum(__global const Texture *tex,\n"
"		__global HitPoint *hitPoint\n"
"		IMAGEMAPS_PARAM_DECL) {\n"
"	__global const ImageMap *imageMap = &imageMapDescs[tex->imageMapTex.imageMapIndex];\n"
"	__global const float *pixels = ImageMap_GetPixelsAddress(\n"
"			imageMapBuff, imageMap->pageIndex, imageMap->pixelsIndex);\n"
"\n"
"	const float2 uv = VLOAD2F(&hitPoint->uv.u);\n"
"	const float2 mapUV = TextureMapping2D_Map(&tex->imageMapTex.mapping, hitPoint);\n"
"\n"
"	return tex->imageMapTex.gain * ImageMap_GetSpectrum(\n"
"			imageMap,\n"
"			mapUV.s0, mapUV.s1\n"
"			IMAGEMAPS_PARAM);\n"
"}\n"
"\n"
"// Note: ImageMapTexture_Bump() is defined in texture_bump_funcs.cl\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Scale texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_SCALE)\n"
"\n"
"float ScaleTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const float tex1, const float tex2) {\n"
"	return tex1 * tex2;\n"
"}\n"
"\n"
"float3 ScaleTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float3 tex1, const float3 tex2) {\n"
"	return tex1 * tex2;\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// FresnelApproxN & FresnelApproxK texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_FRESNEL_APPROX_N)\n"
"\n"
"float FresnelApproxNTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const float value) {\n"
"	return FresnelApproxN(value);\n"
"}\n"
"\n"
"float3 FresnelApproxNTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float3 value) {\n"
"	return FresnelApproxN3(value);\n"
"}\n"
"\n"
"#endif\n"
"\n"
"#if defined(PARAM_ENABLE_FRESNEL_APPROX_K)\n"
"\n"
"float FresnelApproxKTexture_ConstEvaluateFloat( __global HitPoint *hitPoint,\n"
"		const float value) {\n"
"	return FresnelApproxK(value);\n"
"}\n"
"\n"
"float3 FresnelApproxKTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float3 value) {\n"
"	return FresnelApproxK3(value);\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Mix texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_MIX)\n"
"\n"
"float MixTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const float amt, const float value1, const float value2) {\n"
"	return mix(value1, value2, clamp(amt, 0.f, 1.f));\n"
"}\n"
"\n"
"float3 MixTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float3 amt, const float3 value1, const float3 value2) {\n"
"	return mix(value1, value2, clamp(amt, 0.f, 1.f));\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// CheckerBoard 2D & 3D texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_CHECKERBOARD2D)\n"
"\n"
"float CheckerBoard2DTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const float value1, const float value2, __global const TextureMapping2D *mapping) {\n"
"	const float2 uv = VLOAD2F(&hitPoint->uv.u);\n"
"	const float2 mapUV = TextureMapping2D_Map(mapping, hitPoint);\n"
"\n"
"	return ((Floor2Int(mapUV.s0) + Floor2Int(mapUV.s1)) % 2 == 0) ? value1 : value2;\n"
"}\n"
"\n"
"float3 CheckerBoard2DTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float3 value1, const float3 value2, __global const TextureMapping2D *mapping) {\n"
"	const float2 uv = VLOAD2F(&hitPoint->uv.u);\n"
"	const float2 mapUV = TextureMapping2D_Map(mapping, hitPoint);\n"
"\n"
"	return ((Floor2Int(mapUV.s0) + Floor2Int(mapUV.s1)) % 2 == 0) ? value1 : value2;\n"
"}\n"
"\n"
"#endif\n"
"\n"
"#if defined(PARAM_ENABLE_CHECKERBOARD3D)\n"
"\n"
"float CheckerBoard3DTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const float value1, const float value2, __global const TextureMapping3D *mapping) {\n"
"	// The +DEFAULT_EPSILON_STATIC is there as workaround for planes placed exactly on 0.0\n"
"	const float3 mapP = TextureMapping3D_Map(mapping, hitPoint) +  + DEFAULT_EPSILON_STATIC;\n"
"\n"
"	return ((Floor2Int(mapP.x) + Floor2Int(mapP.y) + Floor2Int(mapP.z)) % 2 == 0) ? value1 : value2;\n"
"}\n"
"\n"
"float3 CheckerBoard3DTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float3 value1, const float3 value2, __global const TextureMapping3D *mapping) {\n"
"	// The +DEFAULT_EPSILON_STATIC is there as workaround for planes placed exactly on 0.0\n"
"	const float3 mapP = TextureMapping3D_Map(mapping, hitPoint) +  + DEFAULT_EPSILON_STATIC;\n"
"\n"
"	return ((Floor2Int(mapP.x) + Floor2Int(mapP.y) + Floor2Int(mapP.z)) % 2 == 0) ? value1 : value2;\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Cloud texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_CLOUD_TEX)\n"
"\n"
"float CloudTexture_CloudNoise(const float3 p, const float omegaValue, const uint octaves) {\n"
"	// Compute sum of octaves of noise\n"
"	float sum = 0.f, lambda = 1.f, o = 1.f;\n"
"	for (uint i = 0; i < octaves; ++i) {\n"
"		sum += o * Noise3(lambda * p);\n"
"		lambda *= 1.99f;\n"
"		o *= omegaValue;\n"
"	}\n"
"	return sum;\n"
"}\n"
"\n"
"float CloudTexture_NoiseMask(const float3 p, const float radius, const float omega) {\n"
"	return CloudTexture_CloudNoise(p / radius * 1.4f, omega, 1);\n"
"}\n"
"\n"
"float3 CloudTexture_Turbulence(const float3 p, const float noiseScale, const float noiseOffset, const float variability, \n"
"                               const uint octaves, const float radius, const float omega, const float baseFlatness, const float3 sphereCentre) {\n"
"	float3 noiseCoords[3];	\n"
"	const float baseFadeDistance = 1.f - baseFlatness;\n"
"\n"
"	noiseCoords[0] = p / noiseScale;\n"
"	noiseCoords[1] = noiseCoords[0] + (float3)(noiseOffset, noiseOffset, noiseOffset);\n"
"	noiseCoords[2] = noiseCoords[1] + (float3)(noiseOffset, noiseOffset, noiseOffset);\n"
"\n"
"	float noiseAmount = 1.f;\n"
"\n"
"	if (variability < 1.f)	\n"
"		noiseAmount = Lerp(variability, 1.f, CloudTexture_NoiseMask(p + (float3)(noiseOffset * 4.f, 0.f, 0.f), radius, omega));\n"
"\n"
"\n"
"	noiseAmount = clamp(noiseAmount, 0.f, 1.f);\n"
"\n"
"	float3 turbulence;\n"
"\n"
"	turbulence.x = CloudTexture_CloudNoise(noiseCoords[0], omega, octaves) - 0.15f;\n"
"	turbulence.y = CloudTexture_CloudNoise(noiseCoords[1], omega, octaves) - 0.15f;\n"
"	turbulence.z = -CloudTexture_CloudNoise(noiseCoords[2], omega, octaves);\n"
"	if (p.z < sphereCentre.z + baseFadeDistance)\n"
"		turbulence.z *= (p.z - sphereCentre.z) / (2.f * baseFadeDistance);\n"
"\n"
"	turbulence *= noiseAmount;	\n"
"		\n"
"	return turbulence;\n"
"}\n"
"\n"
"float CloudTexture_CloudShape(const float3 p, const float baseFadeDistance, const float3 sphereCentre, const uint numSpheres, const float radius) {	\n"
"/*	if (numSpheres > 0) {\n"
"		if (SphereFunction(p, numSpheres))		//shows cumulus spheres\n"
"			return 1.f;\n"
"		else\n"
"			return 0.f;\n"
"	}\n"
"*/\n"
"	const float3 fromCentre = p - sphereCentre;\n"
"\n"
"	float amount = 1.f - sqrt(fromCentre.x*fromCentre.x + fromCentre.y*fromCentre.y + fromCentre.z*fromCentre.z) / radius;\n"
"	if (amount < 0.f)\n"
"		return 0.f;\n"
"\n"
"	// The base below the cloud's height fades out\n"
"	if (p.z < sphereCentre.z) {\n"
"		if (p.z < sphereCentre.z - radius * 0.4f)\n"
"			return 0.f;\n"
"\n"
"		amount *= 1.f - cos((fromCentre.z + baseFadeDistance) /\n"
"			baseFadeDistance * M_PI_F * 0.5f);\n"
"	}\n"
"	return max(amount, 0.f);\n"
"}\n"
"\n"
"/*\n"
"bool CloudTexture_SphereFunction(const float3 p, uint numSpheres) const {\n"
"// Returns whether a point is inside one of the cumulus spheres\n"
"	for (uint i = 0; i < numSpheres; ++i) {\n"
"		if ((p - spheres[i].position).Length() < spheres[i].radius)\n"
"			return true;\n"
"	}\n"
"	return false;\n"
"}\n"
"*/\n"
"\n"
"float CloudTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const float radius, const uint numSpheres, const uint spheresize, const float sharpness,\n"
"		const float baseFadeDistance, const float baseFlatness, const float variability,\n"
"		const float omega, const float firstNoiseScale, const float noiseOffset, const float turbulenceAmount,\n"
"		const uint numOctaves, __global const TextureMapping3D *mapping) {\n"
"\n"
"	const float3 mapP = TextureMapping3D_Map(mapping, hitPoint);\n"
"	const float3 sphereCentre = (float3)(.5f, .5f, 1.f / 3.f);\n"
"	const float amount = CloudTexture_CloudShape(mapP + turbulenceAmount * CloudTexture_Turbulence(mapP, firstNoiseScale, noiseOffset, variability, numOctaves, radius, omega, baseFlatness, sphereCentre), baseFadeDistance, sphereCentre, numSpheres, radius);\n"
"	const float finalValue = pow(amount * pow(10.f, .7f), sharpness);\n"
"\n"
"	return min(finalValue, 1.f);\n"
"}\n"
"\n"
"float3 CloudTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float radius, const uint numSpheres, const uint spheresize, const float sharpness,\n"
"		const float baseFadeDistance, const float baseFlatness, const float variability,\n"
"		const float omega, const float firstNoiseScale, const float noiseOffset, const float turbulenceAmount,\n"
"		const uint numOctaves, __global const TextureMapping3D *mapping) {\n"
"\n"
"	const float3 mapP = TextureMapping3D_Map(mapping, hitPoint);\n"
"	const float3 sphereCentre = (float3)(.5f, .5f, 1.f / 3.f);\n"
"	const float amount = CloudTexture_CloudShape(mapP + turbulenceAmount * CloudTexture_Turbulence(mapP, firstNoiseScale, noiseOffset, variability, numOctaves, radius, omega, baseFlatness, sphereCentre), baseFadeDistance, sphereCentre, numSpheres, radius);\n"
"	const float finalValue = pow(amount * pow(10.f, .7f), sharpness);\n"
"\n"
"	return min(finalValue, 1.f);\n"
"}\n"
"\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// FBM texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_FBM_TEX)\n"
"\n"
"float FBMTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"	const float omega, const int octaves, __global const TextureMapping3D *mapping) {\n"
"	const float3 mapP = TextureMapping3D_Map(mapping, hitPoint);\n"
"\n"
"	return FBm(mapP, omega, octaves);\n"
"}\n"
"\n"
"float3 FBMTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"	const float omega, const int octaves, __global const TextureMapping3D *mapping) {\n"
"	const float3 mapP = TextureMapping3D_Map(mapping, hitPoint);\n"
"\n"
"	return FBm(mapP, omega, octaves);\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Marble texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_MARBLE)\n"
"\n"
"// Evaluate marble spline at _t_\n"
"__constant float MarbleTexture_c[9][3] = {\n"
"	{ .58f, .58f, .6f},\n"
"	{ .58f, .58f, .6f},\n"
"	{ .58f, .58f, .6f},\n"
"	{ .5f, .5f, .5f},\n"
"	{ .6f, .59f, .58f},\n"
"	{ .58f, .58f, .6f},\n"
"	{ .58f, .58f, .6f},\n"
"	{.2f, .2f, .33f},\n"
"	{ .58f, .58f, .6f}\n"
"};\n"
"\n"
"float3 MarbleTexture_Evaluate(__global HitPoint *hitPoint, const float scale,\n"
"		const float omega, const int octaves, const float variation,\n"
"		__global const TextureMapping3D *mapping) {\n"
"	const float3 P = scale * TextureMapping3D_Map(mapping, hitPoint);\n"
"\n"
"	float marble = P.y + variation * FBm(P, omega, octaves);\n"
"	float t = .5f + .5f * sin(marble);\n"
"#define NC  sizeof(MarbleTexture_c) / sizeof(MarbleTexture_c[0])\n"
"#define NSEG (NC-3)\n"
"	const int first = Floor2Int(t * NSEG);\n"
"	t = (t * NSEG - first);\n"
"#undef NC\n"
"#undef NSEG\n"
"#define ASSIGN_CF3(a) (float3)(a[0], a[1], a[2])\n"
"	const float3 c0 = ASSIGN_CF3(MarbleTexture_c[first]);\n"
"	const float3 c1 = ASSIGN_CF3(MarbleTexture_c[first + 1]);\n"
"	const float3 c2 = ASSIGN_CF3(MarbleTexture_c[first + 2]);\n"
"	const float3 c3 = ASSIGN_CF3(MarbleTexture_c[first + 3]);\n"
"#undef ASSIGN_CF3\n"
"	// Bezier spline evaluated with de Castilejau's algorithm	\n"
"	float3 s0 = mix(c0, c1, t);\n"
"	float3 s1 = mix(c1, c2, t);\n"
"	float3 s2 = mix(c2, c3, t);\n"
"	s0 = mix(s0, s1, t);\n"
"	s1 = mix(s1, s2, t);\n"
"	// Extra scale of 1.5 to increase variation among colors\n"
"	return 1.5f * mix(s0, s1, t);\n"
"}\n"
"\n"
"float MarbleTexture_ConstEvaluateFloat(__global HitPoint *hitPoint, const float scale,\n"
"		const float omega, const int octaves, const float variation,\n"
"		__global const TextureMapping3D *mapping) {\n"
"	return Spectrum_Y(MarbleTexture_Evaluate(hitPoint, scale, omega, octaves,\n"
"			variation, mapping));\n"
"}\n"
"\n"
"float3 MarbleTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint, const float scale,\n"
"		const float omega, const int octaves, const float variation,\n"
"		__global const TextureMapping3D *mapping) {\n"
"	return MarbleTexture_Evaluate(hitPoint, scale, omega, octaves,\n"
"			variation, mapping);\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Dots texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_DOTS)\n"
"\n"
"bool DotsTexture_Evaluate(__global HitPoint *hitPoint, __global const TextureMapping2D *mapping) {\n"
"	const float2 uv = TextureMapping2D_Map(mapping, hitPoint);\n"
"\n"
"	const int sCell = Floor2Int(uv.s0 + .5f);\n"
"	const int tCell = Floor2Int(uv.s1 + .5f);\n"
"	// Return _insideDot_ result if point is inside dot\n"
"	if (Noise(sCell + .5f, tCell + .5f, .5f) > 0.f) {\n"
"		const float radius = .35f;\n"
"		const float maxShift = 0.5f - radius;\n"
"		const float sCenter = sCell + maxShift *\n"
"			Noise(sCell + 1.5f, tCell + 2.8f, .5f);\n"
"		const float tCenter = tCell + maxShift *\n"
"			Noise(sCell + 4.5f, tCell + 9.8f, .5f);\n"
"		const float ds = uv.s0 - sCenter, dt = uv.s1 - tCenter;\n"
"		if (ds * ds + dt * dt < radius * radius)\n"
"			return true;\n"
"	}\n"
"\n"
"	return false;\n"
"}\n"
"\n"
"float DotsTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const float value1, const float value2, __global const TextureMapping2D *mapping) {\n"
"	return DotsTexture_Evaluate(hitPoint, mapping) ? value1 : value2;\n"
"}\n"
"\n"
"float3 DotsTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float3 value1, const float3 value2, __global const TextureMapping2D *mapping) {\n"
"	return DotsTexture_Evaluate(hitPoint, mapping) ? value1 : value2;\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Brick texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_BRICK)\n"
"\n"
"bool BrickTexture_RunningAlternate(const float3 p, float3 *i, float3 *b,\n"
"		const float run, const float mortarwidth,\n"
"		const float mortarheight, const float mortardepth,\n"
"		int nWhole) {\n"
"	const float sub = nWhole + 0.5f;\n"
"	const float rsub = ceil(sub);\n"
"	(*i).z = floor(p.z);\n"
"	(*b).x = (p.x + (*i).z * run) / sub;\n"
"	(*b).y = (p.y + (*i).z * run) / sub;\n"
"	(*i).x = floor((*b).x);\n"
"	(*i).y = floor((*b).y);\n"
"	(*b).x = ((*b).x - (*i).x) * sub;\n"
"	(*b).y = ((*b).y - (*i).y) * sub;\n"
"	(*b).z = (p.z - (*i).z) * sub;\n"
"	(*i).x += floor((*b).x) / rsub;\n"
"	(*i).y += floor((*b).y) / rsub;\n"
"	(*b).x -= floor((*b).x);\n"
"	(*b).y -= floor((*b).y);\n"
"	return (*b).z > mortarheight && (*b).y > mortardepth &&\n"
"		(*b).x > mortarwidth;\n"
"}\n"
"\n"
"bool BrickTexture_Basket(const float3 p, float3 *i,\n"
"		const float mortarwidth, const float mortardepth,\n"
"		const float proportion, const float invproportion) {\n"
"	(*i).x = floor(p.x);\n"
"	(*i).y = floor(p.y);\n"
"	float bx = p.x - (*i).x;\n"
"	float by = p.y - (*i).y;\n"
"	(*i).x += (*i).y - 2.f * floor(0.5f * (*i).y);\n"
"	const bool split = ((*i).x - 2.f * floor(0.5f * (*i).x)) < 1.f;\n"
"	if (split) {\n"
"		bx = fmod(bx, invproportion);\n"
"		(*i).x = floor(proportion * p.x) * invproportion;\n"
"	} else {\n"
"		by = fmod(by, invproportion);\n"
"		(*i).y = floor(proportion * p.y) * invproportion;\n"
"	}\n"
"	return by > mortardepth && bx > mortarwidth;\n"
"}\n"
"\n"
"bool BrickTexture_Herringbone(const float3 p, float3 *i,\n"
"		const float mortarwidth, const float mortarheight,\n"
"		const float proportion, const float invproportion) {\n"
"	(*i).y = floor(proportion * p.y);\n"
"	const float px = p.x + (*i).y * invproportion;\n"
"	(*i).x = floor(px);\n"
"	float bx = 0.5f * px - floor(px * 0.5f);\n"
"	bx *= 2.f;\n"
"	float by = proportion * p.y - floor(proportion * p.y);\n"
"	by *= invproportion;\n"
"	if (bx > 1.f + invproportion) {\n"
"		bx = proportion * (bx - 1.f);\n"
"		(*i).y -= floor(bx - 1.f);\n"
"		bx -= floor(bx);\n"
"		bx *= invproportion;\n"
"		by = 1.f;\n"
"	} else if (bx > 1.f) {\n"
"		bx = proportion * (bx - 1.f);\n"
"		(*i).y -= floor(bx - 1.f);\n"
"		bx -= floor(bx);\n"
"		bx *= invproportion;\n"
"	}\n"
"	return by > mortarheight && bx > mortarwidth;\n"
"}\n"
"\n"
"bool BrickTexture_Running(const float3 p, float3 *i, float3 *b,\n"
"		const float run, const float mortarwidth,\n"
"		const float mortarheight, const float mortardepth) {\n"
"	(*i).z = floor(p.z);\n"
"	(*b).x = p.x + (*i).z * run;\n"
"	(*b).y = p.y - (*i).z * run;\n"
"	(*i).x = floor((*b).x);\n"
"	(*i).y = floor((*b).y);\n"
"	(*b).z = p.z - (*i).z;\n"
"	(*b).x -= (*i).x;\n"
"	(*b).y -= (*i).y;\n"
"	return (*b).z > mortarheight && (*b).y > mortardepth &&\n"
"		(*b).x > mortarwidth;\n"
"}\n"
"\n"
"bool BrickTexture_English(const float3 p, float3 *i, float3 *b,\n"
"		const float run, const float mortarwidth,\n"
"		const float mortarheight, const float mortardepth) {\n"
"	(*i).z = floor(p.z);\n"
"	(*b).x = p.x + (*i).z * run;\n"
"	(*b).y = p.y - (*i).z * run;\n"
"	(*i).x = floor((*b).x);\n"
"	(*i).y = floor((*b).y);\n"
"	(*b).z = p.z - (*i).z;\n"
"	const float divider = floor(fmod(fabs((*i).z), 2.f)) + 1.f;\n"
"	(*b).x = (divider * (*b).x - floor(divider * (*b).x)) / divider;\n"
"	(*b).y = (divider * (*b).y - floor(divider * (*b).y)) / divider;\n"
"	return (*b).z > mortarheight && (*b).y > mortardepth &&\n"
"		(*b).x > mortarwidth;\n"
"}\n"
"\n"
"bool BrickTexture_Evaluate(__global HitPoint *hitPoint,\n"
"		const MasonryBond bond,\n"
"		const float brickwidth, const float brickheight,\n"
"		const float brickdepth, const float mortarsize,\n"
"		const float3 offset,\n"
"		const float run, const float mortarwidth,\n"
"		const float mortarheight, const float mortardepth,\n"
"		const float proportion, const float invproportion,\n"
"		__global const TextureMapping3D *mapping) {\n"
"#define BRICK_EPSILON 1e-3f\n"
"	const float3 P = TextureMapping3D_Map(mapping, hitPoint);\n"
"\n"
"	const float offs = BRICK_EPSILON + mortarsize;\n"
"	float3 bP = P + (float3)(offs, offs, offs);\n"
"\n"
"	// Normalize coordinates according brick dimensions\n"
"	bP.x /= brickwidth;\n"
"	bP.y /= brickdepth;\n"
"	bP.z /= brickheight;\n"
"\n"
"	bP += offset;\n"
"\n"
"	float3 brickIndex;\n"
"	float3 bevel;\n"
"	bool b;\n"
"	switch (bond) {\n"
"		case FLEMISH:\n"
"			b = BrickTexture_RunningAlternate(bP, &brickIndex, &bevel,\n"
"					run , mortarwidth, mortarheight, mortardepth, 1);\n"
"			break;\n"
"		case RUNNING:\n"
"			b = BrickTexture_Running(bP, &brickIndex, &bevel,\n"
"					run, mortarwidth, mortarheight, mortardepth);\n"
"			break;\n"
"		case ENGLISH:\n"
"			b = BrickTexture_English(bP, &brickIndex, &bevel,\n"
"					run, mortarwidth, mortarheight, mortardepth);\n"
"			break;\n"
"		case HERRINGBONE:\n"
"			b = BrickTexture_Herringbone(bP, &brickIndex,\n"
"					mortarwidth, mortarheight, proportion, invproportion);\n"
"			break;\n"
"		case BASKET:\n"
"			b = BrickTexture_Basket(bP, &brickIndex,\n"
"					mortarwidth, mortardepth, proportion, invproportion);\n"
"			break;\n"
"		case KETTING:\n"
"			b = BrickTexture_RunningAlternate(bP, &brickIndex, &bevel,\n"
"					run, mortarwidth, mortarheight, mortardepth, 2);\n"
"			break; \n"
"		default:\n"
"			b = true;\n"
"			break;\n"
"	}\n"
"\n"
"	return b;\n"
"#undef BRICK_EPSILON\n"
"}\n"
"\n"
"float BrickTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const float value1, const float value2, const float value3,\n"
"		const MasonryBond bond,\n"
"		const float brickwidth, const float brickheight,\n"
"		const float brickdepth, const float mortarsize,\n"
"		const float3 offset,\n"
"		const float run, const float mortarwidth,\n"
"		const float mortarheight, const float mortardepth,\n"
"		const float proportion, const float invproportion,\n"
"		__global const TextureMapping3D *mapping) {\n"
"	return BrickTexture_Evaluate(hitPoint,\n"
"			bond,\n"
"			brickwidth, brickheight,\n"
"			brickdepth, mortarsize,\n"
"			offset,\n"
"			run, mortarwidth,\n"
"			mortarheight, mortardepth,\n"
"			proportion, invproportion,\n"
"			mapping) ? (value1 * value3) : value2;\n"
"}\n"
"\n"
"float3 BrickTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float3 value1, const float3 value2, const float3 value3,\n"
"		const MasonryBond bond,\n"
"		const float brickwidth, const float brickheight,\n"
"		const float brickdepth, const float mortarsize,\n"
"		const float3 offset,\n"
"		const float run, const float mortarwidth,\n"
"		const float mortarheight, const float mortardepth,\n"
"		const float proportion, const float invproportion,\n"
"		__global const TextureMapping3D *mapping) {\n"
"	return BrickTexture_Evaluate(hitPoint,\n"
"			bond,\n"
"			brickwidth, brickheight,\n"
"			brickdepth, mortarsize,\n"
"			offset,\n"
"			run , mortarwidth,\n"
"			mortarheight, mortardepth,\n"
"			proportion, invproportion,\n"
"			mapping) ? (value1 * value3) : value2;\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Add texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_ADD)\n"
"\n"
"float AddTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const float value1, const float value2) {\n"
"	return value1 + value2;\n"
"}\n"
"\n"
"float3 AddTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float3 value1, const float3 value2) {\n"
"	return value1 + value2;\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Subtract texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_SUBTRACT)\n"
"\n"
"float SubtractTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"const float value1, const float value2) {\n"
"	return value1 - value2;\n"
"}\n"
"\n"
"float3 SubtractTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"const float3 value1, const float3 value2) {\n"
"	return value1 - value2;\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Windy texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_WINDY)\n"
"\n"
"float WindyTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		__global const TextureMapping3D *mapping) {\n"
"	const float3 mapP = TextureMapping3D_Map(mapping, hitPoint);\n"
"\n"
"	const float windStrength = FBm(.1f * mapP, .5f, 3);\n"
"	const float waveHeight = FBm(mapP, .5f, 6);\n"
"\n"
"	return fabs(windStrength) * waveHeight;\n"
"}\n"
"\n"
"float3 WindyTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		__global const TextureMapping3D *mapping) {\n"
"	return WindyTexture_ConstEvaluateFloat(hitPoint, mapping);\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Wrinkled texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_WRINKLED)\n"
"\n"
"float WrinkledTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const float omega, const int octaves,\n"
"		__global const TextureMapping3D *mapping) {\n"
"	const float3 mapP = TextureMapping3D_Map(mapping, hitPoint);\n"
"\n"
"	return Turbulence(mapP, omega, octaves);\n"
"}\n"
"\n"
"float3 WrinkledTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const float omega, const int octaves,\n"
"		__global const TextureMapping3D *mapping) {\n"
"	return WrinkledTexture_ConstEvaluateFloat(hitPoint, omega, octaves, mapping);\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// UV texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_UV)\n"
"\n"
"float UVTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		__global const TextureMapping2D *mapping) {\n"
"	const float2 uv = TextureMapping2D_Map(mapping, hitPoint);\n"
"\n"
"	return Spectrum_Y((float3)(uv.s0 - Floor2Int(uv.s0), uv.s1 - Floor2Int(uv.s1), 0.f));\n"
"}\n"
"\n"
"float3 UVTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		__global const TextureMapping2D *mapping) {\n"
"	const float2 uv = TextureMapping2D_Map(mapping, hitPoint);\n"
"\n"
"	return (float3)(uv.s0 - Floor2Int(uv.s0), uv.s1 - Floor2Int(uv.s1), 0.f);\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// Band texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_BAND)\n"
"\n"
"float3 BandTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint,\n"
"		const InterpolationType interpType,\n"
"		const uint size, __global float *offsets,\n"
"		__global Spectrum *values, const float3 amt) {\n"
"	const float a = clamp(Spectrum_Y(amt), 0.f, 1.f);\n"
"\n"
"	const uint last = size - 1;\n"
"	if (a < offsets[0])\n"
"		return VLOAD3F(values[0].c);\n"
"	else if (a >= offsets[last])\n"
"		return VLOAD3F(values[last].c);\n"
"	else {\n"
"		int p = 0;\n"
"		for (; p <= last; ++p) {\n"
"			if (a < offsets[p])\n"
"				break;\n"
"		}\n"
"\n"
"		const float o1 = offsets[p - 1];\n"
"		const float o0 = offsets[p];\n"
"		const float factor = (a - o1) / (o0 - o1);\n"
"\n"
"		if (interpType == INTERP_NONE)\n"
"			return VLOAD3F(values[p - 1].c);\n"
"		else if (interpType == INTERP_LINEAR) {\n"
"			const float3 p0 = VLOAD3F(values[p - 1].c);\n"
"			const float3 p1 = VLOAD3F(values[p].c);\n"
"\n"
"			return Lerp3(factor, p0, p1);\n"
"		} else if (interpType == INTERP_CUBIC) {\n"
"			const float3 p0 = VLOAD3F(values[max(p - 2, 0)].c);\n"
"			const float3 p1 = VLOAD3F(values[p - 1].c);\n"
"			const float3 p2 = VLOAD3F(values[p].c);\n"
"			const float3 p3 = VLOAD3F(values[min(p + 1, (int)last)].c);\n"
"\n"
"			return Cerp3(factor, p0, p1, p2, p3);\n"
"		} else\n"
"			return 0.f;\n"
"	}\n"
"}\n"
"\n"
"float BandTexture_ConstEvaluateFloat(__global HitPoint *hitPoint,\n"
"		const InterpolationType interpType,\n"
"		const uint size, __global float *offsets,\n"
"		__global Spectrum *values, const float amt) {\n"
"	return Spectrum_Y(BandTexture_ConstEvaluateSpectrum(hitPoint,\n"
"			interpType, size, offsets, values, amt));\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// HitPointColor texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_HITPOINTCOLOR)\n"
"\n"
"float HitPointColorTexture_ConstEvaluateFloat(__global HitPoint *hitPoint) {\n"
"	return Spectrum_Y(VLOAD3F(hitPoint->color.c));\n"
"}\n"
"\n"
"float3 HitPointColorTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint) {\n"
"	return VLOAD3F(hitPoint->color.c);\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// HitPointAlpha texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_HITPOINTALPHA)\n"
"\n"
"float HitPointAlphaTexture_ConstEvaluateFloat(__global HitPoint *hitPoint) {\n"
"	return hitPoint->alpha;\n"
"}\n"
"\n"
"float3 HitPointAlphaTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint) {\n"
"	const float alpha = hitPoint->alpha;\n"
"	return (float3)(alpha, alpha, alpha);\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// HitPointGrey texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_HITPOINTGREY)\n"
"\n"
"float HitPointGreyTexture_ConstEvaluateFloat(__global HitPoint *hitPoint, const uint channel) {\n"
"	switch (channel) {\n"
"		case 0:\n"
"			return hitPoint->color.c[0];\n"
"		case 1:\n"
"			return hitPoint->color.c[1];\n"
"		case 2:\n"
"			return hitPoint->color.c[2];\n"
"		default:\n"
"			return Spectrum_Y(VLOAD3F(hitPoint->color.c));\n"
"	}\n"
"}\n"
"\n"
"float3 HitPointGreyTexture_ConstEvaluateSpectrum(__global HitPoint *hitPoint, const uint channel) {\n"
"	float v;\n"
"	switch (channel) {\n"
"		case 0:\n"
"			v = hitPoint->color.c[0];\n"
"			break;\n"
"		case 1:\n"
"			v = hitPoint->color.c[1];\n"
"			break;\n"
"		case 2:\n"
"			v = hitPoint->color.c[2];\n"
"			break;\n"
"		default:\n"
"			v = Spectrum_Y(VLOAD3F(hitPoint->color.c));\n"
"			break;\n"
"	}\n"
"\n"
"	return (float3)(v, v, v);\n"
"}\n"
"\n"
"#endif\n"
"\n"
"//------------------------------------------------------------------------------\n"
"// NormalMap texture\n"
"//------------------------------------------------------------------------------\n"
"\n"
"#if defined(PARAM_ENABLE_TEX_NORMALMAP)\n"
"\n"
"float NormalMapTexture_ConstEvaluateFloat(__global const Texture *tex) {\n"
"    return 0.f;\n"
"}\n"
"\n"
"float3 NormalMapTexture_ConstEvaluateSpectrum(__global const Texture *tex) {\n"
"	return 0.f;\n"
"}\n"
"\n"
"// Note: NormalMapTexture_Bump() is defined in texture_bump_funcs.cl\n"
"\n"
"#endif\n"
; } }
