#ifndef routeh   // if x.h hasn't been included yet...
#define routeh  //   #define this so the compiler knows it has been included

class Route{
    public:
    Route(char*, int, char*);
    char* getNetwork();
    int getDistance();
    char* getThrough();
        
    private:
    char* network = new char[4];
    int distance;
    char *through = new char[10];
        
};

#endif
