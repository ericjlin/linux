Eric Lin

# ModifyingKVMBehavior
Assignment2

### 1. Work Distribution
  - I did this assignment by myself.

### 2. Steps taken to setup environment and complete assignment:
  - Environment setup done in assignment 1
  - Make edits to cpuid.c and vmx.c 
    - added spinlock to handle exits concurrently since the VM has more than one CPU
    - main edit included declaring two global counters, one for vm exit count and one for number of cycles in vm
  - Build the kernel using the same command used when first setting up environment for assignment 2
    - make -j 3 modules && make -j 3 && sudo make modules_install && sudo make install
      - since the initial build was already run in assignment 1, each build won't take all night
    - after make, reboot the computer to reflect new changes
  - Install virtual manager and run a VM in the VM
    - sudo apt install virt-manager
    - virt-manager
  - Add test file to the nested virtual manager to check for number of exits and how many cycles spent in the exit.
    See link below to view test script
    - https://github.com/ericjlin/linux/blob/master/test_assignment2.c
    - Basically called cpuid function from include/asm/processor.h which takes in a extra parameter,op, and set eax to it. This is how I call the 0x4fffffff leaf function 
  - Sample Output:
  ![alt text](https://github.com/ericjlin/linux/blob/master/assign2_sample_test.png)
### 3. Comments on the frequency of exits:
  - The rate of number of exits seem to increase at a constant rate. It'll increase around 1000 - 2000 exits without me doing much on the VM. Idle cycle time increase can vary, with observed cycle time ranging from 30,000,000 to 60,000,000 cycles. If I run the cpuid command on linux command line, the number of exits is around 1000 to 3686. The number of cycles spent running the command line cpuid is around 49,723,776. For creating a file, writing to it and saving it to the VM, it takes around 9,161 exits and 307,421,254 cycles. A full vm boot involves around 616,692 exits and 9,643,215,531 cycles. It looks like boot up is one of the longest operations that take a lot of exits and time to complete. File operations like creating a file is second in terms of how much exits and cycles spent on the operation from what I've seen so far with the testing.

# Instrumentation Hypercall
Assignment3

### 1. Work Distribution
  - I did this assignment by myself.

### 2. Steps taken to setup environment and complete assignment:
   - Build on top of assignment 2
   -  Make edits to cpuid.c and vmx.c
      - Add a new leaf, 0x4ffffffe, to cpuid.c and have it handle the exit type count
   -  Build the kernel using the same command used when first setting up environment for assignment 2
      - make -j 3 modules && make -j 3 && sudo make modules_install && sudo make install
      -  reboot the VM to see change
   - Run "virt-manager" to create a new VM
      - May use assignment2 VM or create a new one, use default settings
   - In the nested VM, add test function which will call cpuid with eax set to 0x4ffffffe and ecx to exit type number
      - In the test function, use __cpuid_count function which allows ecx to be set from the test script.
   - Build test function and run it to view exit type counts
      - My test function loops through all 69 exit types and returns the count for each one  
### 3. Comment on the frequency of exits
  The rate of number of exits seem to increase at a constant rate. It'll increase around 1000 - 2000 exits without me doing much on the VM. Idle cycle time increase can vary, with observed cycle time ranging from 30,000,000 to 60,000,000 cycles. If I run the cpuid command on linux command line, the number of exits is around 1000 to 3686. The number of cycles spent running the command line cpuid is around 49,723,776. For creating a file, writing to it and saving it to the VM, it takes around 9,161 exits and 307,421,254 cycles. A full vm boot involves around 616,692 exits and 9,643,215,531 cycles. It looks like boot up is one of the longest operations that take a lot of exits and time to complete. File operations like creating a file is second in terms of how much exits and cycles spent on the operation from what I've seen so far with the testing.

### 4. Of the exit types defined in the SDM, which are the most frequent? Least?

  The most frequent exit type are exit type 28, 31 and 48. Exit type 48, EPT Violation, has the most exit type with 229,108 exits. Exit type 28, control-register accesses, comes in second at 31,145 exits. Exit type 31, RDMSR, comes in third with 639 exits. On the otherhand, exit type 29, MOV DR, and exit type 46, Access to GDTR or IDTR, were one of the least frequent exit types with only 1 exit recorded each. These exit types were recorded a little after boot up so there may be more after performing more operation on the VM. In total, there were 16 exit types activated which is pretty little considering how many exit types are in the Intel SDM. One thing to note, this is done on a very old computer. My computer is using a Haswell chip which is a 4th generation Intel chip which came out around 7 years ago so it's possible there are fewer exit types supported due to the age of the chip.

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

There seem to be a big increase in exit counts when using shadow paging vs nested paging. For example, exit type 0, exception or non-maskable interrupt, with ept had only 18.835 while without ept it was 2,079,800. There also a total of 18 exit types enabled on shadow paging vs nested paging. Note that this is done on an intel cpu released around 2013 so the exit types might be much lower than is shown in the current intel sdm. Without EPT, there were two extra exit types, exit type 14, INVLPG, and exit type 58, INVPCID. This is expected as in shadow paging we perform operation on the virtual TLB which would involve running the INVLPG command to mark corresponding entry in the active shadow table as not present. The counts are expected especially the count for exit type 28, exit on control-register access. Shadow paging relies on exiting whenever loading CR3 so the big increase in exit type 28 is expected. With ep, exit on exit type 28 is around 31,007, but when we use shadow paging it increases to 9,220,720 exits. Also in shadow paging, there are usually lots of page fault exiting which would explain the big count in exit type 0. Overall it was expected to see a increase in exit types when we run the VM without EPT compared to running with EPT.

### 4. What changed between the two runs (ept vs no-ept)?
One big change is the performance of the VM when running on no-ept. With no-ept, the VM was running really slow. Every action in the VM lagged and performance was low. Boot up time seem to take more than ten minutes when it used to only take a few minutes maybe seconds. Trying to open a web browser also takes a considerably more time with shadow paging compared to nested paging. Using vim to open files seemed fine, no performance hit when using shadow paging. As mentioned before, exit on CR3 seemed to have increased a lot via exit type 28 so that was to be expected. Also there seem to be two additional exits, exit on INVLPG and exit on INVPCID. This is due to shadow paging needing to flush the TLB which involves running the INVLPG command. Overall there seems to be a significant increase in exit count accross the board in no-ept run compared to a regualr ept run.
