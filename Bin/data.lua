-- player data
player = {x=0, y=0, direction=0}

-- emenies
blinky = {name="Blinky", x=14, y=12, direction=DIRECTION_NONE}
inky = {name="Inky", x=14, y=14, direction=DIRECTION_NONE}
pinky = {name="Pinky", x=14, y=16, direction=DIRECTION_NONE}
clyde = {name="Clyde", x=13, y=16, direction=DIRECTION_NONE}

-- wander coordinates
-- this will probably not work as maze_width and maze_height are not defined.
BLINKY_SCATTER_TARGET = {x=1, y=1}
PINKY_SCATTER_TARGET = {x=MAZE_WIDTH, y=1}
INKY_SCATTER_TARGET = {x=1, y=MAZE_HEIGHT}
CLYDE_SCATTER_TARGET = {x=MAZE_WIDTH, y=MAZE_HEIGHT}
