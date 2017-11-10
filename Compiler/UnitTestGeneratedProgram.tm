*
*	RUN TIME!  gets command line args and calls main
*	           R1 is used as temp register
*
0: ST  0, 3(0)   ; Storing access link before call to main
1: LDC  6, 4(0)   ; Adjust status pointer before call to main
2: LDA  1, 2(7)   ; Setting return adress and storing in temp register R1
3: ST  1, 2(0)   ; Storing the return adress in DMEM at the control link slot
4: LDA  7, 18(7)   ; Jump to main
5: LD  1, 1(5)   ; Load main's return value
6: OUT  1, 0, 0   ; Printing main return value
7: HALT  0, 0, 0   ; End of program

*
*	print
*
8: ST  2, 0(6)   ; Store register 2 into ( new status pointer) 
9: ST  3, 1(6)   ; Store register 3 into (new status pointer + 1) 
10: ST  4, 2(6)   ; Store register 4 into (new status pointer + 2) 
11: ST  1, 3(6)   ; Store register 1 (the old status pointer) into ( new status pointer + 3) 
12: LDC  1, 3(0)   ; Store constant 3 in R1 
13: ADD  5, 1, 6   ; Adjust top of stack to be (new status pointer + 3) 
14: LD  1, -3(6)   ; Loading the value of whatever argument is passed to print to R1
15: OUT 1, 0, 0   ; Printing the value of whatever argument is passed to print
16: ADD  1,6,0   ; Storing a temp copy of status pointer in R1
17: LD  2, 0(6)   ; restore register 2 from (status pointer) 
18: LD  3, 1(6)   ; restore register 3 from (status pointer + 1) 
19: LD  4, 2(6)   ; restore register 4 from (status pointer + 2) 
20: LD  6, 3(6)   ; restore register 6 from (status pointer + 3) 
21: LD  5,-1(1)   ; Restoring the stack top variable
22: LD  7,-2(1)   ; Restoring the return adress from the control link

*
*	main
*
23: ST  2, 0(6)   ; Store register 2 into ( new status pointer) 
24: ST  3, 1(6)   ; Store register 3 into (new status pointer + 1) 
25: ST  4, 2(6)   ; Store register 4 into (new status pointer + 2) 
26: ST  1, 3(6)   ; Store register 1 (the old status pointer) into ( new status pointer + 3) 
27: LDC  1, 3(0)   ; Store constant 3 in R1 
28: ADD  5, 1, 6   ; Adjust top of stack to be (new status pointer + 3) 
29: LDC 1, 1(0)   ; Push 1 into the temp reg R1
30: ADD 5, 1, 5   ; Incrementing Stack top by 1
31: LDC 1, 1(0)   ; Pushing the value of the print statement into a temp reg R1
32: ST 1, 0(5)   ; Storing into DMEM in the temp data objects slot. Print statement will grab the arg from here.
33: LD 1, -0(5)   ; Moving Temp arg 0 to R1
34: ST 1, 2(5)   ; Storing Temp arg 0 to Arg location in new stack frame
35: ST  5, 4(5)   ; Storing Access Link, about to call print
36: LDA  1, 4(7)   ; Saving return adress in temp register, about to call print
37: ST  1, 3(5)   ; Storing the return adress in DMEM at the control link slot
38: ADD  1, 6, 0   ; Copying current status pointer before function call, about to call print
39: LDA  6, 5(5)   ; Adjusting Status pointer, about to call print
40: LDA  7, 8(0)   ; Jump to print
41: LDC 1, 1(0)   ; Push 1 into the temp reg R1
42: ADD 5, 1, 5   ; Incrementing Stack top by 1
43: LDC 1, 2(0)   ; Pushing the value of the print statement into a temp reg R1
44: ST 1, 0(5)   ; Storing into DMEM in the temp data objects slot. Print statement will grab the arg from here.
45: LD 1, -0(5)   ; Moving Temp arg 0 to R1
46: ST 1, 2(5)   ; Storing Temp arg 0 to Arg location in new stack frame
47: ST  5, 4(5)   ; Storing Access Link, about to call print
48: LDA  1, 4(7)   ; Saving return adress in temp register, about to call print
49: ST  1, 3(5)   ; Storing the return adress in DMEM at the control link slot
50: ADD  1, 6, 0   ; Copying current status pointer before function call, about to call print
51: LDA  6, 5(5)   ; Adjusting Status pointer, about to call print
52: LDA  7, 8(0)   ; Jump to print
53: LDC 1, 3(0)   ; Pushing the return value of main into a register.
54: ST 1, -3(6)   ; Storing return value of main into its' stack frame
55: ADD  1,6,0   ; Storing a temp copy of status pointer in R1
56: LD  2, 0(6)   ; restore register 2 from (status pointer) 
57: LD  3, 1(6)   ; restore register 3 from (status pointer + 1) 
58: LD  4, 2(6)   ; restore register 4 from (status pointer + 2) 
59: LD  6, 3(6)   ; restore register 6 from (status pointer + 3) 
60: LD  5,-1(1)   ; Restoring the stack top variable
61: LD  7,-2(1)   ; Restoring the return adress from the control link
