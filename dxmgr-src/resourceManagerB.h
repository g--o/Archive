/*
 ResourceManagerB.hpp - Generic template resource manager				
									
 (C) Alexander Thorne (SFML Coder) 2011	
 <a href="http://sfmlcoder.wordpress.com/">http://sfmlcoder.wordpress.com/</a>	
		
 Manages loading and unloading of a resource type specified by a
 template argument.

****************************************************************/
#include <map>
#include <string>
#include <exception>

typedef const std::string URI;



// exceptions
namespace Exceptions {

	// thrown if user requests a resource URI not present in the manager's list
	class URINotFound : public std::runtime_error 
	{ 
	public: 
		URINotFound(const std::string& Message = "The specified URI was not found in the resource index.")
			: runtime_error(Message) { } 
	};

	// thrown if a resource allocation fails
	class BadResourceAllocation : public std::runtime_error {
	public: 
		BadResourceAllocation(const std::string& Message = "Failed to allocate memory for resource.")
			: runtime_error(Message) {}
	};
}

template <class Resource> class ResourceManagerB {
	typedef std::pair<URI, Resource*> ResourcePair;
	typedef std::map<URI, Resource*> ResourceList;

	// the list of the manager's resources
	ResourceList Resources;
public:
	~ResourceManagerB() { UnloadAll(); }

	// Load a resource with the specified URI
	// the URI could represent, e.g, a filename
	URI& Load(URI& Uri);
	URI& Load(URI& Uri,Resource* resource);
	// unload a resource with the specified URI
	void Unload(URI& Uri);
	// unload all resources
	void UnloadAll();

	// get a pointer to a resource
	Resource* GetPtr(URI& Uri);
	// get a reference to a resource
	Resource& Get(URI& Uri);
};

template <class Resource>
URI& ResourceManagerB<Resource>::Load(URI& Uri)
{
	// check if resource URI is already in list
	// and if it is, we do no more
	if (Resources.find(Uri) == Resources.end())
	{
		// try to allocate the resource
		// NB: if the Resource template argument does not have a
		// constructor accepting a const std::std::string, then this
		// line will cause a compiler error
		Resource* temp = new (std::nothrow) Resource(Uri);
		// check if the resource failed to be allocated
		// std::nothrow means that if allocation failed
		// temp will be 0
		if (!temp)
			throw Exceptions::BadResourceAllocation();
		// add the resource and it's URI to the manager's list
		Resources.insert(ResourcePair(Uri, temp));
	}
	return Uri;
}

template <class Resource>
URI& ResourceManagerB<Resource>::Load(URI& Uri,Resource* resource)
{
	if (Resources.find(Uri) == Resources.end())
	{
		if (!resource)
			throw Exceptions::BadResourceAllocation();
		// add the resource and it's URI to the manager's list
		Resources.insert(ResourcePair(Uri, resource));
	}
	return Uri;
}

template <class Resource>
void ResourceManagerB<Resource>::Unload(URI& Uri)
{
	// try to find the specified URI in the list
	ResourceList::const_iterator itr = Resources.find(Uri);
	// if it is found...
	if (itr != Resources.end())
	{
		// ... deallocate it
		delete itr->second;
		// then remove it from the list
		Resources.erase(Uri);
	}
}

template <class Resource>
void ResourceManagerB<Resource>::UnloadAll()
{
	// iterate through every element of the resource list
	ResourceList::iterator itr;
	for (itr = Resources.begin(); itr != Resources.end(); itr++)
		// delete each resource
		delete itr->second;
	// finally, clear the list
	Resources.clear();
}

template <class Resource>
Resource* ResourceManagerB<Resource>::GetPtr(URI& Uri)
{
	// find the specified URI in the list
	ResourceList::const_iterator itr;
	// if it is there...
	if ((itr = Resources.find(Uri)) != Resources.end())
		// ... return a pointer to the corresponding resource
		return itr->second;
	// ... else return 0
	throw Exceptions::URINotFound();
	return 0;
}

template <class Resource>
Resource& ResourceManagerB<Resource>::Get(URI& Uri)
{
	// get a pointer to the resource
	Resource* temp = GetPtr(Uri);
	// if the resource was found...
	if (temp)
		// ... dereference the pointer to return a reference
		// to the resource
		return *temp;
	else
		// ... else throw an exception to notify the caller that
		// the resource was not found
		throw Exceptions::URINotFound();
}


//////////////////////////////////////////////////////////////////////////////////

class dxResource
{
public:
	URI* path;
	inline dxResource(URI pPath) { path = &pPath; }// Just an interface
};