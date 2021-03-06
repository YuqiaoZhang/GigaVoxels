///////////////////////////////////////////////////////////////////////////////////////////////////
// OpenGL Mathematics Copyright (c) 2005 - 2013 G-Truc Creation (www.g-truc.net)
///////////////////////////////////////////////////////////////////////////////////////////////////
// Created : 2011-05-25
// Updated : 2011-05-31
// Licence : This source is under MIT licence
// File    : test/gtx/quaternion.cpp
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <gsglm/glm.hpp>
#include <gsglm/gtc/epsilon.hpp>
#include <gsglm/gtx/quaternion.hpp>

int test_quat_fastMix()
{
	int Error = 0;

	glm::quat A = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));
	glm::quat B = glm::angleAxis(90.0f, glm::vec3(0, 0, 1));
	glm::quat C = glm::fastMix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(45.0f, glm::vec3(0, 0, 1));

	Error += glm::epsilonEqual(C.x, D.x, 0.01f) ? 0 : 1;
	Error += glm::epsilonEqual(C.y, D.y, 0.01f) ? 0 : 1;
	Error += glm::epsilonEqual(C.z, D.z, 0.01f) ? 0 : 1;
	Error += glm::epsilonEqual(C.w, D.w, 0.01f) ? 0 : 1;

	return Error;
}

int test_quat_shortMix()
{
	int Error(0);

	glm::quat A = glm::angleAxis(0.0f, glm::vec3(0, 0, 1));
	glm::quat B = glm::angleAxis(90.0f, glm::vec3(0, 0, 1));
	glm::quat C = glm::shortMix(A, B, 0.5f);
	glm::quat D = glm::angleAxis(45.0f, glm::vec3(0, 0, 1));

	Error += glm::epsilonEqual(C.x, D.x, 0.01f) ? 0 : 1;
	Error += glm::epsilonEqual(C.y, D.y, 0.01f) ? 0 : 1;
	Error += glm::epsilonEqual(C.z, D.z, 0.01f) ? 0 : 1;
	Error += glm::epsilonEqual(C.w, D.w, 0.01f) ? 0 : 1;

	return Error;
}

int test_orientation()
{
	int Error(0);

	{
		glm::quat q(1.0f, 0.0f, 0.0f, 1.0f);
		float p = glm::roll(q);
	}

	{
		glm::quat q(1.0f, 0.0f, 0.0f, 1.0f);
		float p = glm::pitch(q);
	}

	{
		glm::quat q(1.0f, 0.0f, 0.0f, 1.0f);
		float p = glm::yaw(q);
	}

	return Error;
}

int main()
{
	int Error = 0;

	Error += test_quat_fastMix();
	Error += test_quat_shortMix();

	return Error;
}
