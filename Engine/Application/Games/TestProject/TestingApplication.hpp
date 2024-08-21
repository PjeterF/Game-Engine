#pragma once

#include "../../Application.hpp"

class TestingApplicaton : public Application
{
public:
	TestingApplicaton(float windowWidth, float windowHeight, std::string windowName);
	virtual void initialize();
};