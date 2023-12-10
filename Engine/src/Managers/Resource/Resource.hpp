#pragma once

#include <string>

class ResourceBase
{
public:
	ResourceBase() {}
	virtual ~ResourceBase() {}
};

template <typename T>
class Resource
{
public:
	Resource(T* resource) : resource(resource) {}
	T* getContents() { return resource; }
	void unload();
	int getSubscriberCount() { return subscriberCount; }
	void subscribe() { subscriberCount++; }
	void unsubscribe() { subscriberCount--; }
private:
	T* resource;
	int subscriberCount = 0;
};

template<typename T>
inline void Resource<T>::unload()
{
	delete resource;
	resource = nullptr;
}