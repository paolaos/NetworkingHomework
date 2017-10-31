#ifndef actionh   // if x.h hasn't been included yet...
#define actionh  //   #define this so the compiler knows it has been included

class Action{
    public:
        Action(int, char*);
	Action();
	int getType();
        char* getIP();

    private:
	int type;
	char* ip = new char[4];
};

#endif
