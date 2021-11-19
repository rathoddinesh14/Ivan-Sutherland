#ifndef __ISOSURFACE_H__
#define __ISOSURFACE_H__

#pragma once
class IsoSurface : public Intersectable {
	Node*root;
	Vector3f *vertices;
	int w;
	int h;
	float isoValue;
public:
	IsoSurface(Node*_root, Vector3f*_vertices, int _w, int _h, float _isoValue, Material* _material) {
		root = _root;
		vertices = _vertices;
		w = _w;
		h = _h;
		isoValue = _isoValue;
		material = _material;
	}
	float getA(Node*node,
			float u0b, float v0b, float w0b,
			float u1b, float v1b, float w1b,
			float iso_000, float iso_001,float iso_010, float iso_011,
			float iso_100, float iso_101,float iso_110, float iso_111){

		return u0b*v0b*w0b*iso_000 + u0b*v0b*w1b*iso_001 + u0b*v1b*w0b*iso_010 +
				u0b*v1b*w1b*iso_011 + u1b*v0b*w0b*iso_100 + u1b*v0b*w1b*iso_101 +
				u1b*v1b*w0b*iso_110 + u1b*v1b*w1b*iso_111;
	}
	float getB(Node*node,
			float u0a, float v0a, float w0a,
			float u0b, float v0b, float w0b,
			float u1a, float v1a, float w1a,
			float u1b, float v1b, float w1b,
			float iso_000, float iso_001,float iso_010, float iso_011,
			float iso_100, float iso_101,float iso_110, float iso_111){
		return (u0a*v0b*w0b + u0b*v0a*w0b + u0b*v0b*w0a)*iso_000 +
				(u0a*v0b*w1b + u0b*v0a*w1b + u0b*v0b*w1a)*iso_001 +
				(u0a*v1b*w0b + u0b*v1a*w0b + u0b*v1b*w0a)*iso_010 +
				(u0a*v1b*w1b + u0b*v1a*w1b + u0b*v1b*w1a)*iso_011 +
				(u1a*v0b*w0b + u1b*v0a*w0b + u1b*v0b*w0a)*iso_100 +
				(u1a*v0b*w1b + u1b*v0a*w1b + u1b*v0b*w1a)*iso_101 +
				(u1a*v1b*w0b + u1b*v1a*w0b + u1b*v1b*w0a)*iso_110 +
				(u1a*v1b*w1b + u1b*v1a*w1b + u1b*v1b*w1a)*iso_111;
	}
	float getC(Node*node,
			float u0a, float v0a, float w0a,
			float u0b, float v0b, float w0b,
			float u1a, float v1a, float w1a,
			float u1b, float v1b, float w1b,
			float iso_000, float iso_001,float iso_010, float iso_011,
			float iso_100, float iso_101,float iso_110, float iso_111){
		return (u0b*v0a*w0a + u0a*v0b*w0a + u0a*v0a*w0b)*iso_000 +
				(u0b*v0a*w1a + u0a*v0b*w1a + u0a*v0a*w1b)*iso_001 +
				(u0b*v1a*w0a + u0a*v1b*w0a + u0a*v1a*w0b)*iso_010 +
				(u0b*v1a*w1a + u0a*v1b*w1a + u0a*v1a*w1b)*iso_011 +
				(u1b*v0a*w0a + u1a*v0b*w0a + u1a*v0a*w0b)*iso_100 +
				(u1b*v0a*w1a + u1a*v0b*w1a + u1a*v0a*w1b)*iso_101 +
				(u1b*v1a*w0a + u1a*v1b*w0a + u1a*v1a*w0b)*iso_110 +
				(u1b*v1a*w1a + u1a*v1b*w1a + u1a*v1a*w1b)*iso_111;
	}
	float getD(Node*node,
			float u0a, float v0a, float w0a,
			float u1a, float v1a, float w1a,
			float iso_000, float iso_001,float iso_010, float iso_011,
			float iso_100, float iso_101,float iso_110, float iso_111, float iso){

		return u0a*v0a*w0a*iso_000 + u0a*v0a*w1a*iso_001 + u0a*v1a*w0a*iso_010 +
				u0a*v1a*w1a*iso_011 + u1a*v0a*w0a*iso_100 + u1a*v0a*w1a*iso_101 +
				u1a*v1a*w0a*iso_110 + u1a*v1a*w1a*iso_111 - iso;
	}
	Hit intersectVoxel(const Ray&ray, Node*node){
		Hit hit;

		/*
		 * Get isovalue of the eight vertices of the voxel here.
		 */
		 int p = w*h;
		float iso_000 = vertices[2 * node->index + 1].z;
		float iso_001 = vertices[2 * (node->index + 1) + 1].z;
		//float iso_101 = vertices[2 * (node->index + p + 1) + 1].z;
		float iso_010 = vertices[2 * (node->index + p + 1) + 1].z;
		//float iso_100 = vertices[2 * (node->index + p) + 1].z;
		float iso_011 = vertices[2 * (node->index + p) + 1].z;
		//float iso_011 = vertices[2 * (node->index + w) + 1].z;
		float iso_100 = vertices[2 * (node->index + w) + 1].z;
		//float iso_010 = vertices[2 * (node->index + w + 1) + 1].z;
		float iso_101 = vertices[2 * (node->index + w + 1) + 1].z;
		float iso_110 = vertices[2 * (node->index + p + w + 1) + 1].z;
		float iso_111 = vertices[2 * (node->index + p + w) + 1].z;

		float x0 = vertices[2 * node->index].x;
		float y0 = vertices[2 * node->index].y;
		float z0 = vertices[2 * node->index].z;

		float x1 = vertices[2 * (node->index + p + w + 1)].x;
		float y1 = vertices[2 * (node->index + p + w + 1)].y;
		float z1 = vertices[2 * (node->index + p + w + 1)].z;

		float xa = ray.start.x;
		float ya = ray.start.y;
		float za = ray.start.z;

		float xb = ray.dir.x;
		float yb = ray.dir.y;
		float zb = ray.dir.z;

		float u0a = (x1 - xa)/(x1 - x0);
		float v0a = (y1 - ya)/(y1 - y0);
		float w0a = (z1 - za)/(z1 - z0);

		float u0b = xb/(x1 - x0);
		float v0b = yb/(y1 - y0);
		float w0b = zb/(z1 - z0);

		float u1a = (xa - x0)/(x1 - x0);
		float v1a = (ya - y0)/(y1 - y0);
		float w1a = (za - z0)/(z1 - z0);

		float u1b = xb/(x0 - x1);
		float v1b = yb/(y0 - y1);
		float w1b = zb/(z0 - z1);

		double*c = (double*)malloc(sizeof(double)*4);
		
		c[0] = getA(node,
				u0b, v0b, w0b,
				u1b, v1b, w1b,
				iso_000, iso_001, iso_010, iso_011,
				iso_100, iso_101, iso_110, iso_111);
		c[1] = getB(node,
				u0a, v0a, w0a,
				u0b, v0b, w0b,
				u1a, v1a, w1a,
				u1b, v1b, w1b,
				iso_000, iso_001,iso_010, iso_011,
				iso_100, iso_101,iso_110, iso_111);
		c[2] = getC(node,
				u0a, v0a, w0a,
				u0b, v0b, w0b,
				u1a, v1a, w1a,
				u1b, v1b, w1b,
				iso_000, iso_001,iso_010, iso_011,
				iso_100, iso_101,iso_110, iso_111);
		c[3] = getD(node,
				u0a, v0a, w0a,
				u1a, v1a, w1a,
				iso_000, iso_001,iso_010, iso_011,
				iso_100, iso_101,iso_110, iso_111, isoValue);

		double*s = (double*)malloc(sizeof(double)*3);
		int num = SolveCubic(c, s);
		double closestIntersection = -1;
		for(int i = 0; i < num; i++){
			if(closestIntersection < 0 && s[i] > 0){
				closestIntersection = s[i];
			}
			if(s[i] > 0 && s[i] < closestIntersection){
				closestIntersection = s[i];
			}
		}
		if(closestIntersection < 0){
			/*
			 * No intersection
			 */
			return hit;
		} else {
			hit.t = closestIntersection;
			hit.position = ray.start + ray.dir * hit.t;
			/*
			 * Calculate the normal later
			 */
			//hit.normal = (hit.position - center) / radius;
			hit.material = material;
			return hit;
		}
	}
	Hit intersect(const Ray& ray) {
		Hit hit = intersectVoxel(ray, root);
		/*
		 * Later we will have to traverse the grid to find the actual intersection.
		 */
		return hit;
	}
};
#endif // __ISOSURFACE_H__
