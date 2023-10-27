from src.chessboard import *
from src.chessboard_renderer import *
from src.pos import *

def startConsoleGameLoop():
    while chessboard.checkGameOver() is False:
        try:
            direction=key2Direction(input())
        except:
            print("无法读取输入")
            break
        chessboard.playerPush(direction)
        print(refresh_chessboard(chessboard))
    print("死了啦，都你害的啦")
def key2Direction(key:str):
    if key.startswith("w"):
        return Direction.Up
    if key.startswith("a"):
        return Direction.Left
    if key.startswith("s"):
        return Direction.Down
    if key.startswith("d"):
        return Direction.Right
    return Direction.NoDirection
chessboard=Chessboard(4,4)
#chessboard.getGrid(IntPos(2,3)).place(2)
chessboard.randomNew()
print(refresh_chessboard(chessboard))
startConsoleGameLoop()



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
