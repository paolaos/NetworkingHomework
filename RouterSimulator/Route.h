#ifndef routeh   // if x.h hasn't been included yet...
#define routeh  //   #define this so the compiler knows it has been included

class Route{
    public:
    Route(char*, int, char*);
    char* getNetwork();
    int getDistance();
    char* getThrough();
        
    private:
    char* network;
    int distance;
    char *through;
        
};

#endif
