////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2021 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

    Version 1.1.3
    September 1, 2021

    INTRO
    ~~~~~
    
    Hi folks!

    TLSim is a pet project that I decided to build after discovering Ben Eater's
YouTube channel where he teaches how to build a very simple computer over some
breadboards using just TTL ICs, switches, LEDs and other discrete parts.

    For reference, the playlist with the complete tutorial is here:
   
    https://www.youtube.com/playlist?list=PLowKtXNTBypGqImE405J2565dvjafglHU
   
    Just for the record, I'm not affiliated with Ben Eater in any way, this is
simply a software inspired by his work, although I definitely recommend to check
his YouTube channel. He's a great teacher in the field of electronics, IMO.

    This project aims to accurately simulate Ben's implementation through 
software.


    BEN'S COMPUTER SIMULATORS
    ~~~~~~~~~~~~~~~~~~~~~~~~~

    TLSim is not the first BEC (Ben Eater's Computer) software simulator to hit
the road, and a quick web search will yield results in no time. But I decided to
write one anyway, because:

    1. It's fun.
    
    2. It's different from the others in the sense that is simulates every TTL
       chip on the board and their individual connections. It's almost like to
       build the real thing. That is, this is a chip-level model, rather than a
       module-level model.
       
    3. I needed something to do in my free time as going outside in times of
       COVID is not an option.    
    
    4. The TTL models and the engine can easily be reused in other designs. 
       Although the project is written in pure C, it's quite structured and
       hierarchical. It's almost tempting to rewrite it in C++ (no, I'm not
       planning to do so, not for now).
       
         
    ENVIRONMENT
    ~~~~~~~~~~~    
    
    This is (for now) a Linux-only project, since it was only tested in Ubuntu
20.04. I used Eclipse 2019-09 over OpenJDK 11, and the 'ncurses' and 'pthread' 
additional libraries are required to build.

    The program must run in a Terminal (X) window with at least 127 columns x 
50 rows. I could not run it correctly in native text mode, even in framebuffer
modes that supply large amounts of rows and columns. I don't know why.   
    
    With some tweaking it's possible to arrange the screen elements to take up
way less space, requiring smaller windows if needed.
    
    
    BEN'S COMPUTER VARIANTS
    ~~~~~~~~~~~~~~~~~~~~~~~
    
    Some fans created hardware projects that extended the BEC's functionality
(more instructions, more memory, etc). I have no plans, for now, to cater to
these variants.
    
    
    CAVEATS AND NOTES
    ~~~~~~~~~~~~~~~~~
    
    There are still probably lots of caveats. Several were been solved, though, 
and this project is still receiving regular improvements over time, slowly.

    When I reached version 1.0.7 I thought that the project was about to come to
an end. At that point it was able to run all Ben Eater's test programs perfectly
and was not planning to expand it any more, except for minor tweaks and fixes.

    However I always felt that the NCurses interface was not really satisfying,
so I'm researching how to reimplement TLSim with a true GUI. I think it deserves
such an improvement.


    QUICKSTART
    ~~~~~~~~~~

    The EEPROM's microcode is pre-initialized, and the RAM contents is 
automatically initialized by the 'exmachina.c' module that fires a thread that
simulates the needed user inputs to fill the RAM with a sample program. 

    After that the computer is started, but can be stopped and the RAM program
can be edited by the user through the UI using the keyboard.
    
    How each module works is explained in detail in the Ben's videos, I have
replicated them quite faithfully in the UI.

    The UI interface is controlled only by the keyboard. The keys are:
    
    * F2, F3: change focused Module (the active keys will be highlited)
    
    * F10: Pause/Step Clock
    
    * F11: Unpause/Slower Clock
    
    * F12: Unpause/Faster Clock
    
    * ESC: Quits the program
    
    Within a focused Module, the keys are usually alpha or digits, and are 
hinted within brackets.

    That's for now.

    Stay tuned and have fun!
    
    
    CHANGELOG
    ~~~~~~~~~
    
    V1.0.1 - May 11, 2020:
    
    - Created the "pulldown" object to condition the bus to read as zero when no
      peripheral is active.
    
    - Added the new instructions: SUB, STA, LDI, JMP and HLT.
    
    V1.0.2 - May 12, 2020:
    
    - Corrected the Creator and the Destroyer of the LS138 - some members were
      missing initialization/destruction.

    V1.0.3 - May 14, 2020:
    
    - FLAGS Register fully operational. Conditional Jumps implemented!
    
    - Slight screen relayout
    
    V1.0.4 - May 15, 2020:
    
    - Clock Step key(F10) faster response

    - Better screen presentation of the Bitswitch controls
    
    - Other screen small enhancements

    V1.0.5 - May 15, 2020:

    - Clock step - screen refresh correction
    
    - More screen layout tweaks
    
    - Microcode generation closer to Ben's implementation
    
    - Change in the style of the switches
    
    V1.0.6 - May 16, 2020:
    
    - Small code improvements - no changes in program operation

    V1.0.7 - May 21, 2020:
    
    - Eliminated almost all global variables, in an attempt to make the overall 
      code neater. Minor screen tweaks.
      
    V1.0.8 - May 23, 2020:
    
    - Last globals eliminated, and now 'gatetest.c' and 'testls191.c' can work
      (they are older tests not related to Ben's computer simulation). You can
      play with them by editing 'main.c'.

    V1.1.0 - May 23, 2020:
    
    - Created the Ex-Machina Autoloader. It simulates the needed keypresses to
      load a demo program into Ben's computer's RAM. It's simply a thread that
      injects fake keypresses into the engine, and is implemented into the file
      'exmachina.c'. There the demo program can be changed to whatever program
      the user wants, and it will work as long as it is compatible with the 
      Ben's computer architecture!

    V1.1.1 - May 26, 2020:
    
    - Better clock synchronization to avoid simulation glitches in slower 
      systems.

    V1.1.2 - June 25, 2020:
    
    - Grouping the main Computer Sim variables. 

    V1.1.3 - September 1, 2021:
    
    - Created the Turbo Switch like the one in GTLSim
