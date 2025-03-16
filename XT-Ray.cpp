#include <iostream>
#include "glm/vec3.hpp"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "Ray.h"
#include "glm/geometric.hpp"
using namespace std;


double hit_sphere(const glm::vec3& center, double radius, const ray& r) {
	glm::vec3 oc = center - r.origin();
	auto a = dot(r.direction(), r.direction());
	auto b = -2.0 * dot(r.direction(), oc);
	auto c = dot(oc, oc) - radius * radius;
	auto discriminant = b * b - 4 * a * c;
	if (discriminant < 0) {
		return -1.0;
	}
	else {
		return (-b - std::sqrt(discriminant)) / (2.0 * a);
	}
}

glm::vec3 ray_color(const ray& r) {
	auto t = hit_sphere(glm::vec3(0, 0, -1), 0.5, r);
	if (t > 0.0) {
		glm::vec3 N = glm::normalize(r.at(t) - glm::vec3(0, 0, -1));
		return static_cast<float>(0.5) * glm::vec3(N.x + 1, N.y + 1, N.z + 1);
	}
	glm::vec3 unit_direction = glm::normalize(r.direction());
	auto a = 0.5 * (unit_direction.y + 1.0);
	return static_cast<float>((1.0 - a)) * glm::vec3(1.0, 1.0, 1.0) + static_cast<float>(a) *glm::vec3(0.5, 0.7, 1.0);
}


int main()
{
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;

	// Calculate the image height, and ensure that it's at least 1.
	int image_height = int(image_width / aspect_ratio);
	image_height = (image_height < 1) ? 1 : image_height;
	int n = 4;
	// Camera

	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(image_width) / image_height);
	auto camera_center = glm::vec3(0, 0, 0);

	// Calculate the vectors across the horizontal and down the vertical viewport edges.
	auto viewport_u = glm::vec3(viewport_width, 0, 0);
	auto viewport_v = glm::vec3(0, -viewport_height, 0);

	// Calculate the horizontal and vertical delta vectors from pixel to pixel.
	auto pixel_delta_u = viewport_u / static_cast<float>(image_width);
	auto pixel_delta_v = viewport_v / static_cast<float>(image_height);

	// Calculate the location of the upper left pixel.
	auto viewport_upper_left = camera_center
		- glm::vec3(0, 0, focal_length) - viewport_u / static_cast<float>(2) - viewport_v / static_cast<float>(2);
	auto pixel00_loc = viewport_upper_left + static_cast<float>(0.5) * (pixel_delta_u + pixel_delta_v);


	unsigned char* data = new unsigned char[image_width * image_height * n];

	for (int j = 0; j < image_height; j++)
	{
		for (int i = 0; i < image_width; i++)
		{
			auto pixel_center = pixel00_loc + (static_cast<float>(i) * pixel_delta_u) + (static_cast<float>(j) * pixel_delta_v);
			auto ray_direction = pixel_center - camera_center;
			ray r(camera_center, ray_direction);
			auto col =  ray_color(r);
			//glm::vec3 col(float(i) / float(image_width), float(j) / float(image_height), 0);
			data[j * image_width * n + i * n + 0] = int(255.99 * col.r);
			data[j * image_width * n + i * n + 1] = int(255.99 * col.g);
			data[j * image_width * n + i * n + 2] = int(255.99 * col.b);
			data[j * image_width * n + i * n + 3] = 255;
		}
	}

	cout << "write png to file!" << endl;
	stbi_write_png("cpt1_1.png", image_width, image_height, n, data, image_width * 4);
	stbi_image_free(data);
	return 0;
}
