#include <string>

class Part{
private:
	int id_number;
	std::string name;
	std::string type;

public:
	virtual Part() = 0;

};