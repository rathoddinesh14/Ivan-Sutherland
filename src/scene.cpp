#include "include/common/scene.h"
#include "include/common/camera.h"

Scene::Scene(Camera camera, Vector3f ambientLight):
                    camera(camera),
                    ambientLight(ambientLight){
	lights.push_back(new Light(Vector3f(1, 1, 1), Vector3f(0, 0, 0)));
}
void Scene::addObject(Intersectable*object){
	objects.push_back(object);
}
Hit Scene::intersect(Ray ray) {
	Hit bestHit;
	for (Intersectable* object : objects) {
		Hit hit = object->intersect(ray); //  hit.t < 0 if no intersection
		if (hit.t > 0 && (bestHit.t < 0 || hit.t < bestHit.t)){
			bestHit = hit;
		}
	}
	if ((ray.dir).Dot(bestHit.normal) > 0){
		bestHit.normal = bestHit.normal * (-1);
	}
	return bestHit;
}
Vector3f Scene::trace(Ray ray, int depth){
	if(depth > ITERATION_DEPTH){
		return ambientLight;
	}
	Hit hit = intersect(ray);
	if (hit.t < 0){
		return ambientLight;
	} else {
		return Vector3f(1, 1, 0);
	}

}
void Scene::render(std::vector<Vector4f>& image, int height, int width) {
	for (int Y = 0; Y < height; Y++) {
		for (int X = 0; X < width; X++) {
			camera.generateRay(X, Y);
			printf(".");
			// Vector3f color = trace(camera.generateRay(X, Y), 0);
			//image[Y * width + X] = Vector4f(color.x, color.y, color.z, 1);
		}
	}
}
