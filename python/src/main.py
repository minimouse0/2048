from src.chessboard import *
from src.chessboard_renderer import *
from src.pos import *
def key2Direction(key:str):
    if key == "w":
        return Direction.Up
    if key == "a":
        return Direction.Left
    if key == "s":
        return Direction.Down
    if key == "d":
        return Direction.Right
    return Direction.NoDirection
chessboard=Chessboard(4,4)
chessboard.getGrid(IntPos(2,2)).place()
print(render_chessboard(chessboard))
for i in range(0,100):
    try:
        direction=key2Direction(input())
    except:
        print("无法读取输入")
        break
    chessboard.push(direction)
    chessboard.getGrid(IntPos(2,2)).place()
    print(render_chessboard(chessboard))
'''
chessboard.getGrid(IntPos(0,2)).place()
chessboard.getGrid(IntPos(0,1)).place().getPiece().push(Direction.Right)
chessboard.getGrid(IntPos(3,1)).getPiece().push(Direction.Down)
chessboard.getGrid(IntPos(3,3)).getPiece().push(Direction.Left)
chessboard.getGrid(IntPos(0,3)).getPiece().push(Direction.Up)
chessboard.getGrid(IntPos(0,1)).place()
chessboard.getGrid(IntPos(0,2)).place()
chessboard.push(Direction.Down)
print(render_chessboard(chessboard))
chessboard.getGrid(IntPos(0,1)).place()
chessboard.getGrid(IntPos(1,1)).place()
chessboard.getGrid(IntPos(2,1)).place()
chessboard.getGrid(IntPos(3,1)).place()
chessboard.push(Direction.Right)
print(render_chessboard(chessboard))
'''
