#if ! defined(__DISPLAY_H__)
#define __DISPLAY_H__

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct DisplayData
{
	int width;
	int height;
	float fieldOfView;
	float aspectRatio;
	float nearPlane;
	float farPlane;

	glm::vec3 cameraPos;
	glm::vec3 lookAt;
	glm::vec3 up;

	glm::mat4 viewProjection;

	DisplayData()
		: width(800), height(600),
			fieldOfView(45.0f),
			aspectRatio((float)width/(float)height),
			nearPlane(0.1f), farPlane(100.0f),
			cameraPos(0), lookAt(0), up(0,1,0)
	{
	}

	void Update()
	{
		glm::mat4 view = glm::lookAt(cameraPos, lookAt, up);
		glm::mat4 projection = glm::perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
		viewProjection = projection * view;
	}
};

extern DisplayData display;

#endif // __DISPLAY_H__
