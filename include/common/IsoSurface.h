#ifndef __ISOSURFACE_H
#define __ISOSURFACE_H

#pragma once
class IsoSurce : public Intersectable {
	Node*root;
public:
	IsoSurce(Node*_root, Material* _material) {
		root(_root);
		material(_material);
	}
	Hit intersect(const Ray& ray) {}
};
