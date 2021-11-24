#include "include/raytracing/scene.h"

Scene::Scene(Vector3f ambientLight, int width, int height, VolumeRender *vr) : 
camera(camera), ambientLight(ambientLight), width(width), height(height), volumeRenderer(vr)
{
	camera = new RayCamera();
	Vector3f eye = Vector3f(0, 0, 20), vup = Vector3f(0, 1, 0), lookat = Vector3f(0, 0, 0);
	float fov = 45 * M_PI / 180;
	camera->set(eye, lookat, vup, fov, width, height);

	lights.push_back(new Light(Vector3f(1, 1, 1), ambientLight));

	Vector3f ks(2, 2, 2);

	isoValue = 6;

	for (auto node : volumeRenderer->getDomainSearch()->getNodes(isoValue))
	{
		node->isoValue = isoValue;
		objects.push_back(node);
	}
	printf("%d nodes\n", objects.size());

	objects.push_back(new Sphere(Vector3f(-0.55, 0, 0), 0.5,
								 new RoughMaterial(Vector3f(0.3, 0.2, 0.1), ks, 50)));
	// objects.push_back(new Sphere(Vector3f(0.55, 0, 0), 0.5,
	// 							 new RoughMaterial(Vector3f(0.1, 0.2, 0.3), ks, 100)));
	// objects.push_back(new Sphere(Vector3f(0, 0.5, -0.8), 0.5,
	// 							 new RoughMaterial(Vector3f(0.3, 0, 0.2), ks, 20)));
	// objects.push_back(new Sphere(Vector3f(0, 0.3, 0.6), 0.5,
	// 							 new RefractiveMaterial(Vector3f(1.2, 1.2, 1.2))));
	// objects.push_back(new Sphere(Vector3f(0, -6.5, 0), 6,
	//  new ReflectiveMaterial(Vector3f(0.14, 0.16, 0.13), Vector3f(4.1, 2.3, 3.1))));
	printf("Scene created\n");
}

Hit Scene::intersect(Ray ray)
{
	Hit bestHit;
	for (Intersectable *object : objects)
	{
		Hit hit = object->intersect(ray); //  hit.t < 0 if no intersection
		if (hit.t > 0 && (bestHit.t < 0 || hit.t < bestHit.t))
		{
			bestHit = hit;
		}
	}
	if ((ray.dir).Dot(bestHit.normal) > 0)
	{
		bestHit.normal = bestHit.normal * (-1);
	}
	return bestHit;
}

bool Scene::shadowIntersect(Ray ray)
{ // for directional lights
	for (Intersectable *object : objects)
		if (object->intersect(ray).t > 0)
			return true;
	return false;
}

Vector3f Scene::trace(Ray ray, int depth = 0)
{
	if (depth > ITERATION_DEPTH)
	{
		return ambientLight;
	}

	printf("======================================\n");
	ray.start.Print();
	ray.dir.Print();

	Hit hit = intersect(ray);

	printf("%f\n", hit.t);
	printf("======================================\n");

	if (hit.t < 0)
	{
		return ambientLight;
	}

	if (hit.material->type == ROUGH)
	{
		Vector3f outRadiance = hit.material->ka * ambientLight;
		for (Light *light : lights)
		{
			Ray shadowRay(hit.position + hit.normal * epsilon, light->direction);
			float cosTheta = hit.normal.Dot(light->direction);
			if (cosTheta > 0 && !shadowIntersect(shadowRay))
			{ // shadow computation
				outRadiance = outRadiance + light->emittence * hit.material->kd * cosTheta;
				Vector3f halfway = (light->direction - ray.dir).Normalize();
				float cosDelta = hit.normal.Dot(halfway);
				if (cosDelta > 0)
					outRadiance = outRadiance + light->emittence * hit.material->ks * powf(cosDelta, hit.material->shininess);
			}
		}
		return outRadiance;
	}

	float cosa = -ray.dir.Dot(hit.normal);
	Vector3f one(1, 1, 1);
	Vector3f F = hit.material->F0 + (one - hit.material->F0) * pow(1 - cosa, 5);
	Vector3f reflectedDir = ray.dir - hit.normal * hit.normal.Dot(ray.dir) * 2.0f;
	Vector3f outRadiance = trace(Ray(hit.position + hit.normal * epsilon, reflectedDir), depth + 1) * F;

	if (hit.material->type == REFRACTIVE)
	{
		float disc = 1 - (1 - cosa * cosa) / hit.material->ior / hit.material->ior; // scalar n
		if (disc >= 0)
		{
			Vector3f refractedDir = ray.dir / hit.material->ior + hit.normal * (cosa / hit.material->ior - sqrt(disc));
			outRadiance = outRadiance +
						  trace(Ray(hit.position - hit.normal * epsilon, refractedDir), depth + 1) * (one - F);
		}
	}
	return outRadiance;
}

void Scene::render(std::vector<Vector4f> &image)
{
	for (int Y = 0; Y < height; Y++)
	{
		for (int X = 0; X < width; X++)
		{
			printf("%d %d\n", X, Y);
			Vector3f color = trace(camera->getRay(X, Y));
			image[Y * width + X] = Vector4f(color.x, color.y, color.z, 1);
			// printf("%d %d\n", X, Y);
		}
		// printf("\rRendering %d%%", 100 * Y / height);
	}
}

void Scene::Animate(float dt)
{
	camera->Animate(dt);
}

void Scene::setIsoValue(float iso)
{
	isoValue = iso;
}

float Scene::getIsoValue()
{
	return isoValue;
}