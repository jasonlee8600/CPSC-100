#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "decimal.h"


int decimalAdd(size_t precision, char augend[], const char addend[]) {
    
    // Check if either argument has bad digits
    for (size_t i = (precision - 1); (precision > i) && (i >= 0); i--)
    {
        if ((augend[i] > 9) || (augend[i] < 0) || (addend[i] > 9) || (addend[i] < 0)) 
        {
            return DECIMAL_BAD_DIGIT;
        }
    }

    // Temporary array to hold sum in and variables for carry and array index addition sum
    char tmp[precision];
    int carry = 0;
    int indexSum = 0;

    // Loop through augend and addend, adding their values and checking for carry
    for (size_t i = 0; i < precision; i++)
    {
        indexSum = augend[i] + addend[i] + carry;

        if (indexSum > 9) 
        {
            carry = 1;
            indexSum %= 10;
        }

        // In case carry needs to be reset to zero
        else 
        {
            carry = 0;
        }
        
        // Places array index sum into tmp array
        tmp[i] = indexSum;
    }
    
    // If carry equals 1 after iterating entire array
    if (carry == 1)
    {
        return DECIMAL_OVERFLOW;
    }

    // Copy tmp into augend
    else 
    {
        for (size_t i = 0; i < precision; i++)
        {
            augend[i] = tmp[i];
        }
    }
    
    return DECIMAL_OK;
}




// fix result going to other end of number range instead of becoming negative

int decimalSubtract(size_t precision, char minuend[], const char subtrahend[]) {

    // Check if either argument has bad digits
    for (size_t i = (precision - 1); (precision > i) && (i >= 0); i--)
    { 
        if ((minuend[i] > 9) || (minuend[i] < 0) || (subtrahend[i] > 9) || (subtrahend[i] < 0)) 
        {
            return DECIMAL_BAD_DIGIT;
        }
    }   
    
    // Temporary array to hold result in 
    // leftover variable to hold result from array index subtraction
    // carry variable to see if needed to carry 10 because subtrahend >
    char tmp[precision];
    int leftover = 0;
    int carry = 0;

    // Copy minuend into tmp array
    for (size_t i = 0; i < precision; i++)
    {
        tmp[i] = minuend[i];
    }

    for (size_t i = 0; i < precision; i++)
    {
        if (carry == 1)
        {
            // If subtrahend index value bigger, carry 10 and subtract (including carry).
            if (subtrahend[i] >= tmp[i])
            {
                tmp[i] += 10;
                leftover = tmp[i] - subtrahend[i] - carry;
            }
            
            else
            {
                leftover = tmp[i] - subtrahend[i] - carry;
                carry = 0;
            }
        }

        else 
        {
            // If subtrahend index value bigger, carry 10 and subtract.
            if (subtrahend[i] > tmp[i])
            {
                tmp[i] += 10;
                leftover = tmp[i] - subtrahend[i];
                carry = 1;
            }

            else 
            {
                leftover = tmp[i] - subtrahend[i];
            }
        }
        

        // Store leftover in temporary array
        tmp[i] = leftover;
    }

    // If need to carry at end of array, means result would be negative
    if (carry == 1)
    {
        return DECIMAL_OVERFLOW;
    }
    

    // Check if tmp array has bad digits from negative result
    for (size_t i = 0; i < precision; i++)
    { 
        if ((tmp[i] > 9) || (tmp[i] < 0)) 
        {
            return DECIMAL_BAD_DIGIT;
        }
    }   

    // Copy tmp into minuend
    for (size_t i = 0; i < precision; i++)
    {
        minuend[i] = tmp[i];
    }

    return DECIMAL_OK;
}





void decimalPrint(size_t precision, const char number[]) {


    // Prints 0 if precision is 0
    if (precision == 0)
    {
        printf("0");
        return;
    }

    // for iterating array
    size_t i = (precision - 1);

    // Check for leading zeros
    while ((number[i] == 0) && (i != 0))
    {
        i--;
    }    

    // If i == 0, parsed whole array with zeros so output is just 0
    if ((i == 0) && (number[i] == 0))
    {
        printf("0");
        return;
    }
        
    // Checks each array value for bad digits
    for (size_t j = i; (i >= j) && (j >= 0); j--)
    {
        if ((number[j] > 9) || (number[j] < 0))
        {
            printf("%s", DECIMAL_BAD_OUTPUT);
            return;
        }
    }

    // Prints array if no bad digits
    for (size_t k = i; (i >= k) && (k >= 0); k--)
    {
        printf("%d", number[k]);
    }

    return;
}
