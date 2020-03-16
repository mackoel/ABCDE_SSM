#pragma once
#include "pch.h"
class Model
{
public:
	Config config;
	virtual void act_with_config_file() = 0;
};