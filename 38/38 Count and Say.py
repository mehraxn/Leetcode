class Solution:
    def countAndSay(self, n: int) -> str:
        def count_and_say(input_integer):
            num_string = str(input_integer)
            temp = ""
            counter = 0
            i = 0
            counter = 0
            num_list = []

            while i <= len(num_string) - 1:
                if (i < len(num_string) - 1 and num_string[i] == num_string[i + 1]):
                    counter += 1
                    i += 1
                elif (i == len(num_string) - 1 and num_string[i] == num_string[i - 1]):
                    counter += 1
                    num_list.append([num_string[i], counter])
                    i += 1
                elif (i == len(num_string) - 1 and num_string[i] != num_string[i - 1]):
                    num_list.append([num_string[i], counter + 1])
                    i += 1
                    counter = 0
                elif (num_string[i] != num_string[i + 1] and counter >= 1):
                    num_list.append([num_string[i], counter + 1])
                    i += 1
                    counter = 0
                elif (num_string[i] != num_string[i + 1] and counter == 0):
                    num_list.append([num_string[i], counter + 1])
                    i += 1
                    counter = 0

            for i in range(len(num_list)):
                temp += str(num_list[i][1]) + num_list[i][0]  

            return temp

        A = 1
        for _ in range(n - 1):
            A = count_and_say(A)
        return str(A)
