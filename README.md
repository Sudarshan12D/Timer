# Timer

Developed a Timer App displayed on a PC terminal using UART and Timer peripherals. The app is controlled through push buttons connected to the microcontroller's input ports.

![Image](/emb.jpg)

1. Used UART module to display the outputs on the pc terminal
2. Has the functionalities of a Timer:
   - Button 1 to set Minutes (increments from 0 to 59 one-by-one).
   - Button 2 to set Seconds.
   - Button 3 to start the timer and pause it anytime during the runtime.
   - Used a LED light which blinks after every second passes and remains ON when Alarm has been triggerred (when timer has finished).
