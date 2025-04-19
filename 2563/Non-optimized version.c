long long countFairPairs(int* nums, int numsSize, int lower, int upper) {

    int counter =0;

    for (int i=0 ;i<numsSize-1 ;i++){
        for(int j=i+1;j<numsSize;j++){
            if(nums[i] + nums[j] >=lower && nums[i] + nums[j] <=upper)
                counter++;
        }

    }

    return counter;

}