# Virtulization
Assignment4

### 1. Work Distribution
  - I did this assignment by myself.

### 2. Include a sample of your print of exit count output from dmesg from “with ept” and “without ept”.
Used test file in the nest VM to count exit types.

#### With Ept

![alt text](https://github.com/ericjlin/linux/blob/assignment4/with_ept_output1.png)
![alt text](https://github.com/ericjlin/linux/blob/assignment4/with_ept_output2.png)

#### Without Ept

![alt text](https://github.com/ericjlin/linux/blob/assignment4/withoutEPT_output1.png)
![alt text](https://github.com/ericjlin/linux/blob/assignment4/withoutEPT_output2.png)

### 3. What did you learn from the count of exits? Was the count what you expected? If not, why not?

There seem to be a big increase in exit counts when using shadow paging vs nested paging. For example, exit type 0, exception or non-maskable interrupt, with ept had only 18.835 while without ept it was 2,079,800. There also a total of 18 exit types enabled on shadow paging vs nested paging. Note that this is done on an intel cpu released around 2013 so the exit types might be much lower than is shown in the current intel sdm. Without EPT, there were two extra exit types, exit type 14, INVLPG, and exit type 58, INVPCID. The counts are expected especially the count for exit type 28, exit on control-register access. Shadow paging relies on exiting whenever loading CR3 so the big increase in exit type 28 is expected. With ep, exit on exit type 28 is around 31,007, but when we use shadow paging it increases to 9,220,720 exits. Overall it was expected to see a increase in exit types when we run the VM without EPT compared to running with EPT.

### 4. What changed between the two runs (ept vs no-ept)?
One big change is the performance of the VM when running on no-ept. With no-ept, the VM was running really slow. Every action in the VM lagged and performance was low. Boot up time seem to take more than ten minutes when it used to only take a few minutes maybe seconds. Trying to open a web browser also takes a considerably more time with shadow paging compared to nested paging. Using vim to open files seemed fine, no performance hit when using shadow paging. As mentioned before, exit on CR3 seemed to have increased a lot via exit type 28 so that was to be expected.
