#include <stdio.h>


int lengthOfLastWord(char* s) {

    int longest = 0 ;

    char * char_temp=s;
    while ((*char_temp)!='\0')
        char_temp++;

    char_temp--;

    while( (*char_temp) == ' ')
        char_temp--;

    while ((*char_temp)!=' '  && char_temp!=s ){
        longest ++;
        char_temp--;
    }
    if (char_temp==s && (*char_temp) != ' ')
        longest++;

    return longest;

}