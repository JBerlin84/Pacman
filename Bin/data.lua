-- direction: 0=none, 1=up, 2=down, 3=left, 4=right
DIRECTION_UP = 1
DIRECTION_DOWN = 2
DIRECTION_LEFT = 3
DIRECTION_RIGHT = 4

-- player data
player = {x=0, y=0, direction=0}

-- emenies
blinky = {name="Blinky", x=14, y=12}
inky = {name="Inky", x=14, y=14}
pinky = {name="Pinky", x=14, y=16}
clyde = {name="Clyde", x=13, y=16}

-- enemy types
ENEMY_TYPE_BLINKY = 0
ENEMY_TYPE_INKY = 2
ENEMY_TYPE_PINKY = 4
ENEMY_TYPE_CLYDE = 6

-- wander coordinates
-- this will probably not work as maze_width and maze_height are not defined.
BLINKY_SCATTER_TARGET = {x=1, y=1}
PINKY_SCATTER_TARGET = {x=MAZE_WIDTH, y=1}
INKY_SCATTER_TARGET = {x=1, y=MAZE_HEIGHT}
CLYDE_SCATTER_TARGET = {x=MAZE_WIDTH, y=MAZE_HEIGHT}
