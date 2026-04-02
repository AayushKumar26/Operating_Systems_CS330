USE ./lab.sh to initialize the session and get started.

usage: 
        ./lab.sh --roll|-r  <roll1_roll2> --labnum|-n <lab number> --action|-a <init|get|evaluate|prepare|prepare-save|submit|save|reload|detach|swupdate|signoff>
        Note your roll number string. Never forget or forge!
        
        [--action] can be one of the following
        
        init: Initialize the lab session
        get: Download the assignment
        evaluate: Evaluate the assignment
        prepare: Prepare a submission archive
        prepare-save: Prepare an archive to save
        submit: Submit the assignment. Can be perfomed only once!
        save: Save the assignment
        reload: Reload the last saved solution and apply it to a fresh lab archive
        detach: The lab session is detached. Can be reloaded using 'reload', if supported
        swupdate: Peform software update activities. Caution: Use only if instructed
        signoff: You are done for the lab session. Caution: After signoff, you will not be allowed to submit anymore

EXAMPLE
=======
Assume that your group members have the roll nos 210010 and 211101. 
Every lab will have a lab number (announced by the TAs). Assume lab no to be 5 for the examples shown below.

Fresh Lab? [Yes] 
{

    STEP 1        Initialize session  -->  $./lab.sh -r 210010_211101 -n 5 -a init
    STEP 2        Download the lab  -->  $./lab.sh -r 210010_211101 -n 5 -a get

   STEP {3 to L}   ----- WORK ON THE EXERCISE ------ 
        
   Completed? [Yes] 

        STEP L         Evaluate the exercise --> $./lab.sh -r 210010_211101 -n 5 -a evaluate
        STEP L+1       Prepare submission -->  $./lab.sh -r 210010_211101 -n 5 -a prepare        
        STEP L+2       Submit -->  $./lab.sh -r 210010_211101 -n 5 -a submit
        STEP L+3       Signoff --> $./lab.sh -r 210010_211101 -n 5 -a signoff      

   Completed? [No] 

        STEP L+1       Prepare to save your work -->  $./lab.sh -r 210010_211101 -n 5 -a prepare-save        
        STEP L+2       Save your work  -->  $./lab.sh -r 210010_211101 -n 5 -a save
        STEP L+3       detach --> $./lab.sh -r 210010_211101 -n 5 -a detach
}
Saved Lab? [Yes]
{
   STEP 1        Reload the lab   -->  $./lab.sh -r 210010_211101 -n 5 -a init

   STEP {2 to L}   ----- WORK ON THE EXERCISE ------ 
        
   Completed? [Yes] 

        STEP L         Evaluate the exercise --> $./lab.sh -r 210010_211101 -n 5 -a evaluate
        STEP L+1       Prepare submission -->  $./lab.sh -r 210010_211101 -n 5 -a prepare        
        STEP L+2       Submit -->  $./lab.sh -r 210010_211101 -n 5 -a submit
        STEP L+3       Signoff --> $./lab.sh -r 210010_211101 -n 5 -a signoff      

   Completed? [No] 

        STEP L+1       Prepare to save your work -->  $./lab.sh -r 210010_211101 -n 5 -a prepare-save        
        STEP L+2       Save your work  -->  $./lab.sh -r 210010_211101 -n 5 -a save
        STEP L+3       detach --> $./lab.sh -r 210010_211101 -n 5 -a detach

}


***** IMPORTANT ****

- Check the evaluation output
- Make sure you submit before signing off
- Make sure you save the lab before detaching (if you want to continue next)
- Make sure you signoff (STEP L+3) or else you will not get marks and will not get the submissions emailed
- Make sure you close all applications and logout from the system (not just the docker container)

***** CAUTION  *****
DO NOT DELETE lab.sh core or gem5
