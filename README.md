Team Number: 35
=================================

Team Members
 - Ade Banjo
 - Owen Brouse
 - Samarjit Bhullar
 - Lasitha Amuwala

GITHUB: https://github.com/lasitha-amuwala/CES-Simulator/

=================================

Who did what in the project
 - Lasitha Amuwala
    - Menu System
      - Menu class, (Menu.cpp, Menu.h)
      - dynamically adding menu items to menus
      - displaying menu on the GUI
      - Getting UP and Down Buttons to work with Menu
      - Getting navigating between different menus to work
    - Power ON/OFF button functionality
      - Disabling Buttons when power off
    - Device Shutdown screen
    - Most of the GUI design
      - UI Overlay done on Illustrator
      - All Button Designs done on illustrator
      - Electrodes Design
      - GUI styling (CSS)
      - Control panel arrangement
      - added device autoshutdown timer to ui
      - added Current level bar on ui and functionality
      - added battery level bar on ui and functionality
    - Auto shutdown and disable when current exceeds 700uA logic
      - increasing current by 50, decreasing current by 100 
    - destructured pointers
    - Traceability Matrix
    - Sequence Diagrams

 - Owen Brouse
   - Use Cases Diagrams
   - GUI design
   - Systems Coding

 - Samarjit Bhullar
     - Class Diagram (updated several times based on design and functionality changes)
     - Device Idle Design and functionality (30 minutes)
     - Electrodes Idle Design functionality (5 seconds)
     - Device Shutdown Test Design and functionality (30 seconds)
     - Added Main Window Title and Icon
     - Code Documentation
     - Fixed Battery Drain down bug
     - Test Cases and Documentation (updated several times based on design and functionality changes)
     - Run Test Cases

 - Ade Banjo

=================================

File Organization:
 /resources  -  all button images, device overlay image, application icon
 /DesignDocumentation   -   UML Diagrams, Sequence Diagrams, Traceability Matrix, Use Cases
 /CES-Simulator   -   source Code

=================================

Tested Scenarios

What works:
- Battery Low warning and shutdown
- Current fault over 700uA causing permanent shutdown and disable device.
- Electrodes disconnect warnings, therapy session shutdown after 5 seconds of disconnect
- Therapy resumes after reconnect within 5 seconds
- ON/OFF switch
- Changing Frequencies, Wave forms, Countdown cycles
- increasing / decreasing current level
- 30 min auto shutdown when system is inactive
- 30 seconds auto shutdown test (to be used during demo)
- Therapy starts when electrodes are connected, timer displays
- Battery drain
- Therapy recording
