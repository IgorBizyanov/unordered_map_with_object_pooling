#pragma once
#include <vector>
#include <unordered_set>
using namespace std;

template <class TableCell>
class ObjectPool
{
private:

	unordered_set<TableCell*> locked_objects_pointers;
	unordered_set<TableCell*> free_objects_pointers;

public:

	TableCell* createNewObject()
	{
		if (free_objects_pointers.size() != 0) {
			TableCell* p = *free_objects_pointers.begin();
			locked_objects_pointers.insert(p);
			free_objects_pointers.erase(p);
			return p;
		}

		TableCell* newcell = new TableCell;
		locked_objects_pointers.insert(newcell);
		return newcell;
	}

	void deleteObject(TableCell* object) 
	{
		locked_objects_pointers.erase(object);
		free_objects_pointers.insert(object);
	}

	virtual ~ObjectPool()
	{
		for (auto& e : locked_objects_pointers) 
			delete e;
		for (auto& e : free_objects_pointers) 
			delete e;
	}
};