
#include "snowflake.h"

#include <stdio.h>


int main(){

    snowflake* a = initSnowflake(0, 0, 0, -1);
    snowflake* b = initSnowflake(10, 10, 10, -1);

    setEllipses(a, 6, 6 ,6);
    setEllipses(b, 5, 5 ,5);

    if(boxCollide(a, b)){
        printf("Collision");
    }


}



