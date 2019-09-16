# FinalProject
At your Command
What was originally going to be a 2d platformer is now a 2d sandbox game similar to the old flash game, "Interactive Buddy".  The player is Professor Evans. As a sandbox, there is no real objective in the game, but rather, there are lots of little features to play around with.  See below for the list of valid commands and what they do.  In this game, your actions and commands will affect Professor Evans and will affect his mood.  His facial expressions will change according to his mood, but no matter how happy or upset he gets, the emotions will go back to equilibrium over time.

# Commands
"RESET"
- resets all the shapes and objects in the game

"GRAVITY LEFT"
- moves the current gravity force to the left

"GRAVITY RIGHT"
- moves the current gravity force to the right

"GRAVITY DOWN"
- moves the current gravity force down

"GRAVITY UP"
- moves the current gravity force up

"SHATTER"
- removes all the joints in the player body

"DECREASE GRAVITY"
- decreases gravity

"INCREASE GRAVITY"
- increases gravity

"INCREASE FRICTION"
- increases player body friction

"DECREASE FRICTION"
- decreases player body friction

"INCREASE DENSITY"
- increases player body density

"DECREASE DENSITY"
- decreases player body density

"INCREASE BOUNCE"
- increases player body bounce

"DECREASE BOUNCE"
- decreases player body bounce

"PARTY MODE"
- (potentially seizure inducing, depending on the frequency rate and sensitivity of the people watching, default freq rate should be good? no guarantees) This mode plays music and makes the screen change colors. 

"BORING MODE"
- turns off party mode and sets the game back to the default

"GIVE ME AN A"
- evans gives me an "A" and it makes him happy because that's what he wants to do.

"KNEES WEAK ARMS SPAGHETTI"
- (reference to the old meme based on the Mom's Spaghetti remix for Lose Yourself by Eminem) Evan's loses the T-pose position and his joints go limp like spaghetti.

"GOT YOUR NOSE"
- removes Evan's nose and turns it into a seperate object in the game

"GIVE NOSE BACK"
- gives Evan's his nose back

# Controls (not commands)
Left click with mouse
- if hovering over an object, this will allow you to grab and move around said object using physics. note, due to the scaling of objects in the game, you'll find that you're mouse moves much faster you think.  Pay attention to the on-screen speed and max g force stats, both for knowing if you're being too rough, and for fun.

Right click and move with mouse
- this will all you to draw lines on the screen that are static and have physics interactions

Delete
- pressing the delete key will remove the lines that were drawn with right click

'+'
- pressing the '+' key will spawn a letter A physics object at the mouse location

'-' 
- pressing the '-' key will spawn an xkcd physics object at the mouse location

# Default Happiness Values
These values correspond to different events in the game. Everytime the game is updated, the emotion meter either goes up by 1, or down by 1, depending on whether it is currently positive or negative (it tries to return to equilibrium 0). The starting emotion meter value is 500
- xkcd happiness = 300;
- k_scary_physics_happiness = -3; //note that this value is updated very rapidly, so it is cumulatively significant
- k_dangerous_physics_happiness = -10; //note that this value is updated very rapidly, so it is cumulatively significant
- k_deadly_physics_happiness = -500; //note that this value is updated very rapidly, so it is cumulatively significant
- scary physics happiness = -100;
- party mode happiness = 3000;
- boring mode happiness = -1000;
- shatter happiness = -1000;
- limp joint happiness = -500;
- giving A grade happiness = 300;
- taking nose happiness = -300;
- giving nose happiness = 200;
- happy lower bounds = 100;
- content lower bounds = 50;
- normal lower bounds = -50;

# Evans Physical Limits
due to box2d scaling issues, we're just gonna pretend that evans is a superhuman astronaut, as, when put at realistic values
it is very difficult to not terrify/kill him.  These values are more closely related to when the ofxbox2d revolute joints will start to fail. (G-forces around 30 tend to have the potential to completely rip his head off, while G-forces around 15 make the neck act very weird and in anatomically dangerous ways)

- k_g_force_nausea = 8;
- k_g_force_danger = 15;
- k_g_force_deadly = 30;
