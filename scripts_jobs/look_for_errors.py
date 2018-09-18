import os

f=os.popen("ls -t out_err/*.stderr | sort")
summary_file = open("error_summary.txt", 'w')
list_of_files = open("error_files.txt", 'w')
max_nlines = 20
    
for i in f.readlines():
    in_list = False
    with open(i[:-1]) as error_file:
        error = False
        counter = -1
        for line in error_file.readlines():
            if ("unknown branch" in line) or ("bytes" in line) or ("mkdir" in line) or ("records" in line):
                continue
            else:
                summary_file.write(i + "\n")
                if not in_list: 
                    list_of_files.write(i)
                    in_list = True
                error = True
                counter = 0
            if (error):
                summary_file.write(line)
                counter += 1
            if (counter > max_nlines):
                error = False
                summary_file.write("\n\n\n")
                counter = -1

summary_file.close()
list_of_files.close()
