# include <stdio.h>
#include <stdlib.h>

int peripheral(char input){
    if (input == 'I')
        return 1;
    else if (input == 'V')
        return 5;
    else if (input == 'X')
        return 10;
    else if (input == 'L')
        return 50;
    else if (input == 'C')
        return 100;
    else if (input == 'D')
        return 500;
    else if (input == 'M')
        return 1000;
    return 0;
}

int romanToInt(char* s) {
    int sum=0;
    char *temp=s;
    int temp_number=0;
    int temp_number_second=0;
    for (int i=0; (*temp)!='\0';temp++,i++){
        temp_number=peripheral(*temp);
        sum+=temp_number;
        if (temp_number==5 && temp_number_second==1){
            sum-=(temp_number+temp_number_second);
            sum+=4;
        }
        else if (temp_number==10 && temp_number_second==1){
            sum-=(temp_number+temp_number_second);
            sum+=9;
        }
        else if (temp_number==50 && temp_number_second==10){
            sum-=(temp_number+temp_number_second);
            sum+=40;
        }
        else if (temp_number==100 && temp_number_second==10){
            sum-=(temp_number+temp_number_second);
            sum+=90;
        }
        else if (temp_number==500 && temp_number_second==100){
            sum-=(temp_number+temp_number_second);
            sum+=400;
        }
        else if (temp_number==1000 && temp_number_second==100){
            sum-=(temp_number+temp_number_second);
            sum+=900;
        }

        temp_number_second=temp_number;
    }

    return sum;

}