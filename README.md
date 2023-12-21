# Timer

Developed a Timer App, in group of 3, displayed on a PC terminal using UART and Timer peripherals. The app is controlled through push buttons connected to the microcontroller's input ports.

![Image](/emb.jpg)

1. Outputs are displayed on a PC terminal via a UART module.
2. Timer Functionality:
   - **Minute Setting:** Button 1 to adjust minutes, incrementing sequentially from 0 to 59.
   - **Second Setting:** Button 2 is designated for setting seconds.
   - **Timer Control:**  Button 3 enables starting and pausing the timer at any point during its operation.
   - **LED Indicator:**  LED that blinks every second during the timer countdown and remains continuously lit when the alarm is activated upon timer completion.
   - **Interrups:**      Utilized interrupts of the microcontroller for registering inputs on the buttons


#### Here is a State Diagram to visualize the behaviour of the App:

![Image](/state.jpg)
