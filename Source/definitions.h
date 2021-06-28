#ifndef DEFINITIONS_H
#define DEFINITIONS_H


//#define RAND_MAX_ROADEF ((1U << 31) - 1)

//#include <stdint-gcc.h>
//lectura y escritura
#include <stdint.h>
#include <cstdlib>
#include <string>
#include <string.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <numeric>
#include <list>
#include <map>
#include <random>
#include <thread>
#include <sys/timeb.h>
#include <iostream>
#include <fstream>
#include <functional>
#include <set>
#include <sstream>

#include "main.h"
//#include "simdjson.h"
#include "json.hpp"

/*/
#include "GlassDefect.h"
#include "GlassItem.h"
#include "GlassNode.h"
#include "GlassNodeB.h"
#include "GlassPlate.h"
#include "GlassStack.h"
#include "GlassRectangle.h"
#include "GlassStructures.h"
#include "GlassCorte.h"
*/

#ifdef __linux__ 
//linux code goes here


#endif
#define get_random(min, max) ((rand() % (int)(((max)+1) - (min))) + (min))     
#define get_random_f(min, max) ((rand() % (double)(((max)+1) - (min))) + (min)) 
#define RAND_MAX_ROADEF ((1U << 31) - 1)



//This function give back the new rectangle that we have to pack
/*GlassRectangle * AsignarRectangulo(GlassRectangle &rectangulo)
{

};*/
#endif
