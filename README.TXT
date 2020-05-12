////////////////////////////////////////////////////////////////////////////////
// TLSIM
// A TINY LOGIC CIRCUIT SIMULATOR
// (C) 2019, 2020 BY ARMCODER - milton@armcoder.com.br
//
// THIS PROGRAM IS FREE SOFTWARE
// SEE LICENSE AT https://www.gnu.org/licenses/old-licenses/gpl-2.0.txt
////////////////////////////////////////////////////////////////////////////////

    Version 1.0
    May 10, 2020

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
    
    Probably a lot. It's working for me, but this is not a great argument, of
course.

    The screen layout deserves some redesign. 

    This is still a work in progress, I'm in the final stages of completing the
BEC's original design, that is:

    * Implement the microcode for the last instructions (HLT, etc)
    
    * Implement the flags registers
    
    * Implement the enhanced microcode for the flags registers
     
    * Last UI tweaks

    * (Future) - Graphical UI?


    QUICKSTART
    ~~~~~~~~~~

    This version can already run the first BEC's program, that one that adds two
numbers (in Ben's example, 14 + 28 = 42).

    The EEPROM's microcode is automatically initialized, but the RAM contents 
must be inserted by the user through the UI.
    
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
    
    
    
    
    