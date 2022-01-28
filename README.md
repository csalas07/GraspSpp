# GraspSpp
This is a GRASP algorithm for Strip Packing Problem.

1) The variable "Comparetype" on line 56 is used to assign which comparison should be performed, 
the value of the variable must be manually modified in the code.

2) The algorithm reads a text file named "ListF", which is a list of files to process.

3) The processing of the instances starts when the screen shows the message "Procesando Instancias", 
"Processing Instances" in English.

4) When the instances were processed, the screen shows the message "Ya termine", "I'm done" in English.

5) As a result, the algorithm creates a text file with the compared author, 
the file number e.g. "1", the waste function used and the name of the file.

5.1) The file handles this format "Name,N,W,StrA,Lb,Ub,Best,Beta,Time".

Name = Filename.

N = Number of elements.

W = strip width.

StrA = Strip Area.

Lb = Lowerbound.

Ub = Uperbound.

Best = Best result.

Beta = Beta value. 

Time = Computational time.

6) Only the first switch case will return two text files, 
the first is a result file e.g. "TestAlvarezst_1_W1" with the compared author, the file number e.g. "1", and
the waste function used. The second file is the arrangement of the elements e.g. "AcomodoAlvarezst_1_W1" 
with the compared author, the file number e.g. "1", and the waste function used.

6.1) The test result file handles this format:

Dataset = Filename.

N = Number of elements.

Width = strip width.

StripArea = Strip Area.

WasteFunction = Waste Function used.

Lb = Lowerbound.

Ub =Uperbound.

L = Best result.

B = Beta value.

T = Computational time.

6.2) The arrangement file handles this format:

W = strip width.

N = Number of elements, 2.

Object width, Object height, X coordinate, Y coordinate.
