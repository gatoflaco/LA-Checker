/* LA-Checker by Isaac Jung
Last updated 02/25/2022

|===========================================================================================================|
|   This file contains just the deconstructor for the Factor class. The Interaction class, which is also    |
| defined in factor.h, has no deconstructor, because it requires no extra heap memory when instantiated.    |
|===========================================================================================================| 
*/

#include "factor.h"

/* CONSTRUCTOR - initializes the object
*/
Factor::Factor()
{
    id = -1;
    level = -1;
    interactions_size = 0;
    interactions = nullptr;
}

/* DECONSTRUCTOR - frees memory
*/
Factor::~Factor()
{
    for (int i = 0; i < interactions_size; i++) delete[] interactions[i];
    delete[] interactions;
}