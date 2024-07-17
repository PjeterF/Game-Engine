#include "SystemsManager.hpp"

SystemsManager& SystemsManager::getInstance()
{
	static SystemsManager instance;
	return instance;
}

SystemsManager::SystemsManager()
{

}
