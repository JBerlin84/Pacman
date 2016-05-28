--MAZE_HEIGHT=31
--MAZE_WIDTH=28

DIRECTION_NONE = 0
DIRECTION_UP = 1
DIRECTION_DOWN = 2
DIRECTION_LEFT = 3
DIRECTION_RIGHT = 4

-- player data
player = {x=0, y=0, direction=0}

-- emenies
blinky = {name="Blinky", x=14, y=12, direction=DIRECTION_NONE, scatter={}}  -- red ghost
inky = {name="Inky", x=14, y=14, direction=DIRECTION_NONE, scatter={}} -- blue
pinky = {name="Pinky", x=14, y=16, direction=DIRECTION_NONE, scatter={}} -- pink
clyde = {name="Clyde", x=13, y=16, direction=DIRECTION_NONE, scatter={}} -- orange

deadTarget = {x=12, y=16}

-- Game states
GAME_STATE_CHASE = 0
GAME_STATE_SCATTER = 1
GAME_STATE_FRIGHTENED = 2
