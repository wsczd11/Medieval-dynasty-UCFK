## Medieval dynasty (Rock, Paper, Scissor Edition)
**Rock, Paper, Scissor**
- The game is similar to the normal Rock, Paper, Scissor but instead 
of 3 options we got 5 options to choose from:
  - Sword
  - Shield
  - Human
  - Gun
  - Grenade
- Each options consist of 2 wins and 2 loss:
  - `Sword` ***beats*** `Grenade` and `Shield`, but ***lose*** to `Gun` and `Human`
  - `Shield` ***beats*** `Gun` and `Human`, but ***lose*** to `Sword` and `Grenade`
  - `Human` ***beats*** `Gun` and `Sword`, but ***lose*** to `Shield` and `Grenade`
  - `Gun` ***beats*** `Grenade` and `Sword`, but ***lose*** to `Shield` and `Human`
  - `Grenade` ***beats*** `Human` and `Shield`, but ***lose*** to `Sword` and `Gun`

## HOW TO PLAY (FIRST TO WIN 3 GAMES)
![Funkit](/uploads/49118eeb09db0da9e853500c14962bec/Funkit.jpg)
1.  Using your terminal create a folder using "git clone https://eng-git.canterbury.ac.nz/ence260-2020/group-205.git". 
2.  Double click on the folder and open terminal.
3.  Both players will type "Make" into the terminal to compile the game and "Make program" after the compile has finish to play the game.
4.  LED Screen and Blue LED should light up on both boards and players showing `"WELCOME TO MEDIEVAL DYNASTY, MOVE NAVSWITCH TO START"`.
5.  Both players can move the nav switch to any direction to start the game. 
6.  Using the nav switch, moving it left and right to select which option the player will go for.
7.  After both of the players have decide their selection, push up on the nav switch to lock in what you have selected, LED screen will turn off so your opponent can't see what you chose.
8.  Then place the board next to each other and press on the button next to the LED screen to send signal between the boards and it will display either 'W', 'L' or 'S':
    - 'W' = Winner
    - 'L' = Loser
    - 'S' = Same/Tie
9. Repeat this process till one of the player wins 3 games. Everytime the person lost the round the speaker will start playing sound.
10. After one of the player have won 3 games, the player that loss will have their Blue LED turn off and winner will still be on.

